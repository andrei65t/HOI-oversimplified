#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
class HexagonShape : public ConvexShape
{
    float radius=50;
    Color color=Color::White;
    public:
        HexagonShape(Color color=Color::White)
        {
            setPointCount(6);
            for (int i=0;i<6;i++)
            {
                float angle=i*2*M_PI/6;
                setPoint(i,Vector2f(radius*cos(angle),radius*sin(angle)));
            }
            this->color=color;
            setFillColor(color);
            setOutlineThickness(2);
            setOutlineColor(Color::Black);
        }
        Color getColor()
        {
            return color;
        }
};