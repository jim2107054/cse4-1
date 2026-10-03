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
    // ================================================
    // INPUT
    // ================================================
    long long p, q, M;

    cout << "Enter p: ";
    cin >> p;

    cout << "Enter q: ";
    cin >> q;

    cout << "Enter message M: ";
    cin >> M;


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
    long long C =
        encrypt(M, e, n);


    cout << "\n========== ENCRYPTION ==========\n";

    cout << "Message = "
         << M << endl;

    cout << "Ciphertext = "
         << C << endl;


    // ================================================
    // DECRYPTION
    // ================================================
    long long decrypted =
        decrypt(C, d, n);


    cout << "\n========== DECRYPTION ==========\n";

    cout << "Decrypted Message = "
         << decrypted << endl;


    // ================================================
    // CHECK
    // ================================================
    if (decrypted == M)
        cout << "\nSUCCESSFUL\n";
    else
        cout << "\nFAILED\n";


    return 0;
}
