#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Resources.hpp"

using namespace std;
using namespace filesystem;

bool checkMainPath();

path fontPath, includePath, shaderPath, imagePath;

int main() {
    if (!checkMainPath()) {
        cerr << "Konnte Hauptpfad nicht finden. Breche ab" << endl;
        return 1;
    }

    cout << "Dumpe Fonts" << flush;
    bool fontSuccess = dumpFonts(fontPath, includePath);
    if (fontSuccess) cout << " -- Erfolgreich" << endl;
    else cout << " -- Fehlgeschlagen" << endl;

    cout << "Dumpe Bilder" << flush;
    bool imageSuccess = dumpImages(imagePath, includePath);
    if (imageSuccess) cout << " -- Erfolgreich" << endl;
    else cout << " -- Fehlgeschlagen" << endl;

    cout << "Lese Shader" << flush;
    bool shaderSuccess = readShader(shaderPath, includePath);
    if (shaderSuccess) cout << " -- Erfolgreich" << endl;
    else cout << " -- Fehlgeschlagen" << endl;
}

bool checkMainPath() {
    path temp = current_path();
    bool foundInclude = false, foundRes = false, foundFont = false, foundShader = false, foundImage = false;

    for (int i = 0; i < 4; i++) {
        for (const directory_entry& entry: directory_iterator(temp)) {
            if (entry.is_directory() && entry.path().filename() == "res") { //Resource Ordner

                for (const directory_entry& res: directory_iterator(entry)) {
                    if (res.is_directory() && res.path().filename() == "font") {
                        fontPath = res.path();
                        foundFont = true;
                        continue;
                    }

                    if (res.is_directory() && res.path().filename() == "shader") {
                        foundShader = true;
                        shaderPath = res.path();
                        continue;
                    }

                    if (res.is_directory() && res.path().filename() == "image") {
                        foundImage = true;
                        imagePath = res.path();
                        continue;
                    }
                }

                foundRes = foundFont && foundImage && foundShader;
            }

            if (entry.is_directory() && entry.path().filename() == "include") {
                foundInclude = true;
                includePath = entry.path();
                continue;
            }
        }

        if (foundInclude && foundRes) return true;

        foundFont = false;
        foundImage = false;
        foundInclude = false;
        foundRes = false;
        foundShader = false;

        temp = temp.parent_path();
    }

    return false;
}