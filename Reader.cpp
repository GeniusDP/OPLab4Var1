#include <fstream>
#include "BMP_HEADER_STRUCT.h"
#include "Picture.h"
#include <vector>
#include "Pixel.h"
#include "PIXEL_STRUCT.h"
#include "Reader.h"

void Reader::_read(Picture& image) {
    BMP fileHeader;
    /*opening file with picture*/
    std::ifstream in;
    in.open(m_fileName, std::ios_base::binary);

    in.read((char*)&fileHeader, sizeof(fileHeader));//reading header

    image.setHeader(fileHeader);
    image.setPadding((4 - image.getHeader().width * sizeof(PIXEL) % 4) % 4);
    in.seekg(image.getHeader().headersize, ios::beg);

    std::vector< std::vector<Pixel> > px(image.getHeader().height, std::vector<Pixel>(image.getHeader().width));
    //reading pixels
    char str[3];
    for (int i = 0; i < image.getHeader().height; i++) {
        for (int j = 0; j < image.getHeader().width; j++) {
            PIXEL temp;
            in.read((char*)&temp, sizeof(PIXEL));
            px[i][j].setPx(temp);
        }
        in.read(str, image.getPadding());
    }
    image.setPixels(px);
    in.close();
}
