#pragma once
#include <string>
#include "Picture.h"

/*Printer*/
class Printer {
    std::string m_fileName;
public:
    Printer(std::string file);
    void _print(Picture image);
};
