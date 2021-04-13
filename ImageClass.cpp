#include <stack>
#include "ImageClass.h" 

Image::Image(BMP Header, vector< vector< PIXEL > >& Map, int Padding) {
    header = Header;
    map.resize(Map.size());
    for (int i = 0; i < Map.size(); i++) {
        map[i].resize(Map[i].size());
        for (int j = 0; j < Map[i].size(); j++)
            map[i][j] = Map[i][j];
    }
    padding = Padding;
}
BMP Image::getHeader() {
    return header;
}
int Image::getPadding() {
    return padding;
}
vector< vector< PIXEL > > Image::getImage() {
    return map;
}

struct Position {
    int begin, end;
};

Image Image::interpolation(double k) {
    int newWidth = int(double(header.width) * k);
    int newDepth = int(double(header.depth) * k);
    int newPadding = (4 - newWidth * sizeof(PIXEL) % 4) % 4;
    int newFileSize = newWidth * newDepth * sizeof(PIXEL) + newDepth * newPadding + sizeof(BMP);
    BMP newHeader = header;
    newHeader.filesize = newFileSize;
    newHeader.width = newWidth;
    newHeader.depth = newDepth;
    vector< vector< PIXEL > > newMap(newDepth);
    vector< vector< bool > > calculated(newDepth);
    for (int i = 0; i < newDepth; i++) {
        newMap[i].resize(newWidth);
        calculated[i].resize(newWidth, false);
    }
    if (k < 1) {
        for (int i = 0; i < newDepth; i++) {
            for (int j = 0; j < newWidth; j++) {
                newMap[i][j] = map[int(i / k)][int(j / k)];
            }
        }
        for (int i = 0; i < newWidth; i++) {
            newMap[newDepth - 1][i] = map[header.depth - 1][int(i / k)];
        }
        for (int i = 0; i < newDepth; i++) {
            newMap[i][newWidth - 1] = map[int(i / k)][header.depth - 1];
        }
    }
    else {
        int D = map.size(), W = map[0].size();
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < W; j++) {
                newMap[int(k * i)][int(k * j)] = map[i][j];
                calculated[int(k * i)][int(k * j)] = true;
            }
            newMap[int(k * i)][int(W * k) - 1] = map[i][W - 1];
            calculated[int(k * i)][int(W * k) - 1] = true;
        }
        for (int i = 0; i < W; i++) {
            newMap[int(D * k) - 1][int(k * i)] = map[D - 1][i];
            calculated[int(D * k) - 1][int(k * i)] = true;
        }
        newMap[int(D * k) - 1][int(W * k) - 1] = map[D - 1][W - 1];
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
    Image newImage(newHeader, newMap, newPadding);
    return newImage;
}


