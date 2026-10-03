#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Custom mod to handle negative numbers
int mymod(int a, int b)
{
    int r = a % b;
    return (r < 0) ? r + b : r;
}

// Vernam Encryption: C[i] = (P[i] + K[i]) mod 26
string encrypt(const string &text, const string &key)
{
    string cipher = "";
    for (size_t i = 0; i < text.length(); i++)
    {
        char p = text[i];
        char k = key[i];

        if (isalpha(p) && isalpha(k))
        {
            char baseP = isupper(p) ? 'A' : 'a';
            char baseK = isupper(k) ? 'A' : 'a';
            int pVal = p - baseP;
            int kVal = k - baseK;
            cipher += static_cast<char>(baseP + mymod(pVal + kVal, 26));
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

        if (isalpha(c) && isalpha(k))
        {
            char baseC = isupper(c) ? 'A' : 'a';
            char baseK = isupper(k) ? 'A' : 'a';
            int cVal = c - baseC;
            int kVal = k - baseK;
            text += static_cast<char>(baseC + mymod(cVal - kVal, 26));
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
