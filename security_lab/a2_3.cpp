#include <iostream>
using namespace std;

// ======================================================
// CUSTOM MOD FUNCTION
// ======================================================
long long mod(long long a, long long m)
{
    while (a >= m)
        a = a - m;

    while (a < 0)
        a = a + m;

    return a;
}

// ======================================================
// CUSTOM GCD FUNCTION
// ======================================================
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long r = mod(a, b);
        a = b;
        b = r;
    }

    return a;
}

// ======================================================
// CUSTOM MODULAR POWER
// Calculates (base^exp) mod m
// ======================================================
long long modPower(long long base, long long exp, long long m)
{
    long long result = 1;

    base = mod(base, m);

    while (exp > 0)
    {
        if (mod(exp, 2) == 1)
        {
            result = mod(result * base, m);
        }

        base = mod(base * base, m);

        exp = exp / 2;
    }

    return result;
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

    long long g = extendedGCD(b, mod(a, b), x1, y1);

    x = y1;

    y = x1 - (a / b) * y1;

    return g;
}

// ======================================================
// CUSTOM MODULAR INVERSE
// Finds d such that (e*d) mod phi = 1
// ======================================================
long long modInverse(long long e, long long phi)
{
    long long x, y;

    extendedGCD(e, phi, x, y);

    x = mod(x, phi);

    return x;
}

// ======================================================
// AUTOMATICALLY CALCULATE e
// ======================================================
long long calculateE(long long phi)
{
    long long e = 2;

    while (e < phi)
    {
        if (gcd(e, phi) == 1)
            return e;

        e++;
    }

    return -1;
}

// ======================================================
// MAIN
// ======================================================
int main()
{
    long long p, q;

    cout << "Enter p: ";
    cin >> p;

    cout << "Enter q: ";
    cin >> q;

    // Calculate n
    long long n = p * q;

    // Calculate phi(n)
    long long phi = (p - 1) * (q - 1);

    // Automatically calculate e
    long long e = calculateE(phi);

    // Calculate d
    long long d = modInverse(e, phi);

    cout << "\n========== RSA PARAMETERS ==========\n";

    cout << "n   = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "e   = " << e << endl;
    cout << "d   = " << d << endl;

    cout << "\nPublic Key  = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;


    // ==================================================
    // SIGNATURE
    // ==================================================

    long long m;

    cout << "\nEnter message: ";
    cin >> m;

    // S = m^d mod n
    long long signature = modPower(m, d, n);

    cout << "\nSignature = " << signature << endl;


    // ==================================================
    // VERIFICATION
    // ==================================================

    // M = S^e mod n
    long long recoveredMessage =
        modPower(signature, e, n);

    cout << "Recovered Message = "
         << recoveredMessage << endl;


    if (recoveredMessage == m)
        cout << "\nSignature VALID" << endl;
    else
        cout << "\nSignature INVALID" << endl;


    return 0;
}