/*
 * A word ladder is a connection from one word to another formed by changing one letter
 * at a time with the constraint that at each step the sequence of letters still forms
 * a valid word. For example, here is a word ladder connecting the word "code" to the
 * word "data". Each changed letter is underlined as an illustration:
 *              code → cade → cate → date → data
 * There are many other word ladders that connect these two words, but this one is the
 * shortest. There might be others of the same length, but none with fewer steps than
 * this one.
 * In this problem, this program repeatedly prompts the user for two words and finds a
 * minimum-length ladder between the words.
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include "set.h"
#include "hashset.h"
#include "queue.h"
#include "stack.h"
using namespace std;

HashSet<char> alphabet;

// Function prototypes

/*
 * Prints the introduction out and sets up alphabetical string.
 */
void setUp();
/*
 * Prompts the user to enter a dictionary file name and opens it. If the user type a file
 * name that does not exist, reprompts them.
 *
 * @param dictionaryFile    the dictionary file
 */
void openDictionary(ifstream &dictionaryFile);
/*
 * Finds words with specific length in the dictionary hash set.
 *
 * @param dictionary    the dictionary hash set
 * @param wordList      the word-list hash set
 * @param length        length of words to find
 */
void getWordListByLength(HashSet<string> &dictionary, HashSet<string> &wordList, int length);
/*
 * Finds neighbors of a word.
 * A neighbor of a given word w is a word of the same length as w that differs by exactly
 * 1 letter from w. For example, "date" and "data" are neighbors.
 *
 * @param dictionary        the dictionary hash set
 * @param neighborWords     the neighbor-word-list hash set
 * @param word              word to find neighbors
 */
void findNeighborWords(HashSet<string> &dictionary, HashSet<string> &neighborWords, string word);
/*
 * Prompts the user to enter 2 words, reprompt them until the user enter valid words and
 * returns true. Returns false if either of them is empty string.
 * Valid words are words in dictionary, not identical and have the same length.
 *
 * @param dictionary    the dictionary hash set
 * @param word1         the first word
 * @param word2         the second word
 */
bool get2Words(HashSet<string> &dictionary, string &word1, string &word2);
/*
 * Reads a dictionary file and stores it in a hash set.
 *
 * @param dictionaryFile    the dictionary file
 * @param dictionary        the dictionary hash set
 */
void readDictionaryFile(ifstream &dictionaryFile, HashSet<string> &dictionary);
/*
 * Reads a word ladder stack and prints it out from top to bottom.
 *
 * @param wordLadder    the word-ladder stack
 */
void printWordLadder(Stack<string> wordLadder);
/*
 * Finds word ladder of 2 words.
 *
 * @param dictionary    the dictionary file
 * @param word1         the first word
 * @param word2         the second word
 * @param wordLadder    the word-ladder stack
 */
void findWordLadder(HashSet<string> &dictionary, string &word1, string &word2, Stack<string> &wordLadder);

// Main function
int main() {
    string word1, word2;
    HashSet<string> dictionary;
    ifstream dictionaryFile;
    setUp();
    openDictionary(dictionaryFile);
    readDictionaryFile(dictionaryFile, dictionary);
    while (get2Words(dictionary, word1, word2)) {
        Stack<string> wordLadder;
        findWordLadder(dictionary, word1, word2, wordLadder);
        cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
        printWordLadder(wordLadder);
    }
    cout << "Have a nice day" << endl;
    return 0;
}

void setUp() {
    cout << "Welcome to CS 106X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will change the first"
            "into the second by changing one letter at a time." << endl;
    cout << endl;
    string alp = "abcdefghijklmnoqprsxwtuvzy";
    for (int i = 0; i < (int) alp.length(); i++) {
        alphabet.add(alp[i]);
    }
}

void openDictionary(ifstream &dictionaryFile) {
    string dictionaryName;
    do {
        dictionaryName = getLine("Dictionary file name: ");
        dictionaryFile.open(dictionaryName);
        if (not dictionaryFile.is_open()) cerr << "Unable to open that file. Try again." << endl;
        else break;
    } while (true);
}

void readDictionaryFile(ifstream &dictionaryFile, HashSet<string> &dictionary) {
    string word;
    while (getline(dictionaryFile, word)) {
        word = trim(word);
        dictionary.add(word);
    }
    dictionaryFile.close();
}

void getWordListByLength(HashSet<string> &dictionary, HashSet<string> &wordList, int length) {
    for (string word : dictionary) {
        if ((int) word.length() == length) wordList.add(word);
    }
}

bool get2Words(HashSet<string> &dictionary, string &word1, string &word2) {
    HashSet<string> wordList;
    cout << endl;
    while (true) {
        word1 = trim(getLine("Word 1 (or Enter to quit): "));
        transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
        if (word1 == "") return false;
        word2 = trim(getLine("Word 2 (or Enter to quit): "));
        transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
        if (word2 == "") return false;
        if (word1 == word2) {
            cout << "The two words must be different." << endl;
            cout << endl;
            continue;
        }
        if (word1.length() != word2.length()) {
            cout << "The two words must be the same length." << endl;
            cout << endl;
            continue;
        }
        getWordListByLength(dictionary, wordList, word1.length());
        if (wordList.contains(word1) and wordList.contains(word2)) break;
        else cout << "The two words must be found in the dictionary." << endl;
        cout << endl;
    }
    return true;
}

void findNeighborWords(HashSet<string> &dictionary, HashSet<string> &neighborWords, string word) {
    unsigned length = word.length();
    for (unsigned i = 0; i < length; i++) {
        string part1 = word.substr(0, i);
        string part2 = word.substr(i+1);
        for (char letter : alphabet) {
            string newWord = part1 + letter + part2;
            if (dictionary.contains(newWord))
                neighborWords.add(newWord);
        }
    }
}

void findWordLadder(HashSet<string> &dictionary, string &word1, string &word2, Stack<string> &wordLadder) {
    Queue<Stack<string> > ladderQueue;
    Stack<string> originalStack;
    originalStack.add(word1);
    ladderQueue.enqueue(originalStack);
    HashSet<string> usedWords;
    usedWords.add(word1);
    bool isFound = false;
    do {
        Stack<string> s = ladderQueue.dequeue();
        HashSet<string> neighborWords;
        findNeighborWords(dictionary, neighborWords, s.top());
        for (string word : neighborWords) {
            //cout << ladderQueue.toString() << endl;
            if (not usedWords.contains(word)) {
                Stack<string> s2 = s;
                s2.push(word);
                ladderQueue.enqueue(s2);
                usedWords.add(word);
                if (word == word2) {
                    isFound = true;
                    wordLadder = s2;
                    break;
                }
            }
        }
    } while (not (ladderQueue.isEmpty() or isFound));
}

void printWordLadder(Stack<string> wordLadder) {
    while (!wordLadder.isEmpty()) {
        cout << wordLadder.pop() << " ";
    }
    cout << endl;
}
