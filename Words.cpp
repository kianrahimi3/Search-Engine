//
// Created by kianr on 4/13/2021.
//

#include "Words.h"

Words::Words() {

}

/**
 *
 * @param newword
 *
 * The following two functions are used to set the word
 */
Words::Words(char *newword) {
    word = newword;
    totalcount = 0;
}
Words::Words(string newword) {
    word = newword;
    totalcount = 0;
}

/**
 *
 * @param temp
 *
 * Getter and setter for word
 */
void Words::setWord(const string &temp) {
    word = temp;
}

string Words::getWord() {
    return word;
}

/**
 *
 * @return
 *
 * Returns the count of the word
 *
 * Because the wordMap holds the document IDs and the # of frequencies in each,
 * just iterate through the map and add up all of the frequencies.
 */
int Words::getCount() {
    map<string, int>::iterator it;
    for(it = wordMap.begin(); it != wordMap.end(); it++) {
        totalcount += it->second;
    }
    return totalcount;
}

/**
 *
 * @return
 *
 * The last two functions are for the map.
 *
 * Get the map and...
 * Add ID. This will search/add for the ID in the map
 */
map<string, int>& Words::getMap() {
    return wordMap;
}

void Words::addID(string id) {
    wordMap[id];
  //  wordMap.at(id) = 0;
}
