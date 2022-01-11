
#pragma once

class Frame
{
  public:
    Frame() {}
    Frame(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
    int width;
    int height;
};
