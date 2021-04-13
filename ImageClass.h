#pragma once
#include <vector>
#include "BmpStruct.h"
#include "PixelStruct.h"

using namespace std;

class Image {
    BMP header;
    vector< vector< PIXEL > > map;
    int padding;
public:
    Image(BMP Header, vector< vector< PIXEL > >& Map, int Padding);
    BMP getHeader();
    int getPadding();
    vector< vector< PIXEL > > getImage();
    Image interpolation(double k);
};
