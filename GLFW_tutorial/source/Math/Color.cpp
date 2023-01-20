#include "Color.h"

Color::Color() :r(0), g(0), b(0), a(1.f) {}
Color::Color(const glm::vec3& color) :r(color.x), g(color.g), b(color.b), a(1.f) {}
Color::Color(const glm::vec4& color) :r(color.x), g(color.g), b(color.b), a(color.w) {}
Color::Color(float red, float green, float blue) :r(red), g(green), b(blue), a(1.f) {}
Color::Color(float red, float green, float blue, float alpha) :r(red), g(green), b(blue), a(alpha) {}
Color::Color(float factor) :r(factor), g(factor), b(factor), a(1.f) {}

Color Color::ColorU(const glm::uvec4& color) {
    return Color(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}
Color Color::Color::ColorU(byte red, byte green, byte blue, byte alpha) {
    return Color(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
}
Color Color::ColorU(byte red, byte green, byte blue) {
    return Color(red / 255.f, green / 255.f, blue / 255.f, 1.f);
}
Color Color::ColorU(const glm::uvec3& color) {
    return Color(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.f);
}

const Color Color::RED(1.f, 0.f, 0.f);
const Color Color::BLUE(0.f, 0.f, 1.f);
const Color Color::WHITE(1.f, 1.f, 1.f);
const Color Color::BLACK(0.f, 0.f, 0.f);
const Color Color::GREEN(0.f, 1.f, 0.f);
const Color Color::YELLOW(1.f, 1.f, 0.f);
const Color Color::PURPLE(1.f, 0.f, 1.f);
const Color Color::CYAN(0.f, 1.f, 1.f);

//bool operator ==(const Color& left, const Color& right) {
//    return ((left.r == right.r) &&
//        (left.b == right.b) &&
//        (left.g == right.g) &&
//        (left.a == right.a));
//}
//bool operator !=(const Color& left, const Color& right) {
//    return !(left == right);
//}