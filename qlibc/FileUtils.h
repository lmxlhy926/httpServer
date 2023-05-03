//
// Created by 78472 on 2022/5/10.
//

#ifndef EXHIBITION_FILEUTILS_H
#define EXHIBITION_FILEUTILS_H

#include <string>

using namespace std;

#define FILE_CONNECTOR '/'

class FileUtils{
public:
    static bool fileExists(std::string& fileName);

    static std::string contactFileName(const std::string &dest, const std::string &fn);

};


#endif //EXHIBITION_FILEUTILS_H
