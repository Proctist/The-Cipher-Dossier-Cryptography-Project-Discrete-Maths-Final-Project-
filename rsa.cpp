/*

 C++ program for implementation of RSA algorithm, using Chinese Remainder Theorem (CRT)

 ----------
 p = prime number
 q = prime number
 , where p != q
 n = p * q
 phi(n) = (p - 1) * (q - 1)
 public exponent = e, 1 < e < phi(n) and gcd(e, phi(n)) = 1
 private exponent = d, d ≡ e^-1 (mod phi(n))
 encrypted message = encrypted, encrypted ≡ message^e (mod n)
 original message = message, message ≡ encrypted^d (mod n)
 ----------

 CRT method:
 Changes decryption method for easier and efficient computation

 Formula for decryption changes to:
     dp   = d mod (p − 1)
     dq   = d mod (q − 1)
     qinv = q^-1 mod p
     m1 = c^dp mod p
     m2 = c^dq mod q
     h  = (qinv * (m1 − m2)) mod p
     m  = m2 + h * q

*/

#include <iostream>
#include <fstream>
using namespace std;

// Function to calculate modulus
int mod(int dividend, int divisor) {
    // Calculate modulus and handle negative value
    int quotient = dividend / divisor;
    int remainder = dividend - quotient * divisor;
    if (remainder < 0) {
        remainder += divisor;
    }

    return remainder;
}

// Function to compute exponentiation, using memory efficient method
int power(int base, int exponent, int modulus) {
    // Take exponent and mod at each step, to make computation easier
    int result = 1;
    for (int i = 0; i < exponent; ++i) {
        result = mod(result * base, modulus);
    }

    return result;
}

// Function to calculate Greatest Common Divisor (GCD)
int gcd(int num1, int num2) {
    // Calculate GCD
    while (num2 != 0) {
        int temp = num2;
        num2 = mod(num1, num2);
        num1 = temp;
    }

    return num1;
}

// Function to compute modular inverse
int modInv(int num1, int num2) {
    // Calculate the modular inverse, such that remainder is 1
    // Inverse is '-1' if no inverse exists
    for (int i = 2; i < num2; ++i) {
        int prod = i * num1;
        int remainder = mod(prod, num2);

        if (remainder == 1) {
            return i;
        }
    }

    return -1;
}

// Function to check if the two numbers are prime
void primeCheck(int num1, int num2, bool &prime) {
    // Skip numbers less than/equal to 2 (edge case)
    if (num1 <= 2 || num2 <= 2) {
        prime = false;
        return;
    }

    // Check till square root of each number, till highest number
    int max = (num1 > num2) ? num1 : num2;
    for (int i = 2; i <= max; ++i) {
        if ((i * i <= num1) && (mod(num1, i) == 0)) {
            prime = false;
            return;
        }

        if ((i * i <= num2) && (mod(num2, i) == 0)) {
            prime = false;
            return;
        }
    }

    // Prime if modulus != 0
    prime = true;
}

// Function to compute public and private keys
void generateKeys(int &primeP, int &primeQ, int &modulus, int &publicExponent, int &privateExponent, int &phi) {
    // Generate random prime numbers till 50 and check primality
    bool prime = false;
    srand(time(NULL));
    while (!prime || (primeP == primeQ)) {
        primeP = mod(rand(), 50);
        primeQ = mod(rand(), 50);
        primeCheck(primeP, primeQ, prime);
    }

    modulus = primeP * primeQ;
    phi = (primeP - 1) * (primeQ - 1);

    // Find public exponent such that gcd(publicExponent, phi) == 1
    // Inverse must not be '-1'
    privateExponent = -1;
    for (publicExponent = 2; publicExponent < phi; ++publicExponent) {
        if (gcd(publicExponent, phi) == 1) {
            int inv = modInv(publicExponent, phi);
            if (inv != -1) {
                privateExponent = inv;
                break;
            }
        }
    }
}

// Function to encrypt message
int encrypt(int message, int exponent, int modulus) {
    return power(message, exponent, modulus);
}

// Function to decrypt cipher, using CRT method
int decrypt(int encrypted, int privateExponent, int modulus, int primeP, int primeQ) {
    int privateExponentP = mod(privateExponent, (primeP - 1));
    int privateExponentQ = mod(privateExponent, (primeQ - 1));

    int decryptedP = power(encrypted, privateExponentP, primeP);
    int decryptedQ = power(encrypted, privateExponentQ, primeQ);
    int diff = decryptedP - decryptedQ;
    int diffMod = mod(diff, primeP);
    int primeQInv = modInv(primeQ, primeP);
    int h = mod(primeQInv * diffMod, primeP);

    return decryptedQ + (h * primeQ);
}

int main() {
    // Declare variables
    int primeP,
        primeQ,
        publicExponent,
        privateExponent,
        modulus,
        phi,
        message,
        encrypted,
        decrypted;
    char input;
    string file,
           text;

    // Generate keys
    generateKeys(primeP, primeQ, modulus, publicExponent, privateExponent, phi);

    cout << "Public Key (e, n): (" << publicExponent << ", " << modulus << ")\n"
         << "Private Key (d, n): (" << privateExponent << ", " << modulus << ")\n";

    // File handling
    cout << "Get message from file? (Y/N): ";
    cin >> input;
    if (toupper(input) == 'Y') {
        // Get file name, read and open file, and create output file
        int messageNum = 1;
        cout << "Enter file name with extension: ";
        cin >> file;

        ifstream readFile(file);
        ofstream outFile("outputRSA.txt");

        // Get each line in file
        while (getline(readFile, text)) {
            int textMessage = 0;

            // Convert numeric string to number
            for (char c : text) {
                textMessage = (textMessage * 10) + (c - '0');
            }

            // Handle range
            if (textMessage < 0 || textMessage >= modulus) {
                outFile << "Message #" << messageNum << '\n'
                        << "ERROR: Message not in range of 0 to " << (modulus - 1) << " (" << modulus << " = p * q)." << '\n' << endl;
                ++messageNum;
                continue;
            }

            // Send output to file
            encrypted = encrypt(textMessage, publicExponent, modulus);
            decrypted = decrypt(encrypted, privateExponent, modulus, primeP, primeQ);
            outFile << "Message #" << messageNum << '\n'
                    << "Encrypted message: " << encrypted << '\n'
                    << "Decrypted message: " << decrypted << '\n' << endl;
            ++messageNum;
        }

        // Close files
        cout << "Output saved to 'outputRSA.txt'.\n";
        readFile.close();
        outFile.close();
        return 0;
    }
    else if (toupper(input) != 'N') {
        cout << "Invalid option.\n";
        return 1;
    }

    // Input message
    cout << "Enter message: ";
    cin >> message;

    // Handle range
    while (message < 0 || message >= modulus) {
        cout << "Message not in range of 0 to " << (modulus - 1) << " (" << modulus << " = p * q). Try again: ";
        cin >> message;
    }

    // Output original, encrypted and decrypted message
    cout << "Message: " << message << endl;

    encrypted = encrypt(message, publicExponent, modulus);
    cout << "Encrypted Message: " << encrypted << endl;

    decrypted = decrypt(encrypted, privateExponent, modulus, primeP, primeQ);
    cout << "Decrypted Message: " << decrypted << endl;
}