#include <fstream>
#include <string>
#include "ImageClass.h"
#include "Reader.h"

Image Reader::readImageFromFile(string fileName) {
    ifstream in(fileName, ios::binary);
    BMP header;
    in.read((char*)&header, sizeof(BMP));
    in.seekg(header.headersize, ios::beg);
    vector< vector< PIXEL > > map;
    map.resize(header.depth);
    int paddingRead = (4 - sizeof(PIXEL) * header.width % 4) % 4;
    char str[10];
    for (int i = 0; i < header.depth; i++) {
        for (int j = 0; j < header.width; j++) {
            PIXEL pixel;
            in.read((char*)&pixel, sizeof(PIXEL));
            map[i].push_back(pixel);
        }
        in.read(str, paddingRead);
    }
    Image image(header, map, paddingRead);
    in.close();
    return image;
}
