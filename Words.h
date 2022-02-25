//
// Created by kianr on 4/13/2021.
//

/**
 * This class is used to hold every single word.
 * It is primarily used for the AVL Tree.
 * It will store a word as a string, as well as have a wordMap.
 * The wordMap is used to hold all the documents that have that word, as well as the frequency in each document.
 */


#ifndef SEARCH_ENGINE_WORDS_H
#define SEARCH_ENGINE_WORDS_H

#include <map>
#include <string>
#include <iostream>

using namespace std;

class Words {
private:
    string word;
    map<string, int> wordMap;  //removes duplicates and also holds id and count in id
    int totalcount = 0;
public:
    Words();
    Words(string word);
    Words(char* word);

    void setWord(const string &temp);
    string getWord();
    map<string, int>& getMap();
    int getCount();

    void addID(string id);


    /// returning error if i did not add this

    bool operator> (const Words& rhs)  const {
        return word > rhs.word;
    };
    bool operator< (const Words& rhs)  const {
        return word < rhs.word;
    };
    bool operator== (const Words& rhs) const {
        return word == rhs.word;
    };


};


#endif //SEARCH_ENGINE_WORDS_H
