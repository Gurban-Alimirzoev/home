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
        SetPosition({});
        SetSize({});
        // Заглушка. Реализуйте конструктор самостоятельно
        (void) type;
    }

    void SetPosition(Point pos) {
        zero = pos;
        // Заглушка. Реализуйте метод самостоятельно
    }

    void SetSize(Size size) {
        (void) size;
        // Заглушка. Реализуйте метод самостоятельно
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_shape = std::make_shared<Texture>(texture);
        // Заглушка. Реализуйте метод самостоятельно
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        texture_shape

        (void) image;
        // Заглушка. Реализуйте метод самостоятельно
    }

    std::shared_ptr<Texture> texture_shape;
    Point zero;
};