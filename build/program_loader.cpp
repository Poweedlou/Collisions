#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Types.h"
#include <iostream>

extern "C" struct program loads(const char * filename)
{
    std::fstream file (filename);
    std::vector<char *> source;
    char * buffer;
    int len;
    while (!file.eof())
    {
        std::string buffer;
        std::getline(file, buffer, '\n');
        len = buffer.length();
        
        char* line = (char*) malloc(len + 2);
        for (int i = 0; i < len; i++)
            line[i] = buffer[i];
        line[len] = '\n';
        len += 1;
        line[len] = '\0';
        source.push_back(line);
    }
    len = source.size();
    char** sous = (char **) malloc(len * sizeof(char *));
    for (int i = 0; i < len; i++)
        sous[i] = source[i];
    program ret = {sous, len};
    return ret;
}