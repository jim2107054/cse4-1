#include <iostream>
#include <string>
#include <cctype>

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
// ALPHABET ARRAY
// ======================================================
const char ALPHABET[26] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z'
};


// Helper to find index of a character in alphabet array
int getIndex(char ch)
{
    ch = tolower(ch);
    for (int i = 0; i < 26; i++)
    {
        if (ALPHABET[i] == ch)
            return i;
    }
    return -1;
}


// ======================================================
// VERNAM ENCRYPTION
// C[i] = (P[i] + K[i]) mod 26
// ======================================================
string encrypt(string plaintext, string key)
{
    string ciphertext = "";

    for (int i = 0; i < (int)plaintext.length(); i++)
    {
        char pChar = plaintext[i];
        char kChar = key[i];

        if (isalpha(pChar) && isalpha(kChar))
        {
            bool isUpper = isupper(pChar);

            int pIndex = getIndex(pChar);
            int kIndex = getIndex(kChar);

            // Add indices modulo 26 using alphabet array
            int cIndex = mymod(pIndex + kIndex, 26);
            char encChar = ALPHABET[cIndex];

            if (isUpper)
                encChar = toupper(encChar);

            ciphertext += encChar;
        }
        else
        {
            // Non-alphabet characters are preserved
            ciphertext += pChar;
        }
    }

    return ciphertext;
}


// ======================================================
// VERNAM DECRYPTION
// P[i] = (C[i] - K[i]) mod 26
// ======================================================
string decrypt(string ciphertext, string key)
{
    string plaintext = "";

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char cChar = ciphertext[i];
        char kChar = key[i];

        if (isalpha(cChar) && isalpha(kChar))
        {
            bool isUpper = isupper(cChar);

            int cIndex = getIndex(cChar);
            int kIndex = getIndex(kChar);

            // Subtract indices modulo 26 using alphabet array
            int pIndex = mymod(cIndex - kIndex, 26);
            char decChar = ALPHABET[pIndex];

            if (isUpper)
                decChar = toupper(decChar);

            plaintext += decChar;
        }
        else
        {
            // Non-alphabet characters are preserved
            plaintext += cChar;
        }
    }

    return plaintext;
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    string text, key;

    // ================================================
    // INPUT
    // ================================================
    cout << "Enter plaintext message: ";
    getline(cin, text);

    cout << "Enter key (same length as message or string): ";
    getline(cin, key);

    // Auto-pad / repeat key if shorter than plaintext
    if (key.length() < text.length())
    {
        string originalKey = key;
        while (key.length() < text.length())
        {
            key += originalKey;
        }
        key = key.substr(0, text.length());
        cout << "(Key adjusted to match length: " << key << ")\n";
    }


    // ================================================
    // ENCRYPTION
    // ================================================
    string encryptedText = encrypt(text, key);


    // ================================================
    // DECRYPTION
    // ================================================
    string decryptedText = decrypt(encryptedText, key);


    // ================================================
    // DISPLAY RESULTS
    // ================================================
    cout << "\n========== VERNAM CIPHER ==========\n";
    cout << "Plaintext          : " << text << endl;
    cout << "Key                : " << key << endl;
    cout << "Ciphertext         : " << encryptedText << endl;
    cout << "Decrypted Plaintext: " << decryptedText << endl;


    // ================================================
    // CHECK
    // ================================================
    if (decryptedText == text)
        cout << "\nSUCCESSFUL\n";
    else
        cout << "\nFAILED\n";

    return 0;
}
