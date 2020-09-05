/*
 * This program reads an input file and uses it to build a large data structure of word
 * groups called "N-grams" as a basis for randomly generating new text that sounds like
 * it came from the same author as that file.
 */

#include <iostream>
#include <fstream>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include "hashmap.h"
#include "queue.h"
#include "random.h"
using namespace std;

// Function prototypes
/*
 * Prints the introduction out.
 */
void setUp();
/*
 * Prompts the user to enter a file name and opens it. If the user type a file name that
 * does not exist, reprompts them.
 *
 * @param file    file to open
 */
void openFile(ifstream &file);
/*
 * Reads every word in a file and stores it in a vector.
 *
 * @param file    file to read
 * @param words   vector that contains words
 */
void readFile(ifstream &file, Vector<string> &words);
/*
 * Prompts the user to enter N and returns it. If the user enter N that less than 2,
 * reprompts them.
 */
int promptN();
/*
 * Builds a map from prefixes to suffixes from a vector of words.
 *
 * @param words     vector that contains words
 * @param map       hash map from string queues (prefixes) to string vectors (suffixes)
 */
void buildMap(Vector<string> &words, int n, HashMap<Queue<string>, Vector<string> > &map);
/*
 * Prompts the user to enter a number of random words that they want to generate and
 * returns it.
 */
int promptNumOfRandomWords();
/*
 * Prints out n random words based on a map.
 *
 * @param map       hash map from string queues (prefixes) to string vectors (suffixes)
 * @param n         number of random words
 */
void printRandomWords(HashMap<Queue<string>, Vector<string> > &map, int n);
/*
 * Prints a string queue out.
 *
 * @param q     string queue
 */
void printQueue(Queue<string> q);

// Main function
int main() {
    ifstream file;
    Vector<string> words;
    setUp();
    openFile(file);
    readFile(file, words);

    int n = promptN();
    if (n < 2 or n > words.size()) {
        cout << "N is larger than the number of words in the file." << endl;
        cout << "Unable to compute N-grams." << endl;
        cout << "Exiting." << endl;
        return 0;
    }

    HashMap<Queue<string>, Vector<string> > map;
    buildMap(words, n, map);
    int num_of_random_words;
    do {
        num_of_random_words = promptNumOfRandomWords();
        if (num_of_random_words == 0) break;
        printRandomWords(map, num_of_random_words);
    } while (true);
    cout << "Exiting." << endl;
    return 0;
}

void setUp() {
    cout << "Welcome to CS 106X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups of"
            "words, and I will generate random text for you." << endl;
    cout << endl;
}

void openFile(ifstream &file) {
    string fileName;
    do {
        fileName = getLine("Input file name: ");
        file.open(fileName);
        if (not file.is_open()) cerr << "Unable to open that file. Try again." << endl;
        else break;
    } while (true);
}

void readFile(ifstream &file, Vector<string> &words) {
    string word;
    while (file >> word) words.add(word);
    file.close();
}

int promptN() {
    int n;
    do {
        n = getInteger("Value of N: ");
        if (n < 2) cout << "N must be 2 or greater." << endl;
        else break;
    } while (true);
    return n;
}

void buildMap(Vector<string> &words, int n, HashMap<Queue<string>, Vector<string> > &map) {
    Queue<string> window;
    for (int i = 0; i < n-1; i++) {
        words.add(words[i]);
        window.enqueue(words[i]);
    }
    for (int i = n-1; i < words.size(); i++) {
        if (map.containsKey(window)) {
            map[window].add(words[i]);
        } else {
            Vector<string> suffix;
            suffix.add(words[i]);
            map.add(window, suffix);
        }
        window.dequeue();
        window.enqueue(words[i]);
    }
}

int promptNumOfRandomWords() {
    cout << endl;
    return getInteger("# of random words to generate (0 to quit): ");
}

void printRandomWords(HashMap<Queue<string>, Vector<string> > &map, int n) {
    Vector<Queue<string> > keys = map.keys();
    Queue<string> window = keys[randomInteger(0, map.size()-1)];
    cout << "... ";
    printQueue(window);
    for (int i = 2; i <= n; i++) {
        Vector<string> values = map[window];
        string suffix = values[randomInteger(0, values.size()-1)];
        cout << suffix << " ";
        window.dequeue();
        window.enqueue(suffix);
    }
    cout << " ..." << endl;
}

void printQueue(Queue<string> q) {
    while (!q.isEmpty()) {
        cout << q.dequeue() << " ";
    }
}
