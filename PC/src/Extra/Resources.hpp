#pragma once

#include <filesystem>

bool dumpFonts(const std::filesystem::path& fontPath, const std::filesystem::path& includePath);
bool dumpImages(const std::filesystem::path& imagePath, const std::filesystem::path& includePath);
bool readShader(const std::filesystem::path& shaderPath, const std::filesystem::path& includePath);
bool processFile(const std::filesystem::path& in, const std::filesystem::path& out, const bool& firstItem = false);
int readIndexFile(const std::filesystem::path& f);
void writeIndexFile(const std::filesystem::path& f, const int& count);