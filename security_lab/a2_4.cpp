#include <bits/stdc++.h>
using namespace std;

using int64 = long long;


// Custom modulo
int64 mymod(int64 a, int64 m)
{
    int64 r = a % m;

    if (r < 0)
        r += m;

    return r;
}


// Custom GCD
int64 mygcd(int64 a, int64 b)
{
    while (b != 0)
    {
        int64 temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


// Custom Modular Exponentiation
int64 mymodpow(int64 a, int64 e, int64 m)
{
    int64 result = 1;

    a = mymod(a, m);

    while (e > 0)
    {
        if (e % 2 == 1)
            result = (result * a) % m;

        a = (a * a) % m;

        e = e / 2;
    }

    return result;
}


// Extended Euclidean Algorithm
int64 myegcd(int64 a, int64 b, int64 &x, int64 &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;

        return a;
    }

    int64 x1, y1;

    int64 g = myegcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}


// Modular Inverse
int64 mymodinv(int64 a, int64 m)
{
    int64 x, y;

    int64 g = myegcd(a, m, x, y);

    if (g != 1)
        return -1;

    return mymod(x, m);
}


// --------------------------------------------------
// ElGamal Encryption
// --------------------------------------------------
void encrypt(
    int64 M,
    int64 k,
    int64 p,
    int64 alpha,
    int64 beta,
    int64 &c1,
    int64 &c2)
{
    // c1 = alpha^k mod p
    c1 = mymodpow(alpha, k, p);

    // c2 = M * beta^k mod p
    c2 = (M * mymodpow(beta, k, p)) % p;
}


// --------------------------------------------------
// ElGamal Decryption
// --------------------------------------------------
int64 decrypt(
    int64 c1,
    int64 c2,
    int64 p,
    int64 a)
{
    // s = c1^a mod p
    int64 s = mymodpow(c1, a, p);

    // s^-1 mod p
    int64 s_inv = mymodinv(s, p);

    // M = c2 * s^-1 mod p
    return (c2 * s_inv) % p;
}


int main()
{
    // ==================================================
    // KEY GENERATION
    // ==================================================

    int64 p = 467;
    int64 alpha = 2;
    int64 a = 127;

    // beta = alpha^a mod p
    int64 beta = mymodpow(alpha, a, p);


    cout << "====================================\n";
    cout << "   ELGAMAL MULTIPLICATIVE HOMOMORPHISM\n";
    cout << "====================================\n\n";


    cout << "Public Key = ("
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Private Key = "
         << a << "\n";


    // ==================================================
    // TWO MESSAGES
    // ==================================================

    int64 M1 = 10;
    int64 M2 = 20;

    int64 k1 = 5;
    int64 k2 = 7;


    cout << "\nMessage 1 = " << M1;
    cout << "\nRandom k1 = " << k1;

    cout << "\n\nMessage 2 = " << M2;
    cout << "\nRandom k2 = " << k2;


    // ==================================================
    // ENCRYPT M1
    // ==================================================

    int64 c11, c12;

    encrypt(
        M1,
        k1,
        p,
        alpha,
        beta,
        c11,
        c12
    );


    cout << "\n\nCiphertext 1 = ("
         << c11 << ", "
         << c12 << ")";


    // ==================================================
    // ENCRYPT M2
    // ==================================================

    int64 c21, c22;

    encrypt(
        M2,
        k2,
        p,
        alpha,
        beta,
        c21,
        c22
    );


    cout << "\nCiphertext 2 = ("
         << c21 << ", "
         << c22 << ")";


    // ==================================================
    // HOMOMORPHIC MULTIPLICATION
    // ==================================================

    // Multiply first components
    int64 combined_c1 =
        (c11 * c21) % p;

    // Multiply second components
    int64 combined_c2 =
        (c12 * c22) % p;


    cout << "\n\n====================================\n";
    cout << "HOMOMORPHIC MULTIPLICATION\n";
    cout << "====================================\n";


    cout << "\nCombined Ciphertext = ("
         << combined_c1 << ", "
         << combined_c2 << ")";


    // ==================================================
    // DECRYPT COMBINED CIPHERTEXT
    // ==================================================

    int64 result =
        decrypt(
            combined_c1,
            combined_c2,
            p,
            a
        );


    cout << "\n\nDecrypted Result = "
         << result;


    // Expected result
    int64 expected =
        (M1 * M2) % p;


    cout << "\nExpected M1 * M2 mod p = "
         << expected;


    // ==================================================
    // VERIFY
    // ==================================================

    if (result == expected)
    {
        cout << "\n\nHomomorphic Multiplication SUCCESSFUL";
    }
    else
    {
        cout << "\n\nHomomorphic Multiplication FAILED";
    }


    cout << "\n";

    return 0;
}