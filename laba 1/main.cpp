#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

int WIDTH = 1280,
HEIGHT = 720,
OX = WIDTH/2,
OY = HEIGHT/2,
dx = 1,
dy = 1;

Vertex OXLine[] = {
    Vertex(Vector2f(0, OY)),
    Vertex(Vector2f(WIDTH, OY)),
};

Vertex OYLine[] = {
    Vertex(Vector2f(OX, 0)),
    Vertex(Vector2f(OX, HEIGHT)),
};

class Tangents {
public:
    double a;
    double b;
    double c;

    Tangents(double a, double b, double c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }
};

vector<Tangents> findTangent(double R1, double R2, double x1, double y1, double x2, double y2) {
    double a, b, c;

    vector<Tangents> result;

    int index = 0;
    for (int minusR1 = -1; minusR1 <= 1; minusR1 += 2)
        for (int minusR2 = -1; minusR2 <= 1; minusR2 += 2) {
            for (int minusD = -1; minusD <= 1; minusD += 2) {

                double aCoef = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1),
                    bCoef = 2 * (minusR1 * R1 - minusR2 * R2) * (y2 - y1),
                    cCoef = (minusR2 * R2 - minusR1 * R1) * (minusR2 * R2 - minusR1 * R1) - (x2 - x1) * (x2 - x1);

                double D = bCoef * bCoef - 4 * aCoef * cCoef;

                b = (-bCoef + minusD * sqrt(D)) / (2 * aCoef);
                a = sqrt(1 - b * b);
                c = minusR2 * R2 - sqrt(1 - b * b) * x2 - b * y2;

                result.push_back(Tangents(a, b, c));
            }
        }

    return result;
}

Vector2f findThePoint(Tangents tan, double x1, double y1, double R1) {
    double a = tan.a,
        b = tan.b,
        c = tan.c;

    double aCoef, bCoef, cCoef, x, D;

    aCoef = 1 + a * a / (b * b);
    bCoef = -2 * x1 + 2 * a * c / (b * b) + 2 * a * y1 / b;
    cCoef = x1 * x1 + c * c / (b * b) + 2 * c * y1 / b + y1 * y1 - R1 * R1;
    D = bCoef * bCoef - 4 * aCoef * cCoef;

    if (abs(D) <= 160) D = 0;
    else  return Vector2f(0, 0);

    if (D < 0)  return Vector2f(0, 0);

    x = (-bCoef + sqrt(D)) / (2 * aCoef);

    return Vector2f(x, (-a * x - c) / b);
}

bool isInternal(Tangents t1, Tangents t2, double x1, double y1, double x2, double y2) {
    double crossedX = -(t1.c / t1.b - t2.c / t2.b) / (t1.a / t1.b - t2.a / t2.b);
    double crossedY = -t1.c / t1.b - crossedX * t1.a / t1.b;

    double xMax = max(x1, x2);
    double xMin = min(x1, x2);
    double yMax = max(y1, y2);
    double yMin = min(y1, y2);

    return !(crossedX >= xMin && crossedX <= xMax || crossedY >= yMin && crossedY <= yMax);
}

bool print = true;
static int index = 0;

int main()
{
    RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Circle Tangents");
    Clock clock;

    double R1 = 45;
    double R2 = 38;
    double X1 = 114;
    double Y1 = 97;
    double X2 = 198;
    double Y2 = -105;

    CircleShape circle1(R1), circle2(R2);

    circle1.setPosition(Vector2f(OX + X1 - R1, OY - Y1 - R1));
    circle2.setPosition(Vector2f(OX + X2 - R2, OY - Y2 - R2));

    circle1.setFillColor(Color::Black);
    circle2.setFillColor(Color::Black);

    circle1.setOutlineColor(Color::White);
    circle2.setOutlineColor(Color::White);

    circle1.setOutlineThickness(1);
    circle2.setOutlineThickness(1);

    while (window.isOpen())
    {
        Time deltaTime = clock.restart();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        Vector2f circle1Pos = circle1.getPosition();
        Vector2f circle2Pos = circle2.getPosition();

        if (Keyboard::isKeyPressed(Keyboard::Up))
            circle1Pos.y -= dy;
        if (Keyboard::isKeyPressed(Keyboard::Down))
            circle1Pos.y += dy;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            circle1Pos.x -= dx;
        if (Keyboard::isKeyPressed(Keyboard::Right) )
            circle1Pos.x += dx;
        if (Keyboard::isKeyPressed(Keyboard::W))
            circle2Pos.y -= dy;
        if (Keyboard::isKeyPressed(Keyboard::S))
            circle2Pos.y += dy;
        if (Keyboard::isKeyPressed(Keyboard::A))
            circle2Pos.x -= dx;
        if (Keyboard::isKeyPressed(Keyboard::D))
            circle2Pos.x += dx;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
            circle1.setRadius(R1 += 0.01);
        if (Keyboard::isKeyPressed(Keyboard::LControl) && R1 > 0)
            circle1.setRadius(R1 -= 0.01);
        if (Keyboard::isKeyPressed(Keyboard::RShift))
            circle2.setRadius(R2 += 0.01);
        if (Keyboard::isKeyPressed(Keyboard::RControl) && R2 > 0)
            circle2.setRadius(R2 -= 0.01);

        circle1.setPosition(circle1Pos);
        circle2.setPosition(circle2Pos);

        X1 = circle1Pos.x - OX + R1;
        Y1 = -(circle1Pos.y - OY + R1);
        X2 = circle2Pos.x - OX + R2;
        Y2 = -(circle2Pos.y - OY + R2);

        window.clear();
        window.draw(circle1);
        window.draw(circle2);
        window.draw(OXLine, 2, Lines);
        window.draw(OYLine, 2, Lines);

        vector<Tangents> allTangents = findTangent(R1, R2, X1, Y1, X2, Y2);
        vector<Tangents> needed;
        vector<Vertex*> points;

        for (int i = 0; i < allTangents.size(); i++) {
            Vertex touchPoint1 = findThePoint(allTangents[i], X1, Y1, R1);
            Vertex touchPoint2 = findThePoint(allTangents[i], X2, Y2, R2);

            if (touchPoint1.position.x == 0 && touchPoint1.position.y == 0 || touchPoint2.position.x == 0 && touchPoint2.position.y == 0)
                continue;

            touchPoint1.position.x += OX;
            touchPoint1.position.y = OY - touchPoint1.position.y;

            touchPoint2.position.x += OX;
            touchPoint2.position.y = OY - touchPoint2.position.y;

            Vertex* line = new Vertex[2];
            line[0] = touchPoint1, line[1] = touchPoint2;

            needed.push_back(allTangents[i]);
            points.push_back(line);
        }

        for (int i = 0; i < needed.size(); i++) {
            for (int j = i + 1; j < needed.size(); j++) {
                if (isInternal(needed[i], needed[j], X1, Y1, X2, Y2)) {
                    window.draw(points[i], 2, Lines);
                    window.draw(points[j], 2, Lines);
                }
            }
        }

        for (int i = 0; i < points.size(); i++)
            delete[] points[i];

        window.display();
    }

    return 0;
}
