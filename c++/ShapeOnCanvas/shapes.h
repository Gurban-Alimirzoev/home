#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) {
        switch (type) {
        case ShapeType::RECTANGLE:
            sh_ = ShapeType::RECTANGLE;
            break;
        case ShapeType::ELLIPSE:
            sh_ = ShapeType::ELLIPSE;
            break;
        }
    }

    void SetPosition(Point pos) {
        zero = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = texture;
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image & image) const {
        for (int i = 0; i < size_.height; ++i) {
            for (int j = 0; j < size_.width; ++j) {
                if (sh_ == ShapeType::ELLIPSE && !IsPointInEllipse(Point{ j, i }, size_)) {
                    continue;
                }
                int y = i + zero.y;
                int x = j + zero.x;
                if (!(y < static_cast<int>(image.size()) && x < static_cast<int>(image[y].size()))) continue;
                if (texture_ && i < texture_->GetSize().height && j < texture_->GetSize().width) {
                    image[y][x] = texture_->GetImage()[i][j];
                }
                else {
                    image[y][x] = '.';
                }
            }
        }
    }
    std::shared_ptr<Texture> texture_{ nullptr };
    Point zero;
    Size size_;
    ShapeType sh_;
};