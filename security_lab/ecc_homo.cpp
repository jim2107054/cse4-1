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


    // y3 = lambda(x1 - x3) - y1

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
// SCALAR MULTIPLICATION
//
// kP = P + P + P + ... k times
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
// PRINT POINT
// =====================================================

void printPoint(
    string name,
    Point P)
{
    if (P.infinity)
    {
        cout << name << " = O\n";
    }
    else
    {
        cout << name
             << " = ("
             << P.x
             << ", "
             << P.y
             << ")\n";
    }
}


// =====================================================
// MAIN
// =====================================================

int main()
{
    // =================================================
    // ECC PARAMETERS
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
    // TWO VALUES
    // =================================================

    long long m1 = 3;

    long long m2 = 5;


    // =================================================
    // m1G
    // =================================================

    Point P1 =
        scalarMultiplication(
            m1,
            G,
            a,
            p
        );


    // =================================================
    // m2G
    // =================================================

    Point P2 =
        scalarMultiplication(
            m2,
            G,
            a,
            p
        );


    // =================================================
    // LEFT SIDE
    //
    // (m1 + m2)G
    // =================================================

    Point left =
        scalarMultiplication(
            m1 + m2,
            G,
            a,
            p
        );


    // =================================================
    // RIGHT SIDE
    //
    // m1G + m2G
    // =================================================

    Point right =
        pointAddition(
            P1,
            P2,
            a,
            p
        );


    // =================================================
    // OUTPUT
    // =================================================

    cout << "ECC Curve:\n";

    cout << "y^2 = x^3 + "
         << a
         << "x + "
         << b
         << " mod "
         << p
         << "\n\n";


    printPoint(
        "Base Point G",
        G
    );


    cout << "\n";


    cout << "m1 = "
         << m1
         << "\n";


    cout << "m2 = "
         << m2
         << "\n\n";


    printPoint(
        "m1G",
        P1
    );


    printPoint(
        "m2G",
        P2
    );


    cout << "\n";


    // LEFT

    printPoint(
        "(m1 + m2)G",
        left
    );


    // RIGHT

    printPoint(
        "m1G + m2G",
        right
    );


    cout << "\n";


    // =================================================
    // CHECK HOMOMORPHIC PROPERTY
    // =================================================

    if (left.x == right.x &&
        left.y == right.y &&
        left.infinity == right.infinity)
    {
        cout << "Homomorphic Property VERIFIED\n";
    }
    else
    {
        cout << "Homomorphic Property FAILED\n";
    }


    return 0;
}