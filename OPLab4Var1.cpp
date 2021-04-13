#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "BMP_HEADER_STRUCT.h"
#include "PIXEL_STRUCT.h"
#include "Pixel.h"
#include "Reader.h"
#include "Picture.h"
#include "Printer.h"

using namespace std;


int main(int argc, char* argv[]) {
    double times = atof(argv[3]);
    Picture existingImage;
    Picture enlargedImage;
    Reader newReader((string)argv[1]);//"bmp.bmp"
    Printer newPrinter((string)argv[2]);//"output.bmp"

    newReader._read(existingImage);

    enlargedImage = existingImage.interpolate(times);

    newPrinter._print(enlargedImage);
    cerr << "Done!";
    return 0;
}