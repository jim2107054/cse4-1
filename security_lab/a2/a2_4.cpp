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
// ELGAMAL ENCRYPTION
// c1 = alpha^k mod p
// c2 = M * beta^k mod p
// ======================================================
void encrypt(long long M,
             long long k,
             long long p,
             long long alpha,
             long long beta,
             long long &c1,
             long long &c2)
{
    // c1 = alpha^k mod p
    c1 = modPower(alpha, k, p);

    // beta^k mod p
    long long temp =
        modPower(beta, k, p);

    // c2 = M * beta^k mod p
    c2 = mymod(M * temp, p);
}


// ======================================================
// ELGAMAL DECRYPTION
// M = c2 * (c1^a)^(-1) mod p
// ======================================================
long long decrypt(long long c1,
                  long long c2,
                  long long p,
                  long long a)
{
    // s = c1^a mod p
    long long s =
        modPower(c1, a, p);

    // s^(-1) mod p
    long long s_inv =
        ModularInverse(s, p);

    // M = c2 * s^(-1) mod p
    return mymod(c2 * s_inv, p);
}


int main()
{
    // Key Generation
    long long p = 467;
    long long alpha = 2;
    long long a = 127;

    long long beta = modPower(alpha, a, p);

    // Messages and random values
    long long M1 = 10;
    long long M2 = 20;

    long long k1 = 5;
    long long k2 = 7;

    // Encrypt M1
    long long c11, c12;

    encrypt(M1, k1, p, alpha, beta, c11, c12);

    // Encrypt M2
    long long c21, c22;

    encrypt(M2, k2, p, alpha, beta, c21, c22);

    // Homomorphic Multiplication
    long long combined_c1 = mymod(c11 * c21, p);
    long long combined_c2 = mymod(c12 * c22, p);

    // Decrypt combined ciphertext
    long long result =
        decrypt(combined_c1, combined_c2, p, a);

    // Expected result
    long long expected =
        mymod(M1 * M2, p);

    // Output
    cout << "Public Key = ("
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Ciphertext 1 = ("
         << c11 << ", "
         << c12 << ")\n";

    cout << "Ciphertext 2 = ("
         << c21 << ", "
         << c22 << ")\n";

    cout << "Combined Ciphertext = ("
         << combined_c1 << ", "
         << combined_c2 << ")\n";

    cout << "Decrypted Result = "
         << result << "\n";

    cout << "Expected Result = "
         << expected << "\n";

    if (result == expected)
        cout << "SUCCESSFUL\n";
    else
        cout << "FAILED\n";

    return 0;
}
