#include <iostream>
#include <vector>
#include <fstream>
#include "bmpStruct.h"
#include "pixelStruct.h"
#include "Writer.h"
#include "Reader.h"
using namespace std;

int main(int argc, char* argv[])
{
    double k = atof(argv[1]);
    char* originFile = argv[2];
    char* newFile = argv[3];
    cout << k << endl;
    cout << originFile << endl;
    cout << newFile << endl;
    Reader reader;
    Writer writer;
    Image image = reader.readImageFromFile(originFile);
    Image newImage = image.interpolation(k);
    cout << "Enlarged!!!" << endl;
    writer.writeImageToFile(newFile, newImage);
    return 0;
}