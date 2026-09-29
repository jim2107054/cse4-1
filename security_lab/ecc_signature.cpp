#include <iostream>

using namespace std;


// =====================================================
// ECC POINT
// =====================================================

struct Point
{
    long long x;
    long long y;
    bool infinity;
};


// =====================================================
// MOD
// =====================================================

long long mymod(long long a, long long p)
{
    long long r = a % p;

    if (r < 0)
        r = r + p;

    return r;
}


// =====================================================
// GCD
// =====================================================

long long mygcd(long long a, long long b)
{
    if (a < 0)
        a = -a;

    if (b < 0)
        b = -b;

    while (b != 0)
    {
        long long r = mymod(a, b);

        a = b;
        b = r;
    }

    return a;
}


// =====================================================
// EXTENDED GCD
// =====================================================

long long extendedGCD(
    long long a,
    long long b,
    long long &x,
    long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;

        return a;
    }

    long long x1, y1;

    long long g =
        extendedGCD(
            b,
            mymod(a, b),
            x1,
            y1
        );

    x = y1;

    y = x1 - (a / b) * y1;

    return g;
}


// =====================================================
// MODULAR INVERSE
// =====================================================

long long ModularInverse(
    long long a,
    long long m)
{
    long long x, y;

    long long g =
        extendedGCD(
            a,
            m,
            x,
            y
        );

    if (g != 1)
        return -1;

    return mymod(x, m);
}


// =====================================================
// ECC POINT ADDITION
//
// Curve:
// y^2 = x^3 + ax + b mod p
// =====================================================

Point pointAddition(
    Point P,
    Point Q,
    long long a,
    long long p)
{
    // P + O = P

    if (P.infinity)
        return Q;


    // O + Q = Q

    if (Q.infinity)
        return P;


    // P + (-P) = O

    if (P.x == Q.x &&
        mymod(P.y + Q.y, p) == 0)
    {
        return {0, 0, true};
    }


    long long lambda;


    // =================================================
    // POINT DOUBLING
    // =================================================

    if (P.x == Q.x &&
        P.y == Q.y)
    {
        // lambda =
        // (3x1^2 + a) / (2y1)

        long long numerator =
            mymod(
                3 * P.x * P.x + a,
                p
            );

        long long denominator =
            mymod(
                2 * P.y,
                p
            );

        long long inverse =
            ModularInverse(
                denominator,
                p
            );

        lambda =
            mymod(
                numerator * inverse,
                p
            );
    }


    // =================================================
    // NORMAL POINT ADDITION
    // =================================================

    else
    {
        // lambda =
        // (y2 - y1) / (x2 - x1)

        long long numerator =
            mymod(
                Q.y - P.y,
                p
            );

        long long denominator =
            mymod(
                Q.x - P.x,
                p
            );

        long long inverse =
            ModularInverse(
                denominator,
                p
            );

        lambda =
            mymod(
                numerator * inverse,
                p
            );
    }


    // =================================================
    // CALCULATE x3
    // =================================================

    long long x3 =
        mymod(
            lambda * lambda
            - P.x
            - Q.x,
            p
        );


    // =================================================
    // CALCULATE y3
    // =================================================

    long long y3 =
        mymod(
            lambda * (P.x - x3)
            - P.y,
            p
        );


    return {
        x3,
        y3,
        false
    };
}


// =====================================================
// ECC SCALAR MULTIPLICATION
//
// kP = P + P + P + ...
// =====================================================

Point scalarMultiplication(
    long long k,
    Point P,
    long long a,
    long long p)
{
    Point result = {
        0,
        0,
        true
    };

    Point current = P;


    while (k > 0)
    {
        if (k % 2 == 1)
        {
            result =
                pointAddition(
                    result,
                    current,
                    a,
                    p
                );
        }


        current =
            pointAddition(
                current,
                current,
                a,
                p
            );


        k = k / 2;
    }


    return result;
}


// =====================================================
// ECDSA SIGNATURE GENERATION
//
// R = kG
//
// r = xR mod n
//
// s = k^(-1)(M + dr) mod n
// =====================================================

