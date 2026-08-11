#include <iostream>
using namespace std;

// =====================================================
// CUSTOM MOD FUNCTION
// =====================================================
long long mod(long long a, long long m)
{
    while (a >= m)
        a = a - m;

    while (a < 0)
        a = a + m;

    return a;
}

// =====================================================
// CUSTOM GCD
// =====================================================
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long r = mod(a, b);
        a = b;
        b = r;
    }

    return a;
}

// =====================================================
// CUSTOM MODULAR POWER
// Calculates (base^exp) mod m
// =====================================================
long long modPower(long long base, long long exp, long long m)
{
    long long result = 1;

    base = mod(base, m);

    while (exp > 0)
    {
        if (mod(exp, 2) == 1)
        {
            result = mod(result * base, m);
        }

        base = mod(base * base, m);

        exp = exp / 2;
    }

    return result;
}

// =====================================================
// EXTENDED EUCLIDEAN ALGORITHM
// =====================================================
long long extendedGCD(long long a, long long b,
                      long long &x, long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;

        return a;
    }

    long long x1, y1;

    long long g =
        extendedGCD(b, mod(a, b), x1, y1);

    x = y1;

    y = x1 - (a / b) * y1;

    return g;
}

// =====================================================
// MODULAR INVERSE
// Finds d such that:
// (e * d) mod phi = 1
// =====================================================
long long modInverse(long long e, long long phi)
{
    long long x, y;

    extendedGCD(e, phi, x, y);

    return mod(x, phi);
}

// =====================================================
// AUTOMATICALLY CALCULATE e
// =====================================================
long long calculateE(long long phi)
{
    long long e = 2;

    while (e < phi)
    {
        if (gcd(e, phi) == 1)
            return e;

        e++;
    }

    return -1;
}

// =====================================================
// MAIN
// =====================================================
int main()
{
    long long p, q, m1, m2;

    // =================================================
    // USER INPUT
    // =================================================
    cout << "Enter p: ";
    cin >> p;

    cout << "Enter q: ";
    cin >> q;

    cout << "Enter m1: ";
    cin >> m1;

    cout << "Enter m2: ";
    cin >> m2;


    // =================================================
    // RSA KEY GENERATION
    // =================================================

    // n = p * q
    long long n = p * q;

    // phi(n)
    long long phi = (p - 1) * (q - 1);

    // Automatically calculate e
    long long e = calculateE(phi);

    // Calculate d
    long long d = modInverse(e, phi);


    cout << "\n========== RSA PARAMETERS ==========\n";

    cout << "n   = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "e   = " << e << endl;
    cout << "d   = " << d << endl;

    cout << "Public Key  = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;


    // =================================================
    // ENCRYPTION
    // =================================================

    // c1 = m1^e mod n
    long long c1 = modPower(m1, e, n);

    // c2 = m2^e mod n
    long long c2 = modPower(m2, e, n);

    cout << "\n========== ENCRYPTION ==========\n";

    cout << "m1 = " << m1 << endl;
    cout << "m2 = " << m2 << endl;

    cout << "c1 = m1^e mod n = " << c1 << endl;
    cout << "c2 = m2^e mod n = " << c2 << endl;


    // =================================================
    // MULTIPLICATIVE PROPERTY
    // =================================================

    // c = c1 * c2 mod n
    long long c = mod(c1 * c2, n);

    cout << "\nc = c1 * c2 mod n = "
         << c << endl;


    // =================================================
    // DECRYPTION
    // =================================================

    // M = c^d mod n
    long long M = modPower(c, d, n);

    cout << "\n========== DECRYPTION ==========\n";

    cout << "M = c^d mod n = "
         << M << endl;


    // =================================================
    // EXPECTED RESULT
    // M = m1 * m2 mod n
    // =================================================

    long long expected =
        mod(m1 * m2, n);

    cout << "\nm1 * m2 mod n = "
         << expected << endl;


    // =================================================
    // PROOF
    // =================================================

    if (M == expected)
    {
        cout << "\nMultiplicative Property PROVED!" << endl;
        cout << "M = m1 * m2 mod n" << endl;
    }
    else
    {
        cout << "\nProperty NOT satisfied." << endl;
    }


    return 0;
}