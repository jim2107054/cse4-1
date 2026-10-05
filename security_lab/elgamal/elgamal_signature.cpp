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
// ELGAMAL SIGNATURE GENERATION
// y1 = alpha^r mod p
// y2 = r^(-1) * (M - a*y1) mod (p-1)
// ======================================================
void generateSignature(long long M,
                       long long r,
                       long long p,
                       long long alpha,
                       long long a,
                       long long &y1,
                       long long &y2)
{
    // y1 = alpha^r mod p
    y1 = modPower(alpha, r, p);

    // r^(-1) mod (p - 1)
    long long r_inv = ModularInverse(r, p - 1);

    // y2 = r^(-1) * (M - a * y1) mod (p - 1)
    long long temp = M - a * y1;
    y2 = mymod(r_inv * temp, p - 1);
}


// ======================================================
// ELGAMAL SIGNATURE VERIFICATION
// Left:  alpha^M mod p
// Right: (beta^y1 * y1^y2) mod p
// ======================================================
bool verifySignature(long long M,
                     long long y1,
                     long long y2,
                     long long p,
                     long long alpha,
                     long long beta)
{
    // Left side = alpha^M mod p
    long long left = modPower(alpha, M, p);

    // beta^y1 mod p
    long long part1 = modPower(beta, y1, p);

    // y1^y2 mod p
    long long part2 = modPower(y1, y2, p);

    // Right side = (beta^y1 * y1^y2) mod p
    long long right = mymod(part1 * part2, p);

    return left == right;
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    // ================================================
    // KEY GENERATION
    // ================================================
    long long p = 467;
    long long alpha = 2;
    long long a = 127;

    long long beta = modPower(alpha, a, p);

    // ================================================
    // INPUT
    // ================================================
    long long M;
    cout << "Enter message (integer): ";
    cin >> M;

    // Random value r
    long long r = 5;

    // r must be relatively prime to (p - 1)
    // if (mygcd(r, p - 1) != 1)
    // {
    //     cout << "r is not coprime to (p - 1)\n";
    //     return 0;
    // }

    // ================================================
    // SIGNATURE GENERATION
    // ================================================
    long long y1, y2;
    generateSignature(M, r, p, alpha, a, y1, y2);

    // ================================================
    // SIGNATURE VERIFICATION
    // ================================================
    bool valid = verifySignature(M, y1, y2, p, alpha, beta);

    // ================================================
    // DISPLAY RESULTS
    // ================================================
    cout << "\n========== ELGAMAL PARAMETERS ==========\n";
    cout << "Public Key = (p=" << p << ", alpha=" << alpha << ", beta=" << beta << ")\n";
    cout << "Private Key a = " << a << "\n";
    cout << "Random r = " << r << "\n";
    cout << "Signature (y1, y2) = (" << y1 << ", " << y2 << ")\n";

    cout << "\n========== VERIFICATION ==========\n";
    if (valid)
        cout << "Signature VALID\n";
    else
        cout << "Signature INVALID\n";

    return 0;
}
