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


// ---------------- MOD ----------------

long long mymod(long long a, long long b)
{
    long long r = a - (a / b) * b;

    if (r < 0)
        r = r + b;

    return r;
}


// ---------------- GCD ----------------

long long mygcd(long long a, long long b)
{
    if (a < b)
    {
        long long temp = a;
        a = b;
        b = temp;
    }

    if (b == 0)
        return a;

    return mygcd(b, mymod(a, b));
}


// ---------------- Extended GCD ----------------

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


// ---------------- Modular Inverse ----------------

long long ModularInverse(
    long long e,
    long long phi)
{
    long long x, y;

    long long g =
        extendedGCD(e, phi, x, y);

    if (g != 1)
        return -1;

    x = mymod(x, phi);

    if (x < 0)
        x = x + phi;

    return x;
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


    // x3 = lambda^2 - x1 - x2

    long long x3 =
        mymod(
            lambda * lambda
            - P.x
            - Q.x,
            p
        );


    // y3 = lambda(x1-x3)-y1

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
// ECC ENCRYPTION
//
// C1 = kG
//
// C2 = M + kQ
//
// Ciphertext = (C1, C2)
//
// Here M is an ECC point.
// =====================================================

void encrypt(
    Point M,
    long long k,
    Point G,
    Point Q,
    long long a,
    long long p,
    Point &C1,
    Point &C2)
{
    // C1 = kG

    C1 =
        scalarMultiplication(
            k,
            G,
            a,
            p
        );


    // kQ

    Point kQ =
        scalarMultiplication(
            k,
            Q,
            a,
            p
        );


    // C2 = M + kQ

    C2 =
        pointAddition(
            M,
            kQ,
            a,
            p
        );
}


// =====================================================
// ECC DECRYPTION
//
// M = C2 - dC1
//
// Since:
// dC1 = d(kG) = k(dG) = kQ
//
// Therefore:
// M = C2 - kQ
// =====================================================

Point decrypt(
    Point C1,
    Point C2,
    long long d,
    long long a,
    long long p)
{
    // dC1

    Point dC1 =
        scalarMultiplication(
            d,
            C1,
            a,
            p
        );


    // Find -dC1
    //
    // If P = (x,y)
    // then -P = (x,-y mod p)

    Point negativePoint = {
        dC1.x,
        mymod(-dC1.y, p),
        dC1.infinity
    };


    // M = C2 + (-dC1)

    Point M =
        pointAddition(
            C2,
            negativePoint,
            a,
            p
        );


    return M;
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


    // =================================================
    // BASE POINT
    // =================================================

    Point G = {
        5,
        1,
        false
    };


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
    //
    // For ECC ElGamal, message must be represented
    // as an ECC point.
    //
    // Here we use a valid point on the curve.
    // =================================================

    Point M = {
        6,
        3,
        false
    };


    // =================================================
    // RANDOM k
    // =================================================

    long long k = 3;


    // =================================================
    // ENCRYPTION
    // =================================================

    Point C1, C2;

    encrypt(
        M,
        k,
        G,
        Q,
        a,
        p,
        C1,
        C2
    );


    // =================================================
    // DECRYPTION
    // =================================================

    Point decrypted =
        decrypt(
            C1,
            C2,
            d,
            a,
            p
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


    cout << "Private Key d = "
         << d
         << "\n";


    cout << "Public Key Q = ("
         << Q.x
         << ", "
         << Q.y
         << ")\n";


    cout << "\nOriginal Message Point M = ("
         << M.x
         << ", "
         << M.y
         << ")\n";


    cout << "Random k = "
         << k
         << "\n";


    cout << "\nCiphertext:\n";

    cout << "C1 = ("
         << C1.x
         << ", "
         << C1.y
         << ")\n";


    cout << "C2 = ("
         << C2.x
         << ", "
         << C2.y
         << ")\n";


    cout << "\nDecrypted Message Point = ("
         << decrypted.x
         << ", "
         << decrypted.y
         << ")\n";


    if (decrypted.x == M.x &&
        decrypted.y == M.y)
    {
        cout << "\nDecryption SUCCESSFUL\n";
    }
    else
    {
        cout << "\nDecryption FAILED\n";
    }


    return 0;
}