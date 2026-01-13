/*

 C++ header file for implementation of Diffie-Hellman Key Exchange algorithm

*/

#include <iostream>
#include <string>
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

// Function to check if the two numbers are prime
void primeCheck(int num1, int num2, bool &prime) {
    // Skip numbers less than to 2
    if (num1 < 2 || num2 < 2) {
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

// Function to perform Diffie-Hellman key exchange and return XOR key
int generateKey() {
    // Declare variables
    int privKey1,
        privKey2,
        pubKey1,
        pubKey2,
        sharedKey1,
        sharedKey2,
        modulus,
        base;

    // Generate random prime numbers till 50 and check primality
    bool prime = false;
    srand(time(NULL));
    while (!prime || (modulus == base)) {
        modulus = mod(rand(), 50);
        base = mod(rand(), 50);
        primeCheck(modulus, base, prime);
    }

    // Input private keys for both users
    cout << "Enter user 1's private key (integer): ";
    cin >> privKey1;
    while (privKey1 < 0) {
        cout << "Private key can not be negative. Try again: ";
        cin >> privKey1;
    }

    cout << "Enter user 2's private key (integer): ";
    cin >> privKey2;
    while (privKey2 < 0) {
        cout << "Private key can not be negative. Try again: ";
        cin >> privKey2;
    }

    // Calculate public key for user 1 and 2
    pubKey1 = power(base, privKey1, modulus);
    pubKey2 = power(base, privKey2, modulus);

    // Calculate shared key for user 1 and 2
    sharedKey1 = power(pubKey2, privKey1, modulus);
    sharedKey2 = power(pubKey1, privKey2, modulus);

    // Shared keys are both equal so output any
    cout << "Key: " << sharedKey1 << endl;

    return sharedKey1; // Return the shared key as the XOR key
}