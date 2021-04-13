#include "PIXEL_STRUCT.h"
#include <string>
#include "Picture.h"
#include "BMP_HEADER_STRUCT.h"
#include <vector>
#include "Printer.h"
#include <fstream>



Printer::Printer(std::string file) {
    m_fileName = file;
}

void Printer::_print(Picture image) {
    std::ofstream out;
    out.open(m_fileName, std::ios_base::binary);
    BMP header = image.getHeader();
    out.write((char*)&header, sizeof(BMP));
    std::vector< std::vector<Pixel> > px = image.getPixels();
    char str[3];
    for (int i = 0; i < image.getHeader().height; i++) {
        for (int j = 0; j < image.getHeader().width; j++) {
            PIXEL temp = px[i][j].getPx();
            out.write((char*)&temp, sizeof(PIXEL));
        }
        out.write(str, image.getPadding());
    }
    out.close();
}
