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
// ELGAMAL ENCRYPTION
// c1 = alpha^k mod p
// c2 = (M * beta^k) mod p
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
// s_inv = s^(-1) mod p
// M = (c2 * s_inv) mod p
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
// MAIN
// ======================================================
int main()
{
    // ================================================
    // INPUT / KEY PARAMETERS
    // ================================================
    long long p, alpha, a, M, k;

    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter generator alpha: ";
    cin >> alpha;

    cout << "Enter private key a: ";
    cin >> a;

    cout << "Enter message M: ";
    cin >> M;

    cout << "Enter random key k: ";
    cin >> k;


    // ================================================
    // KEY GENERATION
    // beta = alpha^a mod p
    // ================================================
    long long beta = modPower(alpha, a, p);


    // ================================================
    // DISPLAY KEYS
    // ================================================
    cout << "\n========== ELGAMAL PARAMETERS ==========\n";
    cout << "Prime p          = " << p << endl;
    cout << "Generator alpha  = " << alpha << endl;
    cout << "Private Key a    = " << a << endl;
    cout << "Public Key beta  = " << beta << endl;
    cout << "Public Key       = (" << p << ", " << alpha << ", " << beta << ")\n";


    // ================================================
    // ENCRYPTION
    // ================================================
    long long c1, c2;
    encrypt(M, k, p, alpha, beta, c1, c2);

    cout << "\n========== ENCRYPTION ==========\n";
    cout << "Message M = " << M << endl;
    cout << "Ciphertext C1 = " << c1 << endl;
    cout << "Ciphertext C2 = " << c2 << endl;
    cout << "Ciphertext (C1, C2) = (" << c1 << ", " << c2 << ")\n";


    // ================================================
    // DECRYPTION
    // ================================================
    long long decrypted = decrypt(c1, c2, p, a);

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
