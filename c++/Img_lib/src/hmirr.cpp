#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

// реализуйте горизонтальное отражение
void HMirrInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight(); ++y) {
        img_lib::Color* line = image.GetLine(y);
        int mid = image.GetWidth() / 2;

        for (int x = 0; x < mid; ++x) {
            swap(line[x], line[image.GetWidth() - x - 1]);
        }
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    HMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}