//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_BYTEX_H
#define MIDIPARSER_BYTEX_H
#include <string>
#include <iostream>
using namespace std;

class ByteX {
public:
    ByteX(unsigned int value, unsigned int length);
    string toString() const;
    string toHex() const;
    int getByte(int index) const;
    int getNibble(int index, bool first) const;
    void setByte(int index, unsigned int insert_value);
    bool equalsHex(const string& hex_value, int index) const;
    void concatinateDeltaTime(ByteX add);
    bool getMSB(int index);

    unsigned int getValue() const;


private:
    unsigned int length;
    unsigned int value;
};


#endif //MIDIPARSER_BYTEX_H
