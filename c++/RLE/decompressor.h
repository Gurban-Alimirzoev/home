#pragma once
#include <fstream>
#include <iostream>
#include <string>

// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name)
{
    std::ifstream input(src_name, std::ios::binary);
    if (!input) {
        return false;
    }

    std::ofstream out(dst_name, std::ios::binary);
    do
    {
        int c = input.get();
        if (c == std::istream::traits_type::eof())
            return true;

        unsigned char header(c);
        int block_type = (header & 1);
        int data_size = (header >> 1) + 1;

        if (block_type == 0) {
            char buff[1024];
            input.read(buff, data_size);
            out.write(buff, data_size);
        }
        if (block_type == 1) {
            const char c = input.get();
            std::string str(data_size, c);
            out << str;
        }
    }
    while (input);
    return true;
}
