#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include "Resources.hpp"

using namespace std;
using namespace filesystem;

enum ShaderType : unsigned char {None, Vertex, Fragment};

struct ShaderInfo {
    string name, code;
    ShaderType type;
    bool success = false;
};

vector<ShaderInfo> shader;

bool processFile(const path& pfad);

bool readShader(const path& shaderPath, const path& includePath) {
    path shaderFile(includePath);
    shaderFile.append("ShaderCode.hpp");

    vector<bool> success;

    for (const directory_entry& res: directory_iterator(shaderPath)) {
        if (res.is_regular_file() && res.path().extension() == ".shader") {
            success.push_back(processFile(res.path()));
        }
    }

    //Shader in Header schreiben
    ofstream file(shaderFile);
    if (!file) return false;

    file << "/*Automatisch generiert*/" << endl;
    file << "#pragma once" << endl;
    file << "#include <string>" << endl << endl;
    file << "namespace ShaderCode {" << endl;

    for (const ShaderInfo& info: shader) {
        if (!info.success) continue;

        file << "static const std::string ";
        file << info.name << " = \"";
        file << info.code << "\";" << endl;
    }

    file << "}" << endl;

    file.close();

    //Testen, ob ein Shader fehlerhaft war
    for (const bool& b: success) if (!b) return false;

    return true;
}

bool processFile(const path& pfad) {
    ifstream file(pfad);
    if (!file) return false;

    istringstream str;
    string line, cmd, newType;

    vector<string> vertexCode, fragmentCode;
    ShaderType type = None;

    while (getline(file, line)) {
        if (line.starts_with("#shader")) {
            str.str(line);
            str >> cmd >> newType;
            str.clear();

            if (newType == "vertex") type = Vertex;
            else if (newType == "fragment") type = Fragment;
            else type = None;

            continue;
        }

        if (type == Vertex) vertexCode.push_back(line);
        else if (type == Fragment) fragmentCode.push_back(line);
    }

    file.close();

    //Vertex Code verarbeiten
    ostringstream out;
    ShaderInfo info;
    if (!vertexCode.empty()) {
        for (const string& s: vertexCode) {
            out << s << "\\n";
        }

        info.code = out.str();
        info.success = true;
        info.type = Vertex;

        out.clear();
        out.str("");
        out << pfad.stem().string() << "Vertex";
        info.name = out.str();
        out.clear();
        out.str("");

        shader.push_back(info);
    }

    //Fragement Code verarbeiten
    if (!fragmentCode.empty()) {
        for (const string& s: fragmentCode) {
            out << s << "\\n";
        }

        info.code = out.str();
        info.success = true;
        info.type = Fragment;

        out.clear();
        out.str("");
        out << pfad.stem().string() << "Fragment";
        info.name = out.str();
        out.clear();

        shader.push_back(info);
    }

    return true;
}