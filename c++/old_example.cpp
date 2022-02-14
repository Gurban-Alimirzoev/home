
    Wall::Wall(double width, double height) 
    : width_(width), height_(height), color_(Color::WHITE) 
{
}

enum class Color {
        BLUE,
        GREEN,
        RED,
        WHITE,
        YELLOW
    };
    
double Wall::GetHeight() const {
    return height_;
} 
double Wall::GetWidth() const {
    return width_;
} 
void Wall::SetColor(Color color) {
    color_ = color;
}
Color Wall::GetColor() const {
    return color_
}    
private:
    double width_;
    double height_;
    Color color_;    
