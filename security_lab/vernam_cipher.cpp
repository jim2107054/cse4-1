#include <bits/stdc++.h>

using namespace std;

// Vernam Encryption: C[i] = (P[i] + K[i]) mod 26
string encrypt(const string &text, const string &key)
{
    string cipher = "";
    for (size_t i = 0; i < text.length(); i++)
    {
        char p = text[i];
        char k = key[i];

        // Key value (0-25 for letters, 0-9 for digits)
        int kVal = 0;
        bool validKey = true;

        if (k >= 'A' && k <= 'Z')
            kVal = k - 'A';
        else if (k >= 'a' && k <= 'z')
            kVal = k - 'a';
        else if (k >= '0' && k <= '9')
            kVal = k - '0';
        else
            validKey = false;

        // Plaintext uppercase
        if (p >= 'A' && p <= 'Z' && validKey)
        {
            int pVal = p - 'A';
            cipher += (pVal + kVal) % 26 + 'A';
        }
        // Plaintext lowercase
        else if (p >= 'a' && p <= 'z' && validKey)
        {
            int pVal = p - 'a';
            cipher += (pVal + kVal) % 26 + 'a';
        }
        else
        {
            cipher += p;
        }
    }
    return cipher;
}

// Vernam Decryption: P[i] = (C[i] - K[i]) mod 26
string decrypt(const string &cipher, const string &key)
{
    string text = "";
    for (size_t i = 0; i < cipher.length(); i++)
    {
        char c = cipher[i];
        char k = key[i];

        // Key value (0-25 for letters, 0-9 for digits)
        int kVal = 0;
        bool validKey = true;

        if (k >= 'A' && k <= 'Z')
            kVal = k - 'A';
        else if (k >= 'a' && k <= 'z')
            kVal = k - 'a';
        else if (k >= '0' && k <= '9')
            kVal = k - '0';
        else
            validKey = false;

        // Ciphertext uppercase
        if (c >= 'A' && c <= 'Z' && validKey)
        {
            int cVal = c - 'A';
            text += (cVal - kVal + 26) % 26 + 'A';
        }
        // Ciphertext lowercase
        else if (c >= 'a' && c <= 'z' && validKey)
        {
            int cVal = c - 'a';
            text += (cVal - kVal + 26) % 26 + 'a';
        }
        else
        {
            text += c;
        }
    }
    return text;
}

int main()
{
    string text, key;

    cout << "Enter plaintext: ";
    getline(cin, text);

    cout << "Enter key: ";
    getline(cin, key);

    // Pad / repeat key to match text length if needed
    if (key.length() < text.length())
    {
        string origKey = key;
        while (key.length() < text.length())
            key += origKey;
        key = key.substr(0, text.length());
    }

    string cipher = encrypt(text, key);
    string decrypted = decrypt(cipher, key);

    cout << "\nUsed Key  : " << key << "\n";
    cout << "Ciphertext: " << cipher << "\n";
    cout << "Decrypted : " << decrypted << "\n";

    return 0;
}
