#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <random>
#include <ctime>
#include <iomanip>

using namespace std;

// Alphanumeric character set: {0-9, A-Z, a-z} (62 characters)
const string ALPHANUMERIC_SET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// ======================================================
// HELPER FUNCTIONS
// ======================================================

// Convert a single string into a vector of 8-bit ASCII binary strings (one per character)
vector<string> stringToBinaryVector(const string &str)
{
    vector<string> binVec;
    for (unsigned char c : str)
    {
        binVec.push_back(bitset<8>(c).to_string());
    }
    return binVec;
}

// Convert a vector of 8-bit binary strings back to a string
string binaryVectorToString(const vector<string> &binVec)
{
    string result = "";
    for (const string &bin : binVec)
    {
        unsigned long val = bitset<8>(bin).to_ulong();
        result += static_cast<char>(val);
    }
    return result;
}

// Bitwise XOR of two equal-length binary strings ('0' and '1')
string xorBinaryStrings(const string &bin1, const string &bin2)
{
    string result = "";
    for (size_t i = 0; i < bin1.length(); i++)
    {
        char b1 = bin1[i];
        char b2 = bin2[i];
        result += (b1 == b2) ? '0' : '1';
    }
    return result;
}

// Generate a random alphanumeric string of a given length
string generateRandomKeyString(size_t len, mt19937 &rng)
{
    uniform_int_distribution<size_t> dist(0, ALPHANUMERIC_SET.size() - 1);
    string key = "";
    for (size_t i = 0; i < len; i++)
    {
        key += ALPHANUMERIC_SET[dist(rng)];
    }
    return key;
}

// Print a 2x2 matrix of strings
void printMatrix(const string matrix[2][2], const string &title)
{
    cout << "\n--- " << title << " ---\n";
    cout << "+-----------------------+-----------------------+\n";
    for (int r = 0; r < 2; r++)
    {
        cout << "| ";
        cout << left << setw(21) << matrix[r][0] << " | ";
        cout << left << setw(21) << matrix[r][1] << " |\n";
        cout << "+-----------------------+-----------------------+\n";
    }
}

// Print a 2x2 matrix of binary representations
void printBinaryMatrix(const vector<string> binMatrix[2][2], const string &title)
{
    cout << "\n--- " << title << " ---\n";
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            cout << "Cell [" << r << "][" << c << "]: ";
            for (size_t i = 0; i < binMatrix[r][c].size(); i++)
            {
                cout << binMatrix[r][c][i] << (i + 1 < binMatrix[r][c].size() ? " " : "");
            }
            cout << "\n";
        }
    }
}

// ======================================================
// MAIN EXECUTION
// ======================================================
int main()
{
    cout << "====================================================================\n";
    cout << "   LAB-1: TRANSPOSITIONAL CIPHER SYSTEM (2x2 MATRIX OF STRINGS)     \n";
    cout << "====================================================================\n";

    // Step 0: Initial Plaintext Matrix
    string initialPlaintext[2][2] = {
        {"CSE",  "21"},
        {"KUET", "Khulna"}
    };

    printMatrix(initialPlaintext, "Step 0: Initial Plaintext Matrix (2x2)");

    // Step 1: Matrix Transposition
    // Transpose of matrix[i][j] is transposed[j][i]
    string transposedMatrix[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            transposedMatrix[c][r] = initialPlaintext[r][c];
        }
    }

    printMatrix(transposedMatrix, "Step 1: Transposed Matrix (2x2)");

    // Step 2: String-to-Binary Conversion
    // Convert every character of transposed matrix strings into 8-bit ASCII binary representation
    vector<string> transposedBinary[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            transposedBinary[r][c] = stringToBinaryVector(transposedMatrix[r][c]);
        }
    }

    printBinaryMatrix(transposedBinary, "Step 2: Binary Representation of Transposed Matrix (8-bit ASCII)");

    // Step 3: Random Key Generation
    // Key matrix with same dimensions (2x2) and matching string lengths from {0-9, A-Z, a-z}
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    string keyMatrix[2][2];
    vector<string> keyBinary[2][2];

    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            size_t len = transposedMatrix[r][c].length();
            keyMatrix[r][c] = generateRandomKeyString(len, rng);
            keyBinary[r][c] = stringToBinaryVector(keyMatrix[r][c]);
        }
    }

    printMatrix(keyMatrix, "Step 3: Random Alphanumeric Key Matrix (2x2)");
    printBinaryMatrix(keyBinary, "Step 3: Binary Representation of Key Matrix (8-bit ASCII)");

    // Step 4: Encryption
    // Element-by-element XOR between transposed plaintext binary and key binary
    vector<string> cipherBinary[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            for (size_t k = 0; k < transposedBinary[r][c].size(); k++)
            {
                string cBin = xorBinaryStrings(transposedBinary[r][c][k], keyBinary[r][c][k]);
                cipherBinary[r][c].push_back(cBin);
            }
        }
    }

    printBinaryMatrix(cipherBinary, "Step 4: Ciphertext Matrix (Binary representation after XOR)");

    // Step 5: Decryption
    // XOR ciphertext binary with key binary to recover transposed plaintext binary
    vector<string> decryptedTransposedBinary[2][2];
    string decryptedTransposed[2][2];

    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            for (size_t k = 0; k < cipherBinary[r][c].size(); k++)
            {
                string pBin = xorBinaryStrings(cipherBinary[r][c][k], keyBinary[r][c][k]);
                decryptedTransposedBinary[r][c].push_back(pBin);
            }
            decryptedTransposed[r][c] = binaryVectorToString(decryptedTransposedBinary[r][c]);
        }
    }

    printBinaryMatrix(decryptedTransposedBinary, "Step 5: Decrypted Transposed Binary Matrix (Cipher XOR Key)");
    printMatrix(decryptedTransposed, "Step 5: Decrypted Transposed String Matrix");

    // Step 6: Inverse Transpose to Recover Original Plaintext Matrix
    string recoveredPlaintext[2][2];
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            recoveredPlaintext[c][r] = decryptedTransposed[r][c];
        }
    }

    printMatrix(recoveredPlaintext, "Step 6: Recovered Original Plaintext Matrix (Inverse Transpose)");

    // Step 7: Verification
    bool isMatch = true;
    for (int r = 0; r < 2; r++)
    {
        for (int c = 0; c < 2; c++)
        {
            if (recoveredPlaintext[r][c] != initialPlaintext[r][c])
            {
                isMatch = false;
            }
        }
    }

    cout << "\n====================================================================\n";
    if (isMatch)
    {
        cout << " [SUCCESS] Decryption and inverse transpose successfully recovered\n";
        cout << "           the original 2x2 plaintext matrix!\n";
    }
    else
    {
        cout << " [ERROR] Decrypted matrix does NOT match the original plaintext matrix.\n";
    }
    cout << "====================================================================\n";

    return 0;
}
