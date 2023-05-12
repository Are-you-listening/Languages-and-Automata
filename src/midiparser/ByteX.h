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
public:
    /**
     * constructor for byte based on a value and a buyte_length
     */
    ByteX(unsigned int value, unsigned int length);

    /**
     * constructor for byte based on hex
     */
    ByteX(const string& hex_value);

    /**
     * get the string representation of the bytes
     */
    string toString() const;

    /**
     * get the hex representation of the bytes
     */
    string toHex() const;

    /**
     * get the value of the byte on the given index
     */
    int getByte(int index) const;

    /**
     * get the value of the nibble on the given index (also choose first or last nibble)
     */
    int getNibble(int index, bool first) const;

    /**
     * change the value of a certain byte on a certain index
     */
    void setByte(int index, unsigned int insert_value);

    /**
     * check equal between a hex(in string) and a byte
     * */
    bool equalsHex(const string& hex_value, int index) const;

    /**
     * do the special concatinate to support the delta time format
     */
    void concatinateDeltaTime(ByteX add);

    /**
     * get if the MSB is true/false from a certain index
     */
    bool getMSB(int index);

    /**
     * get numerical value
     */
    long unsigned int getValue() const;

private:
    unsigned int length;
    long unsigned int value;
};


#endif //MIDIPARSER_BYTEX_H
