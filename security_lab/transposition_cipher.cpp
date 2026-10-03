#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>

using namespace std;

const string CHARSET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Manual 8-bit binary representation of a character
string charToBinary(unsigned char c)
{
    string bin = "";
    for (int i = 7; i >= 0; i--)
        bin += ((c >> i) & 1) ? '1' : '0';
    return bin;
}

// Manual binary string to character
char binaryToChar(const string &bin)
{
    unsigned char c = 0;
    for (char bit : bin)
        c = (c << 1) | (bit - '0');
    return static_cast<char>(c);
}

// String to vector of 8-bit binary strings
vector<string> toBinary(const string &s)
{
    vector<string> bin;
    for (unsigned char c : s)
        bin.push_back(charToBinary(c));
    return bin;
}

// Vector of 8-bit binary strings to string
string toString(const vector<string> &bin)
{
    string s = "";
    for (const string &b : bin)
        s += binaryToChar(b);
    return s;
}

// Manual XOR for single bits ('0' or '1')
char manualXor(char a, char b)
{
    if ((a == '1' && b == '0') || (a == '0' && b == '1'))
        return '1';
    return '0';
}

// Manual Bitwise XOR of two binary strings
string xorBits(const string &a, const string &b)
{
    string res = "";
    for (size_t i = 0; i < a.length(); i++)
        res += manualXor(a[i], b[i]);
    return res;
}

// Generate random alphanumeric key of given length
string generateKey(size_t len, mt19937 &rng)
{
    uniform_int_distribution<size_t> dist(0, CHARSET.size() - 1);
    string key = "";
    for (size_t i = 0; i < len; i++)
        key += CHARSET[dist(rng)];
    return key;
}

// Print 2x2 string matrix
void printMatrix(const string m[2][2])
{
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
            cout << m[r][c] << "\t";
        cout << "\n";
    }
}

int main()
{
    mt19937 rng(time(nullptr));

    // 0. Initial Matrix
    string pt[2][2] = {
        {"CSE",  "21"},
        {"KUET", "Khulna"}
    };
    cout << "Original Matrix:\n";
    printMatrix(pt);

    // 1. Transpose
    string transposed[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            transposed[c][r] = pt[r][c];

    cout << "\nTransposed Matrix:\n";
    printMatrix(transposed);

    // 2. String to Binary
    vector<string> ptBin[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            ptBin[r][c] = toBinary(transposed[r][c]);

    // 3. Random Key Generation
    string key[2][2];
    vector<string> keyBin[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            key[r][c] = generateKey(transposed[r][c].length(), rng);
            keyBin[r][c] = toBinary(key[r][c]);
        }
    }
    cout << "\nKey Matrix:\n";
    printMatrix(key);

    // 4. Encryption (Transposed XOR Key)
    vector<string> cipherBin[2][2];
    cout << "\nCiphertext (Binary):\n";
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            cout << "[" << r << "][" << c << "]: ";
            for (size_t k = 0; k < ptBin[r][c].size(); k++)
            {
                string cBits = xorBits(ptBin[r][c][k], keyBin[r][c][k]);
                cipherBin[r][c].push_back(cBits);
                cout << cBits << " ";
            }
            cout << "\n";
        }
    }

    // 5. Decryption (Cipher XOR Key)
    string decTransposed[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            vector<string> decBin;
            for (size_t k = 0; k < cipherBin[r][c].size(); k++)
                decBin.push_back(xorBits(cipherBin[r][c][k], keyBin[r][c][k]));
            decTransposed[r][c] = toString(decBin);
        }
    }

    // 6. Inverse Transpose
    string recovered[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            recovered[c][r] = decTransposed[r][c];

    cout << "\nDecrypted Matrix:\n";
    printMatrix(recovered);

    return 0;
}
