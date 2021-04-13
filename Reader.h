#pragma once
#include <string>
#include "Picture.h"

/*Reader*/
class Reader {
    std::string m_fileName;
public:
    Reader(std::string file) :m_fileName{ file } {};
    void _read(Picture& image);
};
