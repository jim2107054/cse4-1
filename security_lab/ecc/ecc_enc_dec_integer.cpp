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
// ECC POINT
// ======================================================
struct Point
{
    long long x;
    long long y;
    bool infinity;
};


// ======================================================
// ECC POINT ADDITION
// y^2 = x^3 + ax + b mod p
// ======================================================
Point pointAddition(Point P, Point Q, long long a, long long p)
{
    if (P.infinity)
        return Q;

    if (Q.infinity)
        return P;

    if (P.x == Q.x && mymod(P.y + Q.y, p) == 0)
        return {0, 0, true};

    long long lambda;

    // Point Doubling
    if (P.x == Q.x && P.y == Q.y)
    {
        long long numerator = mymod(3 * P.x * P.x + a, p);
        long long denominator = mymod(2 * P.y, p);
        long long inverse = ModularInverse(denominator, p);
        lambda = mymod(numerator * inverse, p);
    }
    // Normal Addition
    else
    {
        long long numerator = mymod(Q.y - P.y, p);
        long long denominator = mymod(Q.x - P.x, p);
        long long inverse = ModularInverse(denominator, p);
        lambda = mymod(numerator * inverse, p);
    }

    long long x3 = mymod(lambda * lambda - P.x - Q.x, p);
    long long y3 = mymod(lambda * (P.x - x3) - P.y, p);

    return {x3, y3, false};
}


// ======================================================
// ECC SCALAR MULTIPLICATION
// kP = P + P + ... (k times)
// ======================================================
Point scalarMultiplication(long long k, Point P, long long a, long long p)
{
    Point result = {0, 0, true};
    Point current = P;

    while (k > 0)
    {
        if (k % 2 == 1)
            result = pointAddition(result, current, a, p);

        current = pointAddition(current, current, a, p);
        k = k / 2;
    }

    return result;
}


// ======================================================
// ECC ENCRYPTION (Integer Message m -> Point M = m*G)
// C1 = k * G
// C2 = (m * G) + (k * Q)
// ======================================================
void encryptInteger(long long m,
                    long long k,
                    Point G,
                    Point Q,
                    long long a,
                    long long p,
                    Point &C1,
                    Point &C2)
{
    // Encode integer message m into Curve Point: M = m * G
    Point M = scalarMultiplication(m, G, a, p);

    // C1 = k * G
    C1 = scalarMultiplication(k, G, a, p);

    // k * Q
    Point kQ = scalarMultiplication(k, Q, a, p);

    // C2 = M + k * Q
    C2 = pointAddition(M, kQ, a, p);
}


// ======================================================
// ECC DECRYPTION (Recovers Point M, then finds integer m)
// M = C2 - d * C1
// Then search for m such that m * G == M
// ======================================================
long long decryptInteger(Point C1,
                         Point C2,
                         long long d,
                         Point G,
                         long long a,
                         long long p,
                         long long maxSearch = 1000)
{
    // d * C1
    Point dC1 = scalarMultiplication(d, C1, a, p);

    // -dC1 = (x, -y mod p)
    Point neg_dC1 = {dC1.x, mymod(-dC1.y, p), dC1.infinity};

    // Recover Point M = C2 + (-dC1)
    Point recoveredPoint = pointAddition(C2, neg_dC1, a, p);

    // Discrete log lookup: find integer m where m * G == recoveredPoint
    for (long long m = 0; m <= maxSearch; m++)
    {
        Point candidate = scalarMultiplication(m, G, a, p);

        if (candidate.infinity && recoveredPoint.infinity)
            return m;

        if (!candidate.infinity && !recoveredPoint.infinity &&
            candidate.x == recoveredPoint.x &&
            candidate.y == recoveredPoint.y)
        {
            return m;
        }
    }

    return -1; // Not found
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    // ================================================
    // ECC DOMAIN PARAMETERS
    // Curve: y^2 = x^3 + 2x + 2 mod 17
    // ================================================
    long long p = 17;
    long long a = 2;
    long long b = 2;

    // Base Point G
    Point G = {5, 1, false};

    // Private Key d
    long long d = 7;

    // Public Key Q = d * G
    Point Q = scalarMultiplication(d, G, a, p);


    // ================================================
    // INPUT INTEGER MESSAGE
    // ================================================
    long long message_int;
    cout << "Enter integer message M (e.g. 1 to 15): ";
    cin >> message_int;

    // Random integer k for encryption
    long long k = 3;


    // ================================================
    // DISPLAY SYSTEM PARAMETERS
    // ================================================
    cout << "\n========== ECC PARAMETERS ==========\n";
    cout << "Curve       : y^2 = x^3 + " << a << "x + " << b << " mod " << p << "\n";
    cout << "Base Point G: (" << G.x << ", " << G.y << ")\n";
    cout << "Private Key : d = " << d << "\n";
    cout << "Public Key  : Q = (" << Q.x << ", " << Q.y << ")\n";
    cout << "Random k    : " << k << "\n";


    // ================================================
    // ENCRYPTION
    // ================================================
    Point C1, C2;
    encryptInteger(message_int, k, G, Q, a, p, C1, C2);

    Point embeddedPoint = scalarMultiplication(message_int, G, a, p);

    cout << "\n========== ENCRYPTION ==========\n";
    cout << "Original Integer Message M = " << message_int << "\n";
    cout << "Encoded Point (M = m*G)    = (" << embeddedPoint.x << ", " << embeddedPoint.y << ")\n";
    cout << "Ciphertext C1              = (" << C1.x << ", " << C1.y << ")\n";
    cout << "Ciphertext C2              = (" << C2.x << ", " << C2.y << ")\n";


    // ================================================
    // DECRYPTION
    // ================================================
    long long decrypted_int = decryptInteger(C1, C2, d, G, a, p);

    cout << "\n========== DECRYPTION ==========\n";
    cout << "Decrypted Integer Message  = " << decrypted_int << "\n";


    // ================================================
    // CHECK
    // ================================================
    if (decrypted_int == message_int)
        cout << "\nSUCCESSFUL\n";
    else
        cout << "\nFAILED\n";

    return 0;
}
