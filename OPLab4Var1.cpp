#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#pragma pack(push, 1)
struct BMP {
    int8_t id0;
    int8_t id1;// Завжди дві літери 'B' і 'M'
    int32_t filesize;        // Розмір файла в байтах!!!!!!!!!!!!!!!!!!!!!!
    int32_t reserved;     // 0, 0
    int32_t headersize;      // 54L для 24-бітних зображень
    int32_t infoSize;        // 40L для 24-бітних зображень
    int32_t width;           // ширина зображення в пікселях!!!!!!!!!!!!!!!!!!!!!!
    int32_t depth;           // висота зображення в пікселях!!!!!!!!!!!!!!!!!!!!!!)))
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
    char B;
    char G;
    char R;
};
#pragma pack(pop)



int main()
{
    int k = 1;
    BMP bmp, cpy;
    PIXEL px;
    ifstream in;
    ofstream out;
    in.open("bmp.bmp", ios::in | ios::binary); in.seekg(0, ios_base::beg);
    out.open("output.bmp", ios::out | ios::binary);

    char str[10] = {};
    char zero[10] = {};
    char pal[1024] = {};
    in.read((char*)&bmp, sizeof(bmp));
    //in.read((char*)&pal, 1024);//считываие палитры
    //in.seekg( bmp.headersize + 1024L, ios_base::beg);
    cpy = bmp;
    int pendingRead = (4 - bmp.width % 4) % 4;//(4 - (bmp.width * sizeof(PIXEL)) % 4) % 4;
    int pendingWrite = (4 - (bmp.width * k) % 4) % 4;//(4 - (sizeof(PIXEL) * k * bmp.width) % 4) % 4;
    out.seekp(0, ios_base::beg);
    bmp.filesize = sizeof(BMP) +
        sizeof(PIXEL) * bmp.width * bmp.depth * (k * k - 1) + pendingWrite * bmp.depth * k;
    bmp.width *= k;
    bmp.depth *= k;
    out.write((char*)&bmp, sizeof(BMP));
    bmp.width /= k;
    bmp.depth /= k;
    for (int r = 0; r < bmp.depth; r++) {
        vector<PIXEL> allLine(0);
        for (int j = 0; j < bmp.width; j++) {
            in.read((char*)&px, sizeof(px));
            allLine.push_back(px);
        }
        in.read(str, pendingRead);
        //вывод

        for (int cnt = 0; cnt < k; cnt++) {
            for (int i = 0; i < bmp.width; i++) {
                for (int j = 0; j < k; j++)
                    out.write((char*)&(allLine[i]), sizeof(PIXEL));
            }
            out.write(str, pendingWrite);
        }
    }


    out.close();
    in.close();
    return 0;
}