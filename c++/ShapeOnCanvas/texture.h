#pragma once
#include <cassert>
#include "common.h"

class Texture {
public:
    Texture(Image image) : image_(move(image)) {
        for (const auto& line : image_) {
            assert(line.size() == image_[0].size());
        }
    }

    Size GetSize() const 
    {
        auto width = static_cast<int>(image_.empty() ? 0 : image_[0].size());
        auto height = static_cast<int>(image_.size());
        return { width, height };
    }

    char GetPixelColor(Point p) const {
        if (PositionInsideTexture(p))
            return image_.at(p.y).at(p.x);
        else
            return '.';
    }

    const Image& GetImage() const {
        return image_;
    }

private:
    Image image_;

    bool PositionInsideTexture(const Point& p) const {
        return (0 <= p.y && p.y < static_cast<int>(image_.size())) &&
            (0 <= p.x && p.x < static_cast<int>(image_[0].size())) ? true : false;
    }
};