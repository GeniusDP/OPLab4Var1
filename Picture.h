#pragma once
#include <vector>
#include "BMP_HEADER_STRUCT.h"
#include "Pixel.h"
#include "Picture.h"
#include "PIXEL_STRUCT.h"
using namespace std;

/*Picture*/
class Picture {
    BMP m_pictureHeader;
    int m_padding;
    vector< vector<Pixel> > m_pixel;
public:
    /*getters*/
    BMP getHeader();
    int getPadding();
    std::vector< std::vector<Pixel> >& getPixels();

    /*setters*/
    void setHeader(BMP Header);
    void setPadding(int paddingValue);
    void setPixels(std::vector< std::vector<Pixel> > pixelsToPut);

    /*other methods*/
    Picture interpolate(double times);
};
