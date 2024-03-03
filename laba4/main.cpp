#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;

int OX = WIDTH / 2;
int OY = HEIGHT / 2;

Vertex OXLine[] = {
    Vertex(Vector2f(0, OY)),
    Vertex(Vector2f(WIDTH, OY)),
};

Vertex OYLine[] = {
    Vertex(Vector2f(OX, 0)),
    Vertex(Vector2f(OX, HEIGHT)),
};

vector<Vector2f> getСrossedPoints(Vector2f* window, int winSize, Vertex* segment) {

    vector<Vector2f> crossed;
    vector<int> inRec1, inRec2;

    for (int i = 0; i < winSize - 1; i++) {
        Vector2f v1 = window[i];
        Vector2f v2 = window[i + 1];       

        //y = kx + b - уравнение прямой стороны заданного прямоугольника
        if (segment[0].position.x > segment[1].position.x)
            swap(segment[0], segment[1]);

        Vector2f A = segment[0].position;
        Vector2f B = segment[1].position;

        int extra = 1;
        double k = (v2.x - v1.x != 0) ? (v2.y - v1.y) / (v2.x - v1.x) : 1e+10;
        double b = v1.y - k * v1.x;

        if (k == 1e+10)
            extra = 0, k = -1, b = v1.x;

        //принадлежность концов отрезка
        if (extra * A.y - k * A.x - b > 0)
            inRec1.push_back(i);
        if (extra * B.y - k * B.x - b > 0)
            inRec2.push_back(i);
        
        //границы отрезка
        float maxX = max(window[i].x, window[i + 1].x);
        float minX = min(window[i].x, window[i + 1].x);
        float maxY = max(window[i].y, window[i + 1].y);
        float minY = min(window[i].y, window[i + 1].y);

        //деление отрезка
        while (abs(A.x - B.x) > 1e-2 || abs(A.y - B.y) > 1e-2) {
            Vector2f mid = (A + B) * 0.5f;
            double answer = extra * mid.y - k * mid.x - b;
            double answerA = extra * A.y - k * A.x - b;
            double answerB = extra * B.y - k * B.x - b;

            if (abs(answerA) > abs(answerB))
                A = mid;
            else
                B = mid;
          
            if (abs(answer) < 1e-1 && mid.x >= minX - 1e-1 && mid.x <= maxX + 1e-1 && mid.y >= minY - 1e-1 && mid.y <= maxY + 1e-1) { 
                crossed.push_back(mid);
                break;
            }
        } 
    }

    if (inRec1.size() == 2 && abs(inRec1[0] - inRec1[1]) != 2)
        crossed.push_back(segment[0].position);
    if (inRec2.size() == 2 && abs(inRec2[0] - inRec2[1]) != 2)
        crossed.push_back(segment[1].position);
    if (crossed.size() == 1)
        crossed.push_back(crossed[0]);

    return crossed;
}

float dist = 0.2;

int main()
{
    RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Line break");
    Clock clock;

    //окно отсечения
    Vector2f win[] = {
        Vector2f(179 - 200, 221),
        Vector2f(119 - 200, 281),
        Vector2f(199 - 200, 341),
        Vector2f(259 - 200, 281),
        Vector2f(179 - 200, 221)
    };
      
    //вершины окна для отрисовки
    Vertex* winVert = new Vertex[5];
   
    //отрезки   
    int segmSize = 15;
    vector<Vertex*> segments;
    for (int i = 0; i < segmSize; i++) {
        Vertex* segmVert = new Vertex[2];
        segmVert[0].position = Vector2f(-OX + rand() % WIDTH, -OY + rand() % HEIGHT);
        segmVert[1].position = Vector2f(-OX + rand() % WIDTH, -OY + rand() % HEIGHT);
        segments.push_back(segmVert);
    }

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

        if (Keyboard::isKeyPressed(Keyboard::Up))       
            for (int i = 0; i < 5; i++)
                win[i].y += dist;
        
        if (Keyboard::isKeyPressed(Keyboard::Down))       
            for (int i = 0; i < 5; i++)
                win[i].y -= dist;
        
        if (Keyboard::isKeyPressed(Keyboard::Left))       
            for (int i = 0; i < 5; i++)
                win[i].x -= dist;
        
        if (Keyboard::isKeyPressed(Keyboard::Right))        
            for (int i = 0; i < 5; i++)
                win[i].x += dist;
        

        winVert[0].position = Vector2f(OX + win[0].x, OY - win[0].y);
        winVert[1].position = Vector2f(OX + win[1].x, OY - win[1].y);
        winVert[2].position = Vector2f(OX + win[2].x, OY - win[2].y);
        winVert[3].position = Vector2f(OX + win[3].x, OY - win[3].y);
        winVert[4].position = Vector2f(OX + win[0].x, OY - win[0].y);

        window.clear();

        for (int i = 0; i < segmSize; i++) { 
            //отрисовка отрезка            
            VertexArray drawingSegment(LineStrip, 2);
          
            drawingSegment[0] = segments[i][0];
            drawingSegment[0].position.x += OX;
            drawingSegment[0].position.y = OY - drawingSegment[0].position.y;
            drawingSegment[0].color = Color::White;

            drawingSegment[1] = segments[i][1];
            drawingSegment[1].position.x += OX;
            drawingSegment[1].position.y = OY - drawingSegment[1].position.y;    
            drawingSegment[1].color = Color::White;

            window.draw(drawingSegment);

            //отсеченный отрезок
            vector<Vector2f> res = getСrossedPoints(win, 5, segments[i]);

            //отрисовка отсеченного отрезка
            VertexArray cutLine(LineStrip, 2);
            if (!res.empty()) {
                cutLine[0].position = Vector2f(OX + res[0].x, OY - res[0].y); cutLine[0].color = Color::Green;
                cutLine[1].position = Vector2f(OX + res[1].x, OY - res[1].y); cutLine[1].color = Color::Green;
                window.draw(cutLine);              
            }
        }
    
        window.draw(OXLine, 2, Lines);
        window.draw(OYLine, 2, Lines);
        window.draw(winVert, 5, LineStrip);
        window.display();
    }

    delete[] winVert;
    for (int i = 0; i < segmSize; i++) delete[] segments[i];
        
    return 0;
}