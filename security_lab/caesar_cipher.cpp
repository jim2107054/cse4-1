#include <bits/stdc++.h>

using namespace std;

// Caesar Encryption: C = (P + shift) mod 26
string encrypt(const string &text, int shift)
{
    string cipher = "";
    shift = (shift % 26 + 26) % 26;

    for (char ch : text)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            cipher += (ch - 'A' + shift) % 26 + 'A';
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            cipher += (ch - 'a' + shift) % 26 + 'a';
        }
        else
        {
            cipher += ch;
        }
    }
    return cipher;
}

// Caesar Decryption: P = (C - shift) mod 26
string decrypt(const string &cipher, int shift)
{
    string text = "";
    shift = (shift % 26 + 26) % 26;

    for (char ch : cipher)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            text += (ch - 'A' - shift + 26) % 26 + 'A';
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            text += (ch - 'a' - shift + 26) % 26 + 'a';
        }
        else
        {
            text += ch;
        }
    }
    return text;
}

int main()
{
    string text;
    int shift;

    cout << "Enter plaintext: ";
    getline(cin, text);

    cout << "Enter shift: ";
    cin >> shift;

    string cipher = encrypt(text, shift);
    string decrypted = decrypt(cipher, shift);

    cout << "\nCiphertext: " << cipher << "\n";
    cout << "Decrypted : " << decrypted << "\n";

    return 0;
}
