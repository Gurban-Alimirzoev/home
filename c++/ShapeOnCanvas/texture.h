#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    Size GetSize() const {
        int x = image_.size();
        int y = x == 0 ? 0 : image_[0].size();
        return {x, y};
    }

    char GetPixelColor(Point p) const {
        Size this_text = GetSize();
        if (p.x < this_text.height && p.y < this_text.width)
            return image_[p.x][p.y];
        else
            return '.';
    }

private:
    Image image_;
};