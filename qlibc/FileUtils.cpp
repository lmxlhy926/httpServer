//
// Created by 78472 on 2022/5/10.
//

#include "FileUtils.h"
#include <fstream>

bool FileUtils::fileExists(string &fileName) {
    std::ifstream inFile(fileName, std::ios::in);
    return inFile.is_open();
}

std::string FileUtils::contactFileName(const string &dest, const string &fn) {
    if(dest.empty())
        return fn;
    else{
        const char c = dest[dest.size() - 1];
        if(c == FILE_CONNECTOR)
            return dest + fn;
        else
            return dest + FILE_CONNECTOR + fn;
    }
}

