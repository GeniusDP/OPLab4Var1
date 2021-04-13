#include "BMP_HEADER_STRUCT.h"
#include "Picture.h"
#include <vector>
#include "Pixel.h"
#include "PIXEL_STRUCT.h"
#include <iostream>

using namespace std;



void Picture::setHeader(BMP Header) {
    m_pictureHeader = Header;
}
BMP Picture::getHeader() {
    return m_pictureHeader;
}
void Picture::setPadding(int paddingValue) {
    m_padding = paddingValue;
}
int Picture::getPadding() {
    return m_padding;
}
void Picture::setPixels(vector< vector<Pixel> > pixelsToPut) {
    m_pixel = pixelsToPut;
}
vector< vector<Pixel> >& Picture::getPixels() {
    return m_pixel;
}


Picture Picture::interpolate(double times) {
    Picture newImage;
    cerr << "Enrarging..." << endl;
    BMP newHeader = m_pictureHeader;
    newHeader.width *= times;
    newHeader.height *= times;
    int padding = (4 - newHeader.width * sizeof(PIXEL) % 4) % 4;
    newHeader.filesize = newHeader.headersize + newHeader.width * newHeader.height * sizeof(PIXEL) + newHeader.height * padding;

    newImage.setPadding(padding);
    newImage.setHeader(newHeader);

    vector< vector<Pixel> > p(newImage.getHeader().height, vector<Pixel>(newImage.getHeader().height));
    double X, Y;
    double u, v;
    int truncX, truncY;
    for (int x = 0; x < newImage.getHeader().height; x++) {
        for (int y = 0; y < newImage.getHeader().width; y++) {
            X = x / times;
            Y = y / times;
            truncX = (int)X;
            truncY = (int)Y;
            u = X - truncX;
            v = Y - truncY;
            PIXEL curPixel = { 255,255,255 };
            if (truncX < this->getHeader().height - 1 && truncY < this->getHeader().width - 1) {
                //R
                curPixel.R = (1 - v) * (1 - u) * this->getPixels()[truncX][truncY].getR() + (1 - v) * u * this->getPixels()[truncX + 1][truncY].getR() +
                    v * (1 - u) * this->getPixels()[truncX][truncY + 1].getR() + u * v * this->getPixels()[truncX + 1][truncY + 1].getR();
                //G
                curPixel.G = (1 - v) * (1 - u) * this->getPixels()[truncX][truncY].getG() + (1 - v) * u * this->getPixels()[truncX + 1][truncY].getG() +
                    v * (1 - u) * this->getPixels()[truncX][truncY + 1].getG() + u * v * this->getPixels()[truncX + 1][truncY + 1].getG();
                //B
                curPixel.B = (1 - v) * (1 - u) * this->getPixels()[truncX][truncY].getB() + (1 - v) * u * this->getPixels()[truncX + 1][truncY].getB() +
                    v * (1 - u) * this->getPixels()[truncX][truncY + 1].getB() + u * v * this->getPixels()[truncX + 1][truncY + 1].getB();
            }
            else if (truncX > 0 && truncY > 0) {
                //R
                curPixel.R = (1 - v) * (1 - u) * this->getPixels()[truncX - 1][truncY - 1].getR() + (1 - v) * u * this->getPixels()[truncX][truncY - 1].getR() +
                    v * (1 - u) * this->getPixels()[truncX - 1][truncY].getR() + u * v * this->getPixels()[truncX][truncY].getR();
                //G
                curPixel.G = (1 - v) * (1 - u) * this->getPixels()[truncX - 1][truncY - 1].getG() + (1 - v) * u * this->getPixels()[truncX][truncY - 1].getG() +
                    v * (1 - u) * this->getPixels()[truncX - 1][truncY].getG() + u * v * this->getPixels()[truncX][truncY].getG();
                //B
                curPixel.B = (1 - v) * (1 - u) * this->getPixels()[truncX - 1][truncY - 1].getB() + (1 - v) * u * this->getPixels()[truncX][truncY - 1].getB() +
                    v * (1 - u) * this->getPixels()[truncX - 1][truncY].getB() + u * v * this->getPixels()[truncX][truncY].getB();
            }
            p[x][y].setPx(curPixel);
        }
    }
    newImage.setPixels(p);
    return newImage;
}


