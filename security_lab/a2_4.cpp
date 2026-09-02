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


// ======================================================
// MAIN
// ======================================================
int main()
{
    // ==================================================
    // KEY GENERATION
    // ==================================================

    long long p = 467;

    // Primitive Root / Generator
    long long alpha = 2;

    // Private Key
    long long a = 127;

    // beta = alpha^a mod p
    long long beta =
        modPower(alpha, a, p);


    cout << "========================================\n";
    cout << "       ELGAMAL CRYPTOSYSTEM\n";
    cout << "========================================\n";


    cout << "\nKey Generation\n";
    cout << "---------------\n";

    cout << "p     = " << p << endl;
    cout << "alpha = " << alpha << endl;
    cout << "a     = " << a << " (Private Key)" << endl;
    cout << "beta  = " << beta << endl;


    cout << "\nPublic Key = ("
         << p << ", "
         << alpha << ", "
         << beta << ")" << endl;

    cout << "Private Key = "
         << a << endl;


    // ==================================================
    // TWO MESSAGES
    // ==================================================

    long long M1 = 10;
    long long M2 = 20;

    // Random encryption values
    long long k1 = 5;
    long long k2 = 7;


    cout << "\n========================================\n";
    cout << "             MESSAGES\n";
    cout << "========================================\n";

    cout << "M1 = " << M1 << endl;
    cout << "k1 = " << k1 << endl;

    cout << "\nM2 = " << M2 << endl;
    cout << "k2 = " << k2 << endl;


    // ==================================================
    // ENCRYPT MESSAGE 1
    // ==================================================

    long long c11, c12;

    encrypt(
        M1,
        k1,
        p,
        alpha,
        beta,
        c11,
        c12
    );


    cout << "\n========================================\n";
    cout << "             ENCRYPTION\n";
    cout << "========================================\n";

    cout << "Ciphertext 1 = ("
         << c11 << ", "
         << c12 << ")" << endl;


    // ==================================================
    // ENCRYPT MESSAGE 2
    // ==================================================

    long long c21, c22;

    encrypt(
        M2,
        k2,
        p,
        alpha,
        beta,
        c21,
        c22
    );


    cout << "Ciphertext 2 = ("
         << c21 << ", "
         << c22 << ")" << endl;


    // ==================================================
    // HOMOMORPHIC MULTIPLICATION
    // ==================================================

    // Combined c1
    long long combined_c1 =
        mymod(c11 * c21, p);

    // Combined c2
    long long combined_c2 =
        mymod(c12 * c22, p);


    cout << "\n========================================\n";
    cout << "     HOMOMORPHIC MULTIPLICATION\n";
    cout << "========================================\n";


    cout << "\nCombined Ciphertext = ("
         << combined_c1 << ", "
         << combined_c2 << ")" << endl;


    // ==================================================
    // DECRYPT COMBINED CIPHERTEXT
    // ==================================================

    long long result =
        decrypt(
            combined_c1,
            combined_c2,
            p,
            a
        );


    cout << "\nDecrypted Result = "
         << result << endl;


    // Expected result
    long long expected =
        mymod(M1 * M2, p);


    cout << "Expected M1 * M2 mod p = "
         << expected << endl;


    // ==================================================
    // VERIFY HOMOMORPHIC PROPERTY
    // ==================================================

    cout << "\n========================================\n";
    cout << "              VERIFICATION\n";
    cout << "========================================\n";


    if (result == expected)
    {
        cout << "\nHomomorphic Multiplication SUCCESSFUL"
             << endl;
    }
    else
    {
        cout << "\nHomomorphic Multiplication FAILED"
             << endl;
    }


    return 0;
}
