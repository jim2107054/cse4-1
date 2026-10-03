#include <iostream>
#include <cstdlib>
#include <ctime>

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
    c1 = modPower(alpha, k, p);

    long long temp = modPower(beta, k, p);

    c2 = mymod(M * temp, p);
}


// ======================================================
// ELGAMAL DECRYPTION
// s = c1^a mod p
// M = c2 * s^(-1) mod p
// ======================================================
long long decrypt(long long c1,
                  long long c2,
                  long long p,
                  long long a)
{
    long long s = modPower(c1, a, p);

    long long s_inv = ModularInverse(s, p);

    return mymod(c2 * s_inv, p);
}


// ======================================================
// ELGAMAL RE-RANDOMIZATION
// new_c1 = c1 * alpha^r mod p
// new_c2 = c2 * beta^r mod p
// ======================================================
void rerandomize(long long c1,
                 long long c2,
                 long long r,
                 long long p,
                 long long alpha,
                 long long beta,
                 long long &new_c1,
                 long long &new_c2)
{
    new_c1 = mymod(c1 * modPower(alpha, r, p), p);

    new_c2 = mymod(c2 * modPower(beta, r, p), p);
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    srand(time(0));

    // ================================================
    // KEY GENERATION
    // ================================================
    long long p = 467;
    long long alpha = 2;
    long long a = 127;

    long long beta = modPower(alpha, a, p);

    // ================================================
    // INPUT / PARAMETERS
    // ================================================
    long long M = 10;
    long long k = rand();

    // ================================================
    // ENCRYPTION
    // ================================================
    long long c1, c2;
    encrypt(M, k, p, alpha, beta, c1, c2);

    // ================================================
    // RE-RANDOMIZATION
    // ================================================
    long long r = rand();
    long long new_c1, new_c2;
    rerandomize(c1, c2, r, p, alpha, beta, new_c1, new_c2);

    // ================================================
    // DECRYPTION
    // ================================================
    long long decrypted = decrypt(new_c1, new_c2, p, a);

    // ================================================
    // DISPLAY RESULTS
    // ================================================
    cout << "\n========== ELGAMAL PARAMETERS ==========\n";
    cout << "Prime p       = " << p << endl;
    cout << "Alpha         = " << alpha << endl;
    cout << "Private Key a = " << a << endl;
    cout << "Public Key beta = " << beta << endl;

    cout << "\n========== ENCRYPTION ==========\n";
    cout << "Original Message = " << M << endl;
    cout << "Original Ciphertext = (" << c1 << ", " << c2 << ")\n";

    cout << "\n========== RE-RANDOMIZATION ==========\n";
    cout << "Random factor r = " << r << endl;
    cout << "Re-randomized Ciphertext = (" << new_c1 << ", " << new_c2 << ")\n";

    cout << "\n========== DECRYPTION ==========\n";
    cout << "Decrypted Message = " << decrypted << endl;

    // ================================================
    // CHECK
    // ================================================
    if (decrypted == M)
        cout << "\nSUCCESSFUL\n";
    else
        cout << "\nFAILED\n";

    return 0;
}
