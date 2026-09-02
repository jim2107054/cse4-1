#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


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
        extendedGCD(b, mymod(a, b), x1, y1);

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

    return x;
}


// ---------------- Modular Power ----------------

long long modPower(
    long long base,
    long long power,
    long long n)
{
    long long result = 1;

    base = mymod(base, n);

    while (power > 0)
    {
        if (mymod(power, 2) == 1)
        {
            result =
                mymod(result * base, n);
        }

        base =
            mymod(base * base, n);

        power = power / 2;
    }

    return result;
}


// ---------------- Encryption ----------------

void encrypt(
    long long M,
    long long k,
    long long p,
    long long alpha,
    long long beta,
    long long &c1,
    long long &c2)
{
    c1 = modPower(alpha, k, p);

    long long temp =
        modPower(beta, k, p);

    c2 =
        mymod(M * temp, p);
}


// ---------------- Decryption ----------------

long long decrypt(
    long long c1,
    long long c2,
    long long p,
    long long a)
{
    long long s =
        modPower(c1, a, p);

    long long s_inv =
        ModularInverse(s, p);

    return mymod(c2 * s_inv, p);
}


// ---------------- Re-randomization ----------------

void rerandomize(
    long long c1,
    long long c2,
    long long r,
    long long p,
    long long alpha,
    long long beta,
    long long &new_c1,
    long long &new_c2)
{
    new_c1 =
        mymod(
            c1 * modPower(alpha, r, p),
            p
        );

    new_c2 =
        mymod(
            c2 * modPower(beta, r, p),
            p
        );
}


// ---------------- MAIN ----------------

int main()
{
    srand(time(0));

    // Key Generation
    long long p = 467;
    long long alpha = 2;
    long long a = 127;

    long long beta =
        modPower(alpha, a, p);


    // Message
    long long M = 10;


    // Random k for encryption
    long long k = rand();


    // Encryption
    long long c1, c2;

    encrypt(
        M,
        k,
        p,
        alpha,
        beta,
        c1,
        c2
    );


    // Random r for re-randomization
    long long r = rand();


    // Re-randomization
    long long new_c1, new_c2;

    rerandomize(
        c1,
        c2,
        r,
        p,
        alpha,
        beta,
        new_c1,
        new_c2
    );


    // Decryption
    long long decrypted =
        decrypt(
            new_c1,
            new_c2,
            p,
            a
        );


    // Output
    cout << "Original Ciphertext = ("
         << c1 << ", "
         << c2 << ")\n";

    cout << "Random r = "
         << r << "\n";

    cout << "Re-randomized Ciphertext = ("
         << new_c1 << ", "
         << new_c2 << ")\n";

    cout << "Decrypted Message = "
         << decrypted << "\n";

    if (decrypted == M)
        cout << "SUCCESSFUL\n";
    else
        cout << "FAILED\n";

    return 0;
}