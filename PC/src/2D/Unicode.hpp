#include <vector>
#include <string>
#include <stdint.h>

class String32 {
    private:
        std::vector<uint32_t> data;
        std::size_t sizeIntern;

        //Intern genutzt
        String32(const std::vector<uint32_t>& data, std::size_t& size) : data(data), sizeIntern(size) {}
    
    public:
        static const std::size_t npos;

        /**
         * Konstruktor aus UTF-32 Text
         * @param text Der UTF-32 Text, erstellbar mit U"...";
         * @param max Die maximale Anzahl an zu kopierenden Zeichen (weglassen, um alle Zeichen zu kopieren)
         */ 
        String32(const char32_t* text, const std::size_t& max = npos);

        /**
         * Konstruktor aus 32-Bit Daten
         * @param data Die 32-Bit Daten
         */ 
        String32(const std::vector<uint32_t>& data) : data(data), sizeIntern(data.size()) {}

        /**
         * Konstruktor aus einem ASCII-String
         * @param ascii Der STL ASCII-String
         */ 
        String32(const std::string& ascii);

        //Iteratoren
        std::vector<uint32_t>::iterator begin();
        std::vector<uint32_t>::iterator end();
        std::vector<uint32_t>::const_iterator begin() const;
        std::vector<uint32_t>::const_iterator end() const;

        bool isEmpty() const;
        std::size_t size() const;
        uint32_t& at(const std::size_t& index);

        void append(const String32& other);
        void append(char32_t* text, const std::size_t& max = npos);
        void append(const std::vector<uint32_t>& newData);
        void append(const char32_t& c);
        void append(const uint32_t& i);

        String32& operator += (const String32& other);
        String32& operator += (char32_t* other);
        String32& operator += (const std::vector<uint32_t>& other);
        String32& operator += (const char32_t& other);
        String32& operator += (const uint32_t& other);

        String32 operator + (const String32& other) const;
        String32 operator + (char32_t* other) const;
        String32 operator + (const std::vector<uint32_t>& other) const;
        String32 operator + (const char32_t& c) const;
        String32 operator + (const uint32_t& i) const;

        String32& operator = (const String32& other);
        String32& operator = (char32_t* other);
        String32& operator = (const std::vector<uint32_t>& other);
        String32& operator = (const char32_t& c);
        String32& operator = (const uint32_t& i);
};