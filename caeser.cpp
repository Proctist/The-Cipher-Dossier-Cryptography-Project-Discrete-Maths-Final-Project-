/*

 C++ program for implementation of Caesar Cipher algorithm

*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Function to encrypt the message text using Caesar cipher
void encrypt(const string &message, int shift, string &output) {
    // Iterate through each character, apply shift if
    // it's a letter (taking care of wrap-around) and build the output string
    for (char c : message) {
        if (c >= 'a' && c <= 'z') {
            int dividend = c - 'a' + shift;
            int quotient = dividend / 26;
            int remainder = dividend - (quotient * 26);
            output += char(remainder + 'a');
        }
        else if (c >= 'A' && c <= 'Z') {
            int dividend = c - 'A' + shift;
            int quotient = dividend / 26;
            int remainder = dividend - (quotient * 26);
            output += char(remainder + 'A');
        } else {
            output += c;
        }
    }
}

// Function to decrypt the message text using Caesar cipher
void decrypt(const string &message, int shift, string &output) {
    // Iterate through each character, apply shift if
    // it's a letter (taking care of wrap-around) and build the output string
    for (char c : message)
    {
        if (c >= 'a' && c <= 'z')
        {
            int dividend = c - 'a' - shift + 26;
            int quotient = dividend / 26;
            int remainder = dividend - (quotient * 26);
            output += char(remainder + 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            int dividend = c - 'A' - shift + 26;
            int quotient = dividend / 26;
            int remainder = dividend - (quotient * 26);
            output += char(remainder + 'A');
        }
        else
        {
            output += c;
        }
    }
}

int main() {
    // Initialize variables
    int shift,
        random,
        quotient,
        remainder;
    char direction;
    char input;
    string file,
           text,
           message,
           encrypted = "",
           decrypted = "";

    // Generate random shift value between 0 to 26
    srand(time(NULL));
    random = rand();
    quotient = random / 26;
    remainder = random - quotient * 26;
    shift = remainder;
    cout << "Key: " << shift << endl;

    // File handling
    cout << "Get message from file? (Y/N): ";
    cin >> input;
    if (toupper(input) == 'Y') {
        int messageNum = 1;

        cout << "Enter file name with extension: ";
        cin >> file;

        ifstream readFile(file);
        ofstream outFile("outputCaesar.txt");

        cout << "In which direction do you want to shift? (L for left, R for right): ";
        cin >> direction;
        cin.ignore();

        // Adjust shift for left direction
        if (toupper(direction) == 'L') {
            shift = (26 - shift); // Wrap around for left shift
        }
        else if (toupper(direction) != 'R') {
            cout << "Invalid direction." << endl;
            return 1;
        }

        // Get each line and output
        while (getline(readFile, text)) {
            encrypted = "",
            decrypted = "";
            encrypt(text, shift, encrypted);
            decrypt(encrypted, shift, decrypted);
            outFile << "Message #" << messageNum << '\n'
                    << "Encrypted message: " << encrypted << '\n'
                    << "Decrypted message: " << decrypted << '\n'
                    << endl;
            ++messageNum;
        }

        cout << "Output saved to 'outputCaesar.txt'.\n";
        readFile.close();
        outFile.close();
        return 0;
    }
    else if (toupper(input) != 'N') {
        cout << "Invalid option.\n";
        return 1;
    }

    cout << "In which direction do you want to shift? (L for left, R for right): ";
    cin >> direction;
    cin.ignore();

    // Adjust shift for left direction
    if (toupper(direction) == 'L') {
        shift = (26 - shift); // Wrap around for left shift
    }
    else if (toupper(direction) != 'R') {
        cout << "Invalid direction." << endl;
        return 1;
    }

    // Input text to encrypt
    cout << "Enter text to encrypt: ";
    getline(cin, message);

    // Encrypt and decrypt
    encrypt(message, shift, encrypted);
    cout << "Encrypted text: " << encrypted << endl;

    decrypt(encrypted, shift, decrypted);
    cout << "Decrypted text: " << decrypted << endl;
}