//
// Created by tibov on 29/04/23.
//

#include "ByteX.h"

ByteX::ByteX(unsigned int value, unsigned int length): value(value), length(length) {};

string ByteX::toString() const {
    string s = "";
    unsigned int temp = value;
    for (int i=0; i < length; i++){
        string old_s = s;
        s = static_cast<unsigned char>(temp);
        s += old_s;
        temp = temp >> 8;
    }
    return s;
}

string ByteX::toHex() const {
    stringstream s;
    unsigned int temp = value;
    for (int i=0; i < length; i++){
        string old_s = s.str();
        s.str("");
        s << hex << int(static_cast<unsigned char>(temp));
        s << " " << old_s;
        temp = temp >> 8;
    }
    return s.str();
}

int ByteX::getByte(int index) const{
    unsigned int output = value;
    for (int i = 0; i < length-1-index; i++){
        output = output >> 8;
    }
    return int(static_cast<unsigned char>(output));
}

bool ByteX::equalsHex(const string &hex_value, int index) const {
    int v;
    stringstream ss;
    ss << hex << hex_value;
    ss >> v;
    if (hex_value.size() == 1){
        return (getByte(index) >> 4) == v;
    }else{
        return getByte(index) == v;
    }
}

int ByteX::getNibble(int index, bool first) const {
    int val = getByte(index);
    if (first){
        val = val >> 4;
    }else{
        val = val << 4;
        val = int(static_cast<unsigned char>(val));
        val = val >> 4;
    }
    return val;
}

void ByteX::setByte(int index, unsigned int insert_value) {
    unsigned int temp = value;
    unsigned int new_value = 0;
    for (int i = 0; i < length; i++){
        unsigned int v = int(static_cast<unsigned char>(temp));
        if (length-1-i == index){
            v = insert_value;
        }
        new_value = (v << 8 * i) + new_value;

        temp = temp >> 8;
    }
    value = new_value;
}

void ByteX::concatinateDeltaTime(ByteX add) {
    int MSB = getByte(0);

    if (MSB >= 128){
        MSB -= 128;
    }
    setByte(0, MSB);

    MSB = add.getByte(0);
    int original = MSB;
    if (MSB >= 128){
        MSB -= 128;
    }

    add.setByte(0, MSB);
    unsigned int new_value = (value << 7) + add.value;

    add.setByte(0, original);

    value = new_value;
    length = length+add.length;
}

bool ByteX::getMSB(int index) {
    return getByte(index) >= 128;
}

long unsigned int ByteX::getValue() const {
    return value;
}
