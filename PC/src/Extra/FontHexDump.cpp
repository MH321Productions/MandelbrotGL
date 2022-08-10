#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <fstream>
#include <algorithm>

#include "Resources.hpp"

using namespace std;
using namespace filesystem;

bool dumpFonts(const path& fontPath, const path& includePath) {
    //return true;
    path fontFile(includePath);
    fontFile.append("Fonts.hpp");

    path indexFile(fontPath);
    indexFile.append("Index.txt");
    int index = readIndexFile(indexFile);

    directory_iterator it(fontPath);
    int count = count_if(
        begin(it),
        end(it),
        [](const directory_entry& entry) {return entry.is_regular_file() && entry.path().extension() == ".ttf";}
    );

    if (index == count) return true; //Die Anzahl der Fonts ist gleich, wird übersprungen

    vector<bool> res;
    count = 0;

    ofstream file(fontFile);
    if (!file) return false;

    file << "/*Automatisch generiert*/" << endl;
    file << "#pragma once" << endl << endl;
    file << "namespace Fonts {" << endl;
    file.close();

    current_path(fontPath);

    for (const directory_entry& f: directory_iterator(fontPath)) {
        if (f.is_regular_file() && f.path().extension() == ".ttf") {
            res.push_back(processFile(f.path(), fontFile));
            count++;
        }
    }

    file.open(fontFile, ios_base::openmode::_S_app);
    if (!file) return false;

    file << "}" << endl;
    file.close();

    //Ausgaben Checken
    for (const bool& b: res) if (!b) return false;

    //Indexdatei ausgeben
    writeIndexFile(indexFile, count);

    return true;
}

int readIndexFile(const path& f) {
    ifstream file(f);
    if (!file) return 0;
    int result;
    file >> result;
    file.close();

    return result;
}

void writeIndexFile(const path& f, const int& count) {
    ofstream file(f);
    if (!file) return;
    file << count;
    file.close();
}