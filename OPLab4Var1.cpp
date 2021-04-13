#include <iostream>
#include <vector>
#include <fstream>
#include "bmpStruct.h"
#include "pixelStruct.h"
#include "Writer.h"
#include "Reader.h"
using namespace std;

int main()
{
    double k = 4.5;
    Reader reader;
    Writer writer;
    Image image = reader.readImageFromFile("bmp.bmp");
    Image newImage = image.interpolation(k);
    cout << "Enlarged!!!" << endl;
    writer.writeImageToFile("out.bmp", newImage);
    return 0;
}