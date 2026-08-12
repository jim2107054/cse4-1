#include <iostream>
using namespace std;


// ======================================================
// CUSTOM MOD
// ======================================================
long long mymod(long long a, long long b)
{
    return a - (a / b) * b;
}


// ======================================================
// CUSTOM GCD
// ======================================================
long long mygcd(long long a, long long b)
{
    if (a < b)
        swap(a, b);

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
// CALCULATE e AUTOMATICALLY
// ======================================================
long long calculateE(long long phi)
{
    long long e = 2;

    while (e < phi)
    {
        if (mygcd(e, phi) == 1)
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
    long long p1, q1, m1;
    long long p2, q2, m2;


    // ==================================================
    // USER INPUT
    // ==================================================

    cout << "===== USER 1 =====\n";

    cout << "Enter p1: ";
    cin >> p1;

    cout << "Enter q1: ";
    cin >> q1;

    cout << "Enter message m1: ";
    cin >> m1;


    cout << "\n===== USER 2 =====\n";

    cout << "Enter p2: ";
    cin >> p2;

    cout << "Enter q2: ";
    cin >> q2;

    cout << "Enter message m2: ";
    cin >> m2;


    // ==================================================
    // USER 1 KEY GENERATION
    // ==================================================

    long long n1 = p1 * q1;

    long long phi1 =
        (p1 - 1) * (q1 - 1);

    long long e1 =
        calculateE(phi1);

    long long d1 =
        ModularInverse(e1, phi1);


    // ==================================================
    // USER 2 KEY GENERATION
    // ==================================================

    long long n2 = p2 * q2;

    long long phi2 =
        (p2 - 1) * (q2 - 1);

    long long e2 =
        calculateE(phi2);

    long long d2 =
        ModularInverse(e2, phi2);


    // ==================================================
    // DISPLAY KEYS
    // ==================================================

    cout << "\n========================================";
    cout << "\n           USER 1 KEY";
    cout << "\n========================================\n";

    cout << "n1   = " << n1 << endl;
    cout << "phi1 = " << phi1 << endl;
    cout << "e1   = " << e1 << endl;
    cout << "d1   = " << d1 << endl;

    cout << "Public Key 1  = ("
         << e1 << ", " << n1 << ")" << endl;

    cout << "Private Key 1 = ("
         << d1 << ", " << n1 << ")" << endl;


    cout << "\n========================================";
    cout << "\n           USER 2 KEY";
    cout << "\n========================================\n";

    cout << "n2   = " << n2 << endl;
    cout << "phi2 = " << phi2 << endl;
    cout << "e2   = " << e2 << endl;
    cout << "d2   = " << d2 << endl;

    cout << "Public Key 2  = ("
         << e2 << ", " << n2 << ")" << endl;

    cout << "Private Key 2 = ("
         << d2 << ", " << n2 << ")" << endl;


    // ==================================================
    // USER 1 SIGNATURE
    // ==================================================

    long long signature1 =
        modPower(m1, d1, n1);


    // ==================================================
    // USER 2 SIGNATURE
    // ==================================================

    long long signature2 =
        modPower(m2, d2, n2);


    cout << "\n========================================";
    cout << "\n           SIGNATURES";
    cout << "\n========================================\n";

    cout << "User 1 Message   = " << m1 << endl;
    cout << "User 1 Signature = " << signature1 << endl;

    cout << "\nUser 2 Message   = " << m2 << endl;
    cout << "User 2 Signature = " << signature2 << endl;


    // ==================================================
    // USER 1 VERIFICATION
    // ==================================================

    long long recovered1 =
        modPower(signature1, e1, n1);


    // ==================================================
    // USER 2 VERIFICATION
    // ==================================================

    long long recovered2 =
        modPower(signature2, e2, n2);


    cout << "\n========================================";
    cout << "\n           VERIFICATION";
    cout << "\n========================================\n";

    cout << "User 1 Recovered Message = "
         << recovered1 << endl;

    if (recovered1 == m1)
        cout << "User 1 Signature VALID\n";
    else
        cout << "User 1 Signature INVALID\n";


    cout << "\nUser 2 Recovered Message = "
         << recovered2 << endl;

    if (recovered2 == m2)
        cout << "User 2 Signature VALID\n";
    else
        cout << "User 2 Signature INVALID\n";


    return 0;
}