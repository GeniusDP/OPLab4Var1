#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct BMP {
    int8_t id0;
    int8_t id1;// ������ �� ����� 'B' � 'M'
    int32_t filesize;        // ����� ����� � ������!!!!!!!!!!!!!!!!!!!!!!
    int32_t reserved;     // 0, 0
    int32_t headersize;      // 54L ��� 24-����� ���������
    int32_t infoSize;        // 40L ��� 24-����� ���������
    int32_t width;           // ������ ���������� � �������!!!!!!!!!!!!!!!!!!!!!!
    int32_t depth;           // ������ ���������� � �������!!!!!!!!!!!!!!!!!!!!!!)))
    int16_t biPlanes;        // 1 (��� 24-����� ���������)
    int16_t bits;            // 24 (��� 24-����� ���������)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // ����� ��������� � 0L ��� ��������� ��� �������� (��� ������)
    int32_t biXPelsPerMeter; // ������������� ������� ������ �� ����, ����� 0L
    int32_t biYPelsPerMeter; // �� ����, �� �����
    int32_t biClrUsed;       // ��� ������������ ���������, ����� ��������� 0L
    int32_t biClrImportant;  // �� ����
};
#pragma pack(pop)