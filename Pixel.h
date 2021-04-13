#pragma once
#pragma once
#include "PixelStruct.h"
/*Pixel*/
class Pixel {
    PIXEL m_px;
public:
    void setPx(PIXEL val) {
        m_px = val;
    }
    PIXEL getPx() {
        return m_px;
    }
    unsigned char getR() {
        return m_px.R;
    }
    unsigned char getG() {
        return m_px.G;
    }
    unsigned char getB() {
        return m_px.B;
    }
};

