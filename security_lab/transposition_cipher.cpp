#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const string CHARSET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Convert a string to 8-bit binary string
string toBinary(string s)
{
    string bin = "";
    for (char c : s)
    {
        for (int i = 7; i >= 0; i--)
            bin += ((c >> i) & 1) ? '1' : '0';
    }
    return bin;
}

// Convert 8-bit binary string back to text
string toText(string bin)
{
    string text = "";
    for (size_t i = 0; i < bin.length(); i += 8)
    {
        char c = 0;
        for (int j = 0; j < 8; j++)
            c = (c << 1) | (bin[i + j] - '0');
        text += c;
    }
    return text;
}

// Bitwise XOR of two binary strings
string xorBits(string a, string b)
{
    string res = "";
    for (size_t i = 0; i < a.length(); i++)
        res += (a[i] == b[i]) ? '0' : '1';
    return res;
}

// Print 2x2 string matrix
void printMatrix(string m[2][2])
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
    srand(time(0));

    // 0. Initial Plaintext Matrix
    string pt[2][2] = {
        {"CSE",  "21"},
        {"KUET", "Khulna"}
    };
    cout << "Original Matrix:\n";
    printMatrix(pt);

    // 1. Matrix Transpose
    string trans[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            trans[c][r] = pt[r][c];

    cout << "\nTransposed Matrix:\n";
    printMatrix(trans);

    // 2. String to Binary Matrix
    string ptBin[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            ptBin[r][c] = toBinary(trans[r][c]);

    // 3. Random Key Matrix & Binary
    string key[2][2], keyBin[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            key[r][c] = "";
            for (size_t i = 0; i < trans[r][c].length(); i++)
                key[r][c] += CHARSET[rand() % CHARSET.length()];

            keyBin[r][c] = toBinary(key[r][c]);
        }
    }
    cout << "\nKey Matrix:\n";
    printMatrix(key);

    // 4. Encryption (Transposed XOR Key)
    string cipherBin[2][2];
    cout << "\nCiphertext (Binary):\n";
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            cipherBin[r][c] = xorBits(ptBin[r][c], keyBin[r][c]);
            cout << "[" << r << "][" << c << "]: " << cipherBin[r][c] << "\n";
        }
    }

    // 5. Decryption (Cipher XOR Key)
    string decTrans[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            decTrans[r][c] = toText(xorBits(cipherBin[r][c], keyBin[r][c]));

    // 6. Inverse Transpose
    string recovered[2][2];
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            recovered[c][r] = decTrans[r][c];

    cout << "\nDecrypted Matrix:\n";
    printMatrix(recovered);

    return 0;
}
