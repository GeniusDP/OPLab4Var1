#include "Writer.h"
#include <fstream>
#include <string>
#include <vector>

void Writer::writeImageToFile(string fileName, Image image) {
    ofstream out(fileName, ios::binary);
    BMP header = image.getHeader();
    out.write((char*)&header, sizeof(BMP));
    vector< vector< PIXEL > > map = image.getImage();
    int padding = image.getPadding();
    char str[10];
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            out.write((char*)&map[i][j], sizeof(PIXEL));
        }
        out.write(str, padding);
    }
    out.close();
}
