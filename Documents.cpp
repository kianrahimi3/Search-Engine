//
// Created by kianr on 4/13/2021.
//

#include "Documents.h"

Documents::Documents() {
    id = "";
    title = "";
    docText = "";
}

/**
 * Setter and Getter for Document ID
 * @param temp
 */
void Documents::setDocID(string &temp) {
    id = temp;
}
string Documents::getDocID() {
    return id;
}

/**
 * Adder and Getter for Vector
 * @param temp
 */
void Documents::addToVec(string &temp) {
    vec.push_back(temp);
}
vector<string> Documents::getVec() {
    return vec;
}

/**
 * Setter and Getter for Document Text
 * @param temp
 */
void Documents::setDocText(string &temp) {
    docText = temp;
}
string Documents::getDocText() {
    return docText;
}
