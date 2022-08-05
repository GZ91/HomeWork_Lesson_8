#include "IOFile.h"
#include <string>
#include <iostream>

void reading_file(std::istream &stream, std::string &text) {
    text = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
}
