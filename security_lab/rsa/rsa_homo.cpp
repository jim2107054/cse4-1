#include <iostream>
using namespace std;


// ======================================================
// CUSTOM MOD
// Handles both positive and negative values
// ======================================================
long long mymod(long long a, long long b)
{
    long long r = a - (a / b) * b;

    if (r < 0)
        r = r + b;

    return r;
}


// ======================================================
// CUSTOM GCD
// ======================================================
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


// ======================================================
// EXTENDED GCD
// ======================================================
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
        extendedGCD(b, mymod(a, b), x1, y1);

    x = y1;

    y = x1 - (a / b) * y1;

    return g;
}


// ======================================================
// MODULAR INVERSE
// Finds d such that:
// (e * d) mod phi = 1
// ======================================================
long long ModularInverse(long long e, long long phi)
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


// ======================================================
// MODULAR POWER
// Calculates (base^power) mod n
// ======================================================
long long modPower(long long base,
                   long long power,
                   long long n)
{
    long long result = 1;

    base = mymod(base, n);

    while (power > 0)
    {
        // Check whether power is odd
        if (mymod(power, 2) == 1)
        {
            result = mymod(result * base, n);
        }

        base = mymod(base * base, n);

        power = power / 2;
    }

    return result;
}


// ======================================================
// RSA ENCRYPTION
// C = M^e mod n
// ======================================================
long long encrypt(long long M,
                  long long e,
                  long long n)
{
    return modPower(M, e, n);
}


// ======================================================
// RSA DECRYPTION
// M = C^d mod n
// ======================================================
long long decrypt(long long C,
                  long long d,
                  long long n)
{
    return modPower(C, d, n);
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    long long p, q, m1, m2;

    // ================================================
    // INPUT
    // ================================================
    cout << "Enter p: ";
    cin >> p;

    cout << "Enter q: ";
    cin >> q;

    cout << "Enter m1: ";
    cin >> m1;

    cout << "Enter m2: ";
    cin >> m2;


    // ================================================
    // RSA KEY GENERATION
    // ================================================

    // n = p * q
    long long n = p * q;

    // phi(n) = (p-1)(q-1)
    long long phi = (p - 1) * (q - 1);


    // Choose e
    long long e = 2;

    while (e < phi)
    {
        if (mygcd(e, phi) == 1)
            break;

        e++;
    }


    // Calculate d
    // e*d ≡ 1 mod phi
    long long d =
        ModularInverse(e, phi);


    // ================================================
    // DISPLAY KEYS
    // ================================================
    cout << "\n========== RSA PARAMETERS ==========\n";

    cout << "n   = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "e   = " << e << endl;
    cout << "d   = " << d << endl;

    cout << "Public Key  = ("
         << e << ", "
         << n << ")\n";

    cout << "Private Key = ("
         << d << ", "
         << n << ")\n";


    // ================================================
    // ENCRYPTION
    // ================================================
    long long c1 = encrypt(m1, e, n);
    long long c2 = encrypt(m2, e, n);

    cout << "\n========== ENCRYPTION ==========\n";
    cout << "m1 = " << m1 << endl;
    cout << "m2 = " << m2 << endl;
    cout << "c1 = m1^e mod n = " << c1 << endl;
    cout << "c2 = m2^e mod n = " << c2 << endl;


    // ================================================
    // HOMOMORPHIC MULTIPLICATIVE PROPERTY
    // c = (c1 * c2) mod n
    // ================================================
    long long c = mymod(c1 * c2, n);

    cout << "\n========== HOMOMORPHIC MULTIPLICATION ==========\n";
    cout << "c = (c1 * c2) mod n = " << c << endl;


    // ================================================
    // DECRYPTION OF PRODUCT CIPHERTEXT
    // M = c^d mod n
    // ================================================
    long long M = decrypt(c, d, n);

    cout << "\n========== DECRYPTION ==========\n";
    cout << "M = c^d mod n = " << M << endl;


    // ================================================
    // EXPECTED RESULT
    // expected = (m1 * m2) mod n
    // ================================================
    long long expected = mymod(m1 * m2, n);

    cout << "Expected (m1 * m2 mod n) = " << expected << endl;


    // ================================================
    // CHECK
    // ================================================
    if (M == expected)
    {
        cout << "\nMultiplicative Homomorphic Property PROVED (SUCCESSFUL)!\n";
    }
    else
    {
        cout << "\nProperty NOT satisfied (FAILED).\n";
    }

    return 0;
}
