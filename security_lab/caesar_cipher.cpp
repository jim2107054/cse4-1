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

// Caesar Encryption: C = (P + shift) mod 26
string encrypt(const string &text, int shift)
{
    string cipher = "";
    for (char ch : text)
    {
        if (isalpha(ch))
        {
            char base = isupper(ch) ? 'A' : 'a';
            cipher += static_cast<char>(base + mymod((ch - base) + shift, 26));
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
    for (char ch : cipher)
    {
        if (isalpha(ch))
        {
            char base = isupper(ch) ? 'A' : 'a';
            text += static_cast<char>(base + mymod((ch - base) - shift, 26));
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
