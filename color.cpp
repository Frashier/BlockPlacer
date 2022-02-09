#include "color.h"

Color::Color() : red(255), green(255), blue(255) {};

Color::Color(unsigned char red_, unsigned char green_, unsigned char blue_) :
    red(red_), green(green_), blue(blue_) {};

bool Color::operator==(Color c)
{
    return (this->red == c.red && this->green == c.green && this->blue == c.blue);
}

Color Color::operator+(double n)
{
    return Color(this->red + n, this->green + n, this->blue + n);
}

Color Color::operator+ (Color c)
{
    return Color(this->red + c.red, this->green + c.green, this->blue + c.blue);
}

Color Color::operator* (double n)
{
    Color c;

    // checking whether parameters are in range
    // anything over 254 always gives us 255
    // anything under 0 gives us 0

    if (this->red * n > 254 || this->red * n < 0)
    {
        c.red = (int) this->red * n > 254 ? 255 : 0;
    }
    else
    {
        c.red = this->red * n;
    }

    if (this->green * n > 254 || this->green * n < 0)
    {
        c.green = (int) this->green * n > 254 ? 255 : 0;
    }
    else
    {
        c.green = this->green * n;
    }

    if (this->blue * n > 254 || this->blue * n < 0)
    {
        c.blue = (int) this->blue * n > 254 ? 255 : 0;
    }
    else
    {
        c.blue = this->blue * n;
    }

    return c;
}
