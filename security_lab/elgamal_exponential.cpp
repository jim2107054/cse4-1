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
// EXPONENTIAL ELGAMAL ENCRYPTION
// c1 = alpha^k mod p
// c2 = (alpha^m * beta^k) mod p
// ======================================================
void encryptExponential(long long m,
                        long long k,
                        long long p,
                        long long alpha,
                        long long beta,
                        long long &c1,
                        long long &c2)
{
    // c1 = alpha^k mod p
    c1 = modPower(alpha, k, p);

    // alpha^m mod p
    long long alpha_m = modPower(alpha, m, p);

    // beta^k mod p
    long long beta_k = modPower(beta, k, p);

    // c2 = (alpha^m * beta^k) mod p
    c2 = mymod(alpha_m * beta_k, p);
}


// ======================================================
// EXPONENTIAL ELGAMAL DECRYPTION
// V = (c2 * (c1^a)^(-1)) mod p = alpha^m mod p
// Then find m by Discrete Logarithm (search m where alpha^m == V)
// ======================================================
long long decryptExponential(long long c1,
                             long long c2,
                             long long p,
                             long long alpha,
                             long long a)
{
    // s = c1^a mod p
    long long s = modPower(c1, a, p);

    // s_inv = s^(-1) mod p
    long long s_inv = ModularInverse(s, p);

    // V = c2 * s_inv mod p = alpha^m mod p
    long long V = mymod(c2 * s_inv, p);

    // Discrete log brute force to find original m (for small m)
    for (long long m = 0; m < p; m++)
    {
        if (modPower(alpha, m, p) == V)
            return m;
    }

    return -1; // Not found
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    // ================================================
    // PARAMETERS & KEY GENERATION
    // ================================================
    long long p = 467;
    long long alpha = 2;
    long long a = 127; // private key

    // beta = alpha^a mod p
    long long beta = modPower(alpha, a, p); // public key

    cout << "========== EXPONENTIAL ELGAMAL (ADDITIVE HOMOMORPHIC) ==========\n";
    cout << "Prime p          = " << p << endl;
    cout << "Generator alpha  = " << alpha << endl;
    cout << "Private Key a    = " << a << endl;
    cout << "Public Key beta  = " << beta << endl;
    cout << "Public Key       = (" << p << ", " << alpha << ", " << beta << ")\n";


    // ================================================
    // INPUT MESSAGES
    // ================================================
    long long m1, m2;
    cout << "\nEnter message m1 (integer): ";
    cin >> m1;

    cout << "Enter message m2 (integer): ";
    cin >> m2;

    long long k1 = 5;
    long long k2 = 7;


    // ================================================
    // ENCRYPTION
    // ================================================
    long long c1_1, c2_1;
    encryptExponential(m1, k1, p, alpha, beta, c1_1, c2_1);

    long long c1_2, c2_2;
    encryptExponential(m2, k2, p, alpha, beta, c1_2, c2_2);

    cout << "\n========== ENCRYPTION ==========\n";
    cout << "Ciphertext 1 (c1, c2) = (" << c1_1 << ", " << c2_1 << ")\n";
    cout << "Ciphertext 2 (c1, c2) = (" << c1_2 << ", " << c2_2 << ")\n";


    // ================================================
    // ADDITIVE HOMOMORPHIC OPERATION
    // C_sum = (c1_1 * c1_2 mod p, c2_1 * c2_2 mod p)
    // ================================================
    long long c1_sum = mymod(c1_1 * c1_2, p);
    long long c2_sum = mymod(c2_1 * c2_2, p);

    cout << "\n========== ADDITIVE HOMOMORPHIC MULTIPLICATION ==========\n";
    cout << "C_sum = (" << c1_sum << ", " << c2_sum << ")\n";


    // ================================================
    // DECRYPTION
    // ================================================
    long long decrypted_m1 = decryptExponential(c1_1, c2_1, p, alpha, a);
    long long decrypted_m2 = decryptExponential(c1_2, c2_2, p, alpha, a);
    long long decrypted_sum = decryptExponential(c1_sum, c2_sum, p, alpha, a);

    cout << "\n========== DECRYPTION ==========\n";
    cout << "Decrypted m1 = " << decrypted_m1 << endl;
    cout << "Decrypted m2 = " << decrypted_m2 << endl;
    cout << "Decrypted Sum (m1 + m2) = " << decrypted_sum << endl;

    long long expected_sum = m1 + m2;
    cout << "Expected Sum (m1 + m2)  = " << expected_sum << endl;


    // ================================================
    // CHECK
    // ================================================
    if (decrypted_sum == expected_sum)
    {
        cout << "\nAdditive Homomorphic Property PROVED (SUCCESSFUL)!\n";
    }
    else
    {
        cout << "\nProperty NOT satisfied (FAILED).\n";
    }

    return 0;
}
