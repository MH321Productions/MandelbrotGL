#include <iostream>

#include "Unicode.hpp"

using namespace std;

const size_t String32::npos = static_cast<size_t>(-1);

String32::String32(const char32_t* text, const std::size_t& max) : sizeIntern(0) {
    for (size_t i = 0; i < max; i++) {
        if (text[i] == '\0') break; //'\0' ist das Schlusszeichen eines C-Strings

        data.push_back((uint32_t) text[i]);
        sizeIntern++;
    }
}

String32::String32(const std::string& ascii) : sizeIntern(ascii.size()) {
    data.resize(sizeIntern);

    for (size_t i = 0; i < sizeIntern; i++) {
        data.at(i) = (uint32_t) ascii.at(i);
    }
}

vector<uint32_t>::iterator String32::begin() {
    return data.begin();
}

vector<uint32_t>::iterator String32::end() {
    return data.end();
}

vector<uint32_t>::const_iterator String32::begin() const {
    return data.begin();
}

vector<uint32_t>::const_iterator String32::end() const {
    return data.end();
}


bool String32::isEmpty() const {
    return sizeIntern == 0;
}

size_t String32::size() const {
    return sizeIntern;
}

uint32_t& String32::at(const size_t& index) {
    return data.at(index);
}

void String32::append(const String32& other) {
    size_t max = sizeIntern + other.sizeIntern;
    data.resize(max);

    for (size_t i = 0; i < other.sizeIntern; i++) {
        data.at(sizeIntern + i) = other.data.at(i);
    }

    sizeIntern = max;
}

void String32::append(char32_t* text, const std::size_t& max) {
    for (size_t i = 0; i < max; i++) {
        if (text[i] == '\0') break; //'\0' ist das Schlusszeichen eines C-Strings

        data.push_back((uint32_t) text[i]);
        sizeIntern++;
    }
}

void String32::append(const std::vector<uint32_t>& newData) {
    size_t max = sizeIntern + newData.size();
    data.resize(max);

    for (size_t i = 0; i < newData.size(); i++) {
        data.at(sizeIntern + i) = newData.at(i);
    }

    sizeIntern = max;
}

void String32::append(const char32_t& c) {
    data.push_back((uint32_t) c);
    sizeIntern++;
}

void String32::append(const uint32_t& i) {
    data.push_back(i);
    sizeIntern++;
}

String32& String32::operator += (const String32& other) {
    append(other);
    return *this;
}

String32& String32::operator += (char32_t* other) {
    append(other);
    return *this;
}

String32& String32::operator += (const std::vector<uint32_t>& other) {
    append(other);
    return *this;
}

String32& String32::operator += (const char32_t& other) {
    append(other);
    return *this;
}

String32& String32::operator += (const uint32_t& other) {
    append(other);
    return *this;
}

String32 String32::operator + (const String32& other) const {
    size_t newSize = sizeIntern + other.sizeIntern;
    vector<uint32_t> newData(newSize);

    size_t index = 0;

    for (const uint32_t& i: data) newData.at(index) = i; index++; //Aktueller String
    for (const uint32_t& i: other.data) newData.at(index) = i; index++; //Neuer String

    return {newData, newSize};
}

String32 String32::operator + (char32_t* other) const {
    size_t newSize = sizeIntern;
    vector<uint32_t> newData(newSize);

    size_t index = 0;

    for (const uint32_t& i: data) newData.at(index) = i; index++; //Aktueller String
    
    index = 0;
    while (other[index] != '\0') { //Neuer String
        newData.push_back((uint32_t) other[index]);
        newSize++;
        index++;
    }

    return {newData, newSize};
}

String32 String32::operator + (const std::vector<uint32_t>& other) const {
    size_t newSize = sizeIntern + other.size();
    vector<uint32_t> newData(newSize);

    size_t index = 0;

    for (const uint32_t& i: data) newData.at(index) = i; index++; //Aktueller String
    for (const uint32_t& i: other) newData.at(index) = i; index++; //Neuer String

    return {newData, newSize};
}

String32 String32::operator + (const char32_t& c) const {
    size_t newSize = sizeIntern + 1;
    vector<uint32_t> newData(newSize);

    size_t index = 0;

    for (const uint32_t& i: data) newData.at(index) = i; index++; //Aktueller String
    newData.at(index) = (uint32_t) c; //Neuer String

    return {newData, newSize};
}

String32 String32::operator + (const uint32_t& i) const {
    size_t newSize = sizeIntern + 1;
    vector<uint32_t> newData(newSize);

    size_t index = 0;

    for (const uint32_t& i: data) newData.at(index) = i; index++; //Aktueller String
    newData.at(index) = i; //Neuer String

    return {newData, newSize};
}

String32& String32::operator = (const String32& other) {
    data = other.data;
    sizeIntern = other.sizeIntern;

    return *this;
}

String32& String32::operator = (char32_t* other) {
    data.clear();
    sizeIntern = 0;

    while (other[sizeIntern] != '\0') data.push_back((uint32_t) other[sizeIntern]); sizeIntern++; //Neuer String

    return *this;
}

String32& String32::operator = (const std::vector<uint32_t>& other) {
    data = other;
    sizeIntern = other.size();

    return *this;
}

String32& String32::operator = (const char32_t& c) {
    data.resize(1);
    sizeIntern = 1;

    data.at(0) = (uint32_t) c;

    return *this;
}

String32& String32::operator = (const uint32_t& i) {
    data.resize(1);
    sizeIntern = 1;

    data.at(0) = i;

    return *this;
}