void generateSignature(
    long long M,
    long long k,
    long long d,
    Point G,
    long long a,
    long long p,
    long long n,
    long long &r,
    long long &s)
{
    // R = kG

    Point R =
        scalarMultiplication(
            k,
            G,
            a,
            p
        );


    // r = x-coordinate of R mod n

    r =
        mymod(
            R.x,
            n
        );


    // k^(-1) mod n

    long long k_inv =
        ModularInverse(
            k,
            n
        );


    // s = k^(-1)(M + dr) mod n

    s =
        mymod(
            k_inv *
            (M + d * r),
            n
        );
}


// =====================================================
// ECDSA SIGNATURE VERIFICATION
//
// w  = s^(-1) mod n
//
// u1 = M*w mod n
//
// u2 = r*w mod n
//
// X = u1G + u2Q
//
// v = xX mod n
//
// Signature valid if v == r
// =====================================================

bool verifySignature(
    long long M,
    long long r,
    long long s,
    Point G,
    Point Q,
    long long a,
    long long p,
    long long n)
{
    // w = s^(-1) mod n

    long long w =
        ModularInverse(
            s,
            n
        );


    if (w == -1)
        return false;


    // u1 = M*w mod n

    long long u1 =
        mymod(
            M * w,
            n
        );


    // u2 = r*w mod n

    long long u2 =
        mymod(
            r * w,
            n
        );


    // u1G

    Point P1 =
        scalarMultiplication(
            u1,
            G,
            a,
            p
        );


    // u2Q

    Point P2 =
        scalarMultiplication(
            u2,
            Q,
            a,
            p
        );


    // X = u1G + u2Q

    Point X =
        pointAddition(
            P1,
            P2,
            a,
            p
        );


    if (X.infinity)
        return false;


    // v = xX mod n

    long long v =
        mymod(
            X.x,
            n
        );


    return v == r;
}


// =====================================================
// MAIN
// =====================================================

int main()
{
    // =================================================
    // ECC DOMAIN PARAMETERS
    // =================================================

    // Curve:
    //
    // y^2 = x^3 + 2x + 2 mod 17

    long long p = 17;

    long long a = 2;

    long long b = 2;


    // Base point G

    Point G = {
        5,
        1,
        false
    };


    // Order of G

    long long n = 19;


    // =================================================
    // PRIVATE KEY
    // =================================================

    long long d = 7;


    // =================================================
    // PUBLIC KEY
    //
    // Q = dG
    // =================================================

    Point Q =
        scalarMultiplication(
            d,
            G,
            a,
            p
        );


    // =================================================
    // MESSAGE
    // =================================================

    long long M;

    cout << "Enter message (integer): ";

    cin >> M;


    // =================================================
    // RANDOM / NONCE k
    // =================================================

    long long k = 3;


    // k must be relatively prime to n

    if (mygcd(k, n) != 1)
    {
        cout << "k is not coprime to n\n";

        return 0;
    }


    // =================================================
    // GENERATE SIGNATURE
    // =================================================

    long long r, s;

    generateSignature(
        M,
        k,
        d,
        G,
        a,
        p,
        n,
        r,
        s
    );


    // =================================================
    // VERIFY SIGNATURE
    // =================================================

    bool valid =
        verifySignature(
            M,
            r,
            s,
            G,
            Q,
            a,
            p,
            n
        );


    // =================================================
    // OUTPUT
    // =================================================

    cout << "\n";


    cout << "Curve: y^2 = x^3 + "
         << a
         << "x + "
         << b
         << " mod "
         << p
         << "\n";


    cout << "Base Point G = ("
         << G.x
         << ", "
         << G.y
         << ")\n";


    cout << "Public Key Q = ("
         << Q.x
         << ", "
         << Q.y
         << ")\n";


    cout << "Private Key d = "
         << d
         << "\n";


    cout << "Random k = "
         << k
         << "\n";


    cout << "Signature (r, s) = ("
         << r
         << ", "
         << s
         << ")\n";


    if (valid)
        cout << "Signature VALID\n";
    else
        cout << "Signature INVALID\n";


    return 0;
}