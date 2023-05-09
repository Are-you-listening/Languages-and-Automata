//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_BYTEX_H
#define MIDIPARSER_BYTEX_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class ByteX {
private:
    unsigned int length;
    long unsigned int value;

public:
    ByteX(unsigned int value, unsigned int length);
    ByteX(const string& hex_value);
    string toString() const;
    string toHex() const;
    int getByte(int index) const;
    int getNibble(int index, bool first) const;
    void setByte(int index, unsigned int insert_value);
    bool equalsHex(const string& hex_value, int index) const;
    void concatinateDeltaTime(ByteX add);
    bool getMSB(int index);
    long unsigned int getValue() const;
};


#endif //MIDIPARSER_BYTEX_H
