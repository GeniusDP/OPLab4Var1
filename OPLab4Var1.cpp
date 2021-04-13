#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#pragma pack(push, 1)
struct BMP {
    int8_t id0;
    int8_t id1;              // Завжди дві літери 'B' і 'M'
    int32_t filesize;        // Розмір файла в байтах!!!!!!!!!!!!!!!!!!!!!!
    int32_t reserved;        // 0, 0
    int32_t headersize;      // 54L для 24-бітних зображень
    int32_t infoSize;        // 40L для 24-бітних зображень
    int32_t width;           // ширина зображення в пікселях!!!!!!!!!!!!!!!!!!!!!!
    int32_t height;          // висота зображення в пікселях!!!!!!!!!!!!!!!!!!!!!!)))
    int16_t biPlanes;        // 1 (для 24-бітних зображень)
    int16_t bits;            // 24 (для 24-бітних зображень)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
    int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
    int32_t biYPelsPerMeter; // Те саме, по висоті
    int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
    int32_t biClrImportant;  // Те саме
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PIXEL {
    unsigned char B;
    unsigned char G;
    unsigned char R;
};
#pragma pack(pop)

PIXEL** setMemory(int rows, int cols) {
    PIXEL** matrix;
    matrix = new PIXEL * [rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new PIXEL[cols];
    }
    return matrix;
}

PIXEL** inputData(BMP& Header) {
    ifstream in;
    in.open("bmp.bmp", ios::binary);
    in.read((char*)&Header, sizeof(BMP)); in.seekg(Header.headersize, ios::beg);
    int paddingRead = (4 - Header.width * sizeof(PIXEL) % 4) % 4;
    PIXEL** matr = setMemory(Header.height, Header.width);

    char str[3];
    for (int i = 0; i < Header.height; i++) {
        in.read((char*)matr[i], Header.width * sizeof(PIXEL));
        in.read(str, paddingRead);
    }
    return matr;
}

void printData(BMP Header, PIXEL** newMatrix) {
    ofstream out;
    out.open("output.bmp", ios::binary);
    out.write((char*)&Header, sizeof(BMP));
    int paddingWrite = (4 - Header.width * sizeof(PIXEL) % 4) % 4;
    char str[3];
    for (int i = 0; i < Header.width; i++) {
        out.write((char*)newMatrix[i], Header.width * sizeof(PIXEL));
        out.write(str, paddingWrite);
    }
    cerr << "Done!";
}

void interpolation(PIXEL** startMatrix, BMP startFile, PIXEL** enlargedMatrix, BMP enlargedFile, double k) {
    double X, Y;
    double u, v;
    int truncX, truncY;
    for (int x = 0; x < enlargedFile.height; x++) {
        for (int y = 0; y < enlargedFile.width; y++) {
            X = x / k;
            Y = y / k;
            truncX = (int)X;
            truncY = (int)Y;
            u = X - truncX;
            v = Y - truncY;
            //swap(u, v);
            //cout << truncX << " " << truncY << endl;
            if (truncX < startFile.height - 1 && truncY < startFile.width - 1) {
                enlargedMatrix[x][y].R = (1 - v) * (1 - u) * startMatrix[truncX][truncY].R + (1 - v) * u * startMatrix[truncX + 1][truncY].R +
                    v * (1 - u) * startMatrix[truncX][truncY + 1].R + u * v * startMatrix[truncX + 1][truncY + 1].R;
                enlargedMatrix[x][y].G = (1 - v) * (1 - u) * startMatrix[truncX][truncY].G + (1 - v) * u * startMatrix[truncX + 1][truncY].G +
                    v * (1 - u) * startMatrix[truncX][truncY + 1].G + u * v * startMatrix[truncX + 1][truncY + 1].G;
                enlargedMatrix[x][y].B = (1 - v) * (1 - u) * startMatrix[truncX][truncY].B + (1 - v) * u * startMatrix[truncX + 1][truncY].B +
                    v * (1 - u) * startMatrix[truncX][truncY + 1].B + u * v * startMatrix[truncX + 1][truncY + 1].B;
            }
            else if (truncX > 0 && truncY > 0) {
                enlargedMatrix[x][y].R = (1 - v) * (1 - u) * startMatrix[truncX - 1][truncY - 1].R + (1 - v) * u * startMatrix[truncX][truncY - 1].R +
                    v * (1 - u) * startMatrix[truncX - 1][truncY].R + u * v * startMatrix[truncX][truncY].R;
                enlargedMatrix[x][y].G = (1 - v) * (1 - u) * startMatrix[truncX - 1][truncY - 1].G + (1 - v) * u * startMatrix[truncX][truncY - 1].G +
                    v * (1 - u) * startMatrix[truncX - 1][truncY].G + u * v * startMatrix[truncX][truncY].G;
                enlargedMatrix[x][y].B = (1 - v) * (1 - u) * startMatrix[truncX - 1][truncY - 1].B + (1 - v) * u * startMatrix[truncX][truncY - 1].B +
                    v * (1 - u) * startMatrix[truncX - 1][truncY].B + u * v * startMatrix[truncX][truncY].B;


            }

        }

    }

}

int main() {
    double k = 2.654;
    BMP startFile, enlargedFile;
    PIXEL px;
    PIXEL** startMatrix = inputData(startFile);
    PIXEL** enlargedMatrix;
    enlargedFile = startFile;
    enlargedFile.height *= k;
    enlargedFile.width *= k;
    int paddingWrite = (4 - enlargedFile.width * sizeof(PIXEL) % 4) % 4;
    enlargedFile.filesize = enlargedFile.headersize + enlargedFile.width * enlargedFile.height * sizeof(PIXEL) + enlargedFile.height * paddingWrite;

    //work space
    enlargedMatrix = setMemory(enlargedFile.height, enlargedFile.width);

    interpolation(startMatrix, startFile, enlargedMatrix, enlargedFile, k);
    //end of work space

    printData(enlargedFile, enlargedMatrix);
    return 0;
}