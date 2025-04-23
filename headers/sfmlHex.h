#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
class HexagonShape : public ConvexShape
{
    float radius=70;
    Color color=Color::White;
    Color outline = Color::Black;
    public:
        HexagonShape(Color color=Color::White, Vector2f position = {0.f, 0.f}, Color outline=Color::Black)
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
            setOutlineColor(outline);
            setPosition(position);
        }
        Color getColor()
        {
            return color;
        }
        void changeOutlineColor(Color color)
        {
            outline=color;
            setOutlineColor(color);
        }
        bool contains(Vector2f point) const {
            // Transformarea punctului în coordonatele locale ale hexagonului
            Transform transform;
            transform.translate(getPosition());
            Vector2f localPoint = transform.getInverse().transformPoint(point);
        
            int n = getPointCount();
            bool inside = false;
        
            for (int i = 0, j = n - 1; i < n; j = i++) {
                Vector2f pi = getPoint(i);
                Vector2f pj = getPoint(j);
        
                if (((pi.y > localPoint.y) != (pj.y > localPoint.y)) &&
                    (localPoint.x < (pj.x - pi.x) * (localPoint.y - pi.y) / (pj.y - pi.y) + pi.x)) {
                    inside = !inside;
                }
            }
        
            return inside;
        }
        
};