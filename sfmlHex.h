#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
class HexagonShape : public ConvexShape
{
    public:
        HexagonShape(float radius=50)
        {
            setPointCount(6);
            for (int i=0;i<6;i++)
            {
                float angle=i*2*M_PI/6;
                setPoint(i,Vector2f(radius*cos(angle),radius*sin(angle)));
            }
        }
};