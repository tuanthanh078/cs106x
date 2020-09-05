/*
 * One of the most important tools in modern computing is the ability to encrypt data.
 * There are numerous different types of ciphers, including the Caesar cipher (as we
 * discussed in class), the "one-time-pad" (a perfect cipher, but not without issues),
 * public-key ciphers (used in part for virtually all secure web transactions), etc.
 * This program encrypts and decrypts text using a "transposition" cipher.
 * A transposition cipher is a method for encrypting a message by transposing the
 * characters in the message by putting the message into a rectangular grid and reading
 * down the columns in an order based on a textual key.
 */
#include <iostream>
#include <string>
#include <algorithm>
#include "console.h"
#include "simpio.h"  // for getLine
#include "vector.h"  // for Vector
#include "grid.h"
using namespace std;

// Transposition Cipher

const char PAD = '~'; // padding character for cipher

// Function Prototypes
// Add more functions of your own, as well
/*
 * Changes a readable message into a non-readable message via a key
 *
 * @param plaintext    readable, unencrypted message
 * @param key          set of characters used to perform the encryption and decryption
 *                     algorithm
 */
string encrypt(string plaintext, string key);
/*
 * Changes an encrypted message into a readable message via a key and returns this message.
 *
 * @param ciphertext    unreadable, encrypted message
 * @param key           set of characters used to perform the encryption and decryption
 *                      algorithm
 */
string decrypt(string ciphertext, string key);
/*
 * Transforms a string into a grid. This grid can be read through each row from left to
 * right.
 *
 * @param str       string to transform
 * @param strGrid   string grid
 */
void stringToGrid(string &str, Grid<string> &strGrid);
/*
 * Transforms a grid into a string and returns this string. This grid can be read through
 * each row from left to right.
 *
 * @param strGrid   string grid
 */
string gridToString(Grid<string> &strGrid);
/*
 * Transforms a grid into a ciphertext and returns this ciphertext. This grid can be read
 * through each column from top to bottom.
 *
 * @param strGrid   string grid
 */
string gridToCipherText(Grid<string> &strGrid);
/*
 * Transforms a ciphertext into a grid. This grid can be read through each column from
 * top to bottom.
 *
 * @param ciphertext    unreadable, encrypted message
 * @param strGrid       string grid
 */
void cipherTextToGrid(string &ciphertext, Grid<string> &strGrid);
/*
 * Generates the alphabetic ordering of the letters in the key. The alphabetic ordering
 * for duplicate letters puts the left-most letter first.
 *
 * @param key         set of characters used to perform the encryption and decryption
 *                    algorithm
 * @param order       alphabetic ordering of the letters in the key
 */
void generateOrder(string key, Vector<int> &order);
/*
 * Adds the padding character to the end of a string until the key evenly divides the
 * length of the string.
 *
 * @param text  string to pad
 * @param n     length of key
 */
void pad(string &text, int n);
/*
 * Removes any padding character that are present at the end of the plain text.
 *
 * @param text  string to unpad
 */
void unpad(string &text);
/*
 * Prints out a string grid for debugging.
 *
 * @param strGrid  string grid
 */
void printStringGrid(Grid<string> &strGrid);
/*
 * Sorts columns in a string grid in ascending order, given an original order.
 *
 * @param strGrid  string grid
 * @param order    original order of columns in string grid
 */
void sortByOrder(Grid<string> &strGrid, Vector<int> order);
/*
 * Sorts columns in a string grid with an original order.
 *
 * @param strGrid  string grid
 * @param order    original order of columns in string grid
 */
void unsortByOrder(Grid<string> &strGrid, Vector<int> order);
/*
 * Swaps 2 columns of a string grid.
 *
 * @param strGrid  string grid
 * @param a        index of the first column
 * @param b        index of the second column
 */
void swapCol(Grid<string> &strGrid, int a, int b);

