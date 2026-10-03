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
// CAESAR ENCRYPTION
// C = (P + shift) mod 26
// ======================================================
string encrypt(string plaintext, int shift)
{
    string ciphertext = "";

    for (int i = 0; i < (int)plaintext.length(); i++)
    {
        char ch = plaintext[i];

        if (isalpha(ch))
        {
            bool isUpper = isupper(ch);
            int pIndex = getIndex(ch);

            // Shift using alphabet array
            int cIndex = mymod(pIndex + shift, 26);
            char encChar = ALPHABET[cIndex];

            if (isUpper)
                encChar = toupper(encChar);

            ciphertext += encChar;
        }
        else
        {
            // Keep non-alphabet characters as they are
            ciphertext += ch;
        }
    }

    return ciphertext;
}


// ======================================================
// CAESAR DECRYPTION
// P = (C - shift) mod 26
// ======================================================
string decrypt(string ciphertext, int shift)
{
    string plaintext = "";

    for (int i = 0; i < (int)ciphertext.length(); i++)
    {
        char ch = ciphertext[i];

        if (isalpha(ch))
        {
            bool isUpper = isupper(ch);
            int cIndex = getIndex(ch);

            // Reverse shift using alphabet array
            int pIndex = mymod(cIndex - shift, 26);
            char decChar = ALPHABET[pIndex];

            if (isUpper)
                decChar = toupper(decChar);

            plaintext += decChar;
        }
        else
        {
            // Keep non-alphabet characters as they are
            plaintext += ch;
        }
    }

    return plaintext;
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    string text;
    int shift;

    // ================================================
    // INPUT
    // ================================================
    cout << "Enter plaintext message: ";
    getline(cin, text);

    cout << "Enter shift key (e.g. 3): ";
    cin >> shift;


    // ================================================
    // ENCRYPTION
    // ================================================
    string encryptedText = encrypt(text, shift);


    // ================================================
    // DECRYPTION
    // ================================================
    string decryptedText = decrypt(encryptedText, shift);


    // ================================================
    // DISPLAY RESULTS
    // ================================================
    cout << "\n========== CAESAR CIPHER ==========\n";
    cout << "Original Plaintext : " << text << endl;
    cout << "Shift Key          : " << shift << endl;
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
