#include "BMP_HEADER_STRUCT.h"
#include "Picture.h"
#include <vector>
#include "Pixel.h"
#include "PIXEL_STRUCT.h"
#include <iostream>
#include <stack>

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
void Picture::setPIXELSForVlad(vector< vector<PIXEL> > pixelsToPut) {
    if (pixelsToPut.size()) {
        vector< vector<Pixel> > temp(pixelsToPut.size(), vector<Pixel>(pixelsToPut[0].size()));
        for (int i = 0; i < pixelsToPut.size(); i++) {
            for (int j = 0; j < pixelsToPut[0].size(); j++) {
                temp[i][j].setPx(pixelsToPut[i][j]);
            }
        }
        m_pixel = temp;
    }
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



/*

*/
Picture Picture::enlargerVlad(double k) {
    cerr << "Enlarging..." << endl;
    struct Position {
        int begin, end;
    };
    int newWidth = int(double(m_pictureHeader.width) * k);
    int newDepth = int(double(m_pictureHeader.height) * k);
    int newPadding = (4 - newWidth * sizeof(PIXEL) % 4) % 4;
    int newFileSize = newWidth * newDepth * sizeof(PIXEL) + newDepth * newPadding + sizeof(BMP);
    BMP newHeader = m_pictureHeader;
    newHeader.filesize = newFileSize;
    newHeader.width = newWidth;
    newHeader.height = newDepth;
    vector< vector< PIXEL > > newMap(newDepth);
    vector< vector< bool > > calculated(newDepth);
    for (int i = 0; i < newDepth; i++) {
        newMap[i].resize(newWidth);
        calculated[i].resize(newWidth, false);
    }
    if (k < 1) {
        for (int i = 0; i < newDepth; i++) {
            for (int j = 0; j < newWidth; j++) {
                newMap[i][j] = m_pixel[int(i / k)][int(j / k)].getPx();
            }
        }
        for (int i = 0; i < newWidth; i++) {
            newMap[newDepth - 1][i] = m_pixel[m_pictureHeader.height - 1][int(i / k)].getPx();
        }
        for (int i = 0; i < newDepth; i++) {
            newMap[i][newWidth - 1] = m_pixel[int(i / k)][m_pictureHeader.height - 1].getPx();
        }
    }
    else {
        int D = m_pixel.size(), W = m_pixel[0].size();
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < W; j++) {
                newMap[int(k * i)][int(k * j)] = m_pixel[i][j].getPx();
                calculated[int(k * i)][int(k * j)] = true;
            }
            newMap[int(k * i)][int(W * k) - 1] = m_pixel[i][W - 1].getPx();
            calculated[int(k * i)][int(W * k) - 1] = true;
        }
        for (int i = 0; i < W; i++) {
            newMap[int(D * k) - 1][int(k * i)] = m_pixel[D - 1][i].getPx();
            calculated[int(D * k) - 1][int(k * i)] = true;
        }
        newMap[int(D * k) - 1][int(W * k) - 1] = m_pixel[D - 1][W - 1].getPx();
        calculated[int(D * k) - 1][int(W * k) - 1] = true;
        for (int i = 0; i < newDepth; i++) {
            int j = 0;
            if (!calculated[i][j])
                continue;
            while (j < newWidth - 1) {
                int next = j + 1;
                while (next < newWidth && !calculated[i][next])
                    next++;
                if (next == newWidth)
                    next--;
                stack< Position > stack;
                stack.push({ j, next });
                while (!stack.empty()) {
                    Position pos = stack.top();
                    stack.pop();
                    int mid = (pos.begin + pos.end) / 2;
                    if (calculated[i][mid])
                        continue;
                    newMap[i][mid].R = (newMap[i][pos.begin].R + newMap[i][pos.end].R) / 2;
                    newMap[i][mid].G = (newMap[i][pos.begin].G + newMap[i][pos.end].G) / 2;
                    newMap[i][mid].B = (newMap[i][pos.begin].B + newMap[i][pos.end].B) / 2;
                    calculated[i][mid] = true;
                    stack.push({ pos.begin, mid });
                    stack.push({ mid, pos.end });
                }
                j = next;
            }
        }
        for (int i = 0; i < newWidth; i++) {
            int j = 0;
            while (j < newDepth - 1) {
                int next = j + 1;
                while (next < newDepth && !calculated[next][i])
                    next++;
                if (next == newDepth)
                    next--;
                stack< Position > stack;
                stack.push({ j, next });
                while (!stack.empty()) {
                    Position pos = stack.top();
                    stack.pop();
                    int mid = (pos.begin + pos.end) / 2;
                    if (calculated[mid][i])
                        continue;
                    newMap[mid][i].R = (newMap[pos.begin][i].R + newMap[pos.end][i].R) / 2;
                    newMap[mid][i].G = (newMap[pos.begin][i].G + newMap[pos.end][i].G) / 2;
                    newMap[mid][i].B = (newMap[pos.begin][i].B + newMap[pos.end][i].B) / 2;
                    calculated[mid][i] = true;
                    stack.push({ pos.begin, mid });
                    stack.push({ mid, pos.end });
                }
                j = next;
            }
        }
    }
    Picture newImage;
    newImage.setHeader(newHeader);
    newImage.setPadding(newPadding);
    newImage.setPIXELSForVlad(newMap);
    return newImage;
}
