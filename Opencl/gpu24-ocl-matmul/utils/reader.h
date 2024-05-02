#ifndef OCL1_READER_H
#define OCL1_READER_H

#include <string>
#include <cstdio>
#include <format>

using namespace std;

string read_file(string path_of_file) {
    string file;
    FILE *program_source = fopen(path_of_file.data(), "rb");
    if (program_source == nullptr) {
        throw runtime_error(format("Opencl file with name [{}] doesn't open!", path_of_file));
    }
    fseek(program_source, 0, SEEK_END);
    size_t size = ftell(program_source);
    rewind(program_source);
    file.resize(size);
    fread(file.data(), sizeof(char), size, program_source);
    fclose(program_source);
    return file;
}

#endif
