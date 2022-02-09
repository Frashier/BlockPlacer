#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    Color();

    Color(unsigned char, unsigned char,unsigned char);

    // RGB in unsigned char type (0-255)
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    bool operator==(Color);
    Color operator+(double);
    Color operator+(Color);
    Color operator*(double);
};

#endif // COLOR_H