// Main function
int main() {
    int encryptChoice = 1;
    while (encryptChoice > 0) {
        cout << "Welcome to the Transposition Cipher Machine!" << endl;
        cout << "Please choose:" << endl;
        cout << "1) encrypt text" << endl;
        cout << "2) decrypt text" << endl;
        encryptChoice = getInteger("Please type your choice, 0 to end:","Please type an integer!");

        if (encryptChoice > 0) {
            cout << "Please enter the text to " << (encryptChoice == 1 ? "encrypt: " : "decrypt: ");
            string text = getLine();
            string key = getLine("Please type in a key: ");
            string output;
            if (encryptChoice == 1) {
                output = encrypt(text, key);
                cout << "Encrypted ";

            } else if (encryptChoice == 2) {
                output = decrypt(text, key);
                cout << "Decrypted ";
            }
            cout << "text:" << endl << endl << "\"" << output << "\"" << endl << endl;
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
}

string encrypt(string plaintext, string key) {
    int length_of_key = key.size();
    pad(plaintext, length_of_key);
    Grid<string> strGrid (plaintext.size()/length_of_key, length_of_key);
    Vector<int> order;
    stringToGrid(plaintext, strGrid);
    generateOrder(key, order);
    sortByOrder(strGrid, order);
    string ciphertext = gridToCipherText(strGrid);
    return ciphertext;
}

string decrypt(string ciphertext, string key) {
    int length_of_key = key.size();
    if (ciphertext.size() % length_of_key != 0) {
        throw("Ciphertext length is not divisible by key length!");
    }
    Grid<string> strGrid (ciphertext.size()/length_of_key, length_of_key);
    Vector<int> order;
    cipherTextToGrid(ciphertext, strGrid);
    generateOrder(key, order);
    unsortByOrder(strGrid, order);
    string plaintext = gridToString(strGrid);
    unpad(plaintext);
    return plaintext;
}

void stringToGrid(string &str, Grid<string> &strGrid) {
    int num_of_columns = strGrid.numCols();
    for (unsigned i = 0; i < str.size(); i++) {
        strGrid[i/num_of_columns][i%num_of_columns] = str[i];
    }
}

string gridToString(Grid<string> &strGrid) {
    string str = "";
    int length = strGrid.numCols() * strGrid.numRows();
    int num_of_columns = strGrid.numCols();
    for (unsigned i = 0; i < length; i++) {
        str += strGrid[i/num_of_columns][i%num_of_columns];
    }
    return str;
}

void pad(string &text, int n) {
    string padding((n - text.size()%n)%n, PAD);
    text += padding;
}

void unpad(string &text) {
//    for (int i = text.size()-1; i >= 0; i--) {
//        if (text.at(i) == PAD) text.pop_back();
//    }
    while (text.back() == PAD) text.pop_back();
}

void printStringGrid(Grid<string> &strGrid) {
    for (int i = 0; i < strGrid.numRows(); i++) {
        for (int j = 0; j < strGrid.numCols(); j++) {
            cout << (strGrid[i][j] == " " ? "_" : strGrid[i][j]) << " ";
        }
        cout << endl;
    }
}

void generateOrder(string key, Vector<int> &order) {
    // compsci
    // 0435612
    // 0123456
    for (int i = 0; i < (int) key.size(); i++) {
        order.add((int) key.at(i));
    }
    Vector<int> temp = order;
    sort(temp.begin(), temp.end());
    for (int i = 0; i < (int) key.size(); i++) {
        for (int j = 0; j < (int) key.size(); j++) {
            if (temp[i] == order[j]) {
                order[j] = i;
                temp[i] = 9999;
            }
        }
    }
    cout << endl;
}

void sortByOrder(Grid<string> &strGrid, Vector<int> order) {
    int min_idx;
    for (int i = 0; i < order.size()-1; i++) {
        min_idx = i;
        for (int j = i+1; j < order.size(); j++) {
            if (order[j] < order[min_idx]) min_idx = j;
        }
        swap(order[i], order[min_idx]);
        swapCol(strGrid, i, min_idx);
    }
}

void swapCol(Grid<string> &strGrid, int a, int b) {
    if (a == b) return;
    for (int i = 0; i < strGrid.numRows(); i++) {
        swap(strGrid[i][a], strGrid[i][b]);
    }
}

string gridToCipherText(Grid<string> &strGrid) {
    string str = "";
    for (int j = 0; j < strGrid.numCols(); j++) {
        for (int i = 0; i < strGrid.numRows(); i++) {
            str += strGrid[i][j];
        }
    }
    return str;
}

void cipherTextToGrid(string &ciphertext, Grid<string> &strGrid) {
    for (int i = 0; i < (int) ciphertext.length(); i++) {
        strGrid[i%strGrid.numRows()][i/strGrid.numRows()] = ciphertext[i];
    }
}

void unsortByOrder(Grid<string> &strGrid, Vector<int> order) {
    Vector<int> temp;
    for (int i = 0; i < (int) order.size(); i++) {
        temp.add(i);
    }
    for (int i = 0; i < order.size()-1; i++) {
        for (int j = 0; j < order.size(); j++) {
            if (order[i] == temp[j]) {
                swapCol(strGrid, i, j);
                swap(temp[i], temp[j]);
                break;
            }
        }
    }
}
