/*

 C++ program for implementation of XOR algorithm

*/

#include <iostream>
#include <string>
#include <fstream>
// Include diffie_hellman.h header file for XOR key generation
#include "diffie_hellman.h"
using namespace std;

int main() {
    // Initialize variables
    int key;
    char input;
    string file,
           text,
           message,
           encrypted = "",
           decrypted = "";

    // Generate XOR key from Diffie-Hellman Key Exchange
    key = generateKey();
    cin.ignore();

    // File handling
    cout << "Get message from file? (Y/N): ";
    cin >> input;
    cin.ignore();
    if (toupper(input) == 'Y') {
        int messageNum = 1;

        cout << "Enter file name with extension: ";
        cin >> file;
        cin.ignore();

        ifstream readFile(file);
        ofstream outFile("outputXOR.txt");

        // Get each line and output
        while (getline(readFile, text)) {
            encrypted = "",
            decrypted = "";

            // XOR each character in message with key to encrypt and decrypt
            for (char c : text) {
                encrypted += c ^ key;
            }

            for (char c : encrypted) {
                decrypted += c ^ key;
            }

            outFile << "Message #" << messageNum << '\n'
                    << "Encrypted message: " << encrypted << '\n'
                    << "Decrypted message: " << decrypted << '\n'
                    << endl;
            ++messageNum;
        }

        cout << "Output saved to 'outputXOR.txt'.\n";
        readFile.close();
        outFile.close();
        return 0;
    }
    else if (toupper(input) != 'N') {
        cout << "Invalid option.\n";
        return 1;
    }

    // Input message
    cout << "Enter a string to encrypt: ";
    getline(cin, message);

    // XOR each character in message with key to encrypt and decrypt
    for (char c : message) {
        encrypted += c ^ key;
    }

    for (char c : encrypted) {
        decrypted += c ^ key;
    }

    // Output encrypted and decrypted message
    cout << "Encrypted string: " << encrypted << endl;
    cout << "Decrypted string: " << decrypted << endl;
}