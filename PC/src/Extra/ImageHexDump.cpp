#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#include "Resources.hpp"

using namespace std;
using namespace filesystem;

bool dumpImages(const path& imagePath, const path& includePath) {
    path imageFile(includePath);
    imageFile.append("Images.hpp");

    path indexFile(imagePath);
    indexFile.append("Index.txt");
    int index = readIndexFile(indexFile);

    directory_iterator it(imagePath);
    int count = count_if(
        begin(it),
        end(it),
        [](const directory_entry& entry) {return entry.is_regular_file() && entry.path().extension() == ".png";}
    );

    if (index == count) return true; //Die Anzahl der Fonts ist gleich, wird übersprungen


    vector<bool> res;
    count = 0;

    ofstream file(imageFile);
    if (!file) return false;

    file << "/*Automatisch generiert*/" << endl;
    file << "#pragma once" << endl << endl;
    file << "namespace Image {" << endl;
    file.close();

    current_path(imagePath);

    for (const directory_entry& f: directory_iterator(imagePath)) {
        string t = f.path().extension().string();
        if (f.is_regular_file() && f.path().extension() == ".png") {
            res.push_back(processFile(f.path(), imageFile));
            count++;
        }
    }

    file.open(imageFile, ios_base::openmode::_S_app);
    if (!file) return false;

    file << "}" << endl;
    file.close();

    //Ausgaben Checken
    for (const bool& b: res) if (!b) return false;

    //Indexdatei schreiben
    writeIndexFile(indexFile, count);

    return true;
}

bool processFile(const path& in, const path& out, const bool& firstItem) {
    ostringstream str;
    str << "xxd -i \"" << in.filename().string() << "\"";
    if (firstItem) str << " > \"";
    else str << " >> \"";
    str << out.string() << "\"";

    string cmd = str.str();

    return !system(cmd.c_str());
}