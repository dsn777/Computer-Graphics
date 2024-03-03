#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;

int OX = WIDTH / 2,
OY = HEIGHT / 2;


Vertex OXLine[] = {
    Vertex(Vector2f(0, OY)),
    Vertex(Vector2f(WIDTH, OY)),
};

Vertex OYLine[] = {
    Vertex(Vector2f(OX, 0)),
    Vertex(Vector2f(OX, HEIGHT)),
};

//(1 - u)(1 - v)p1 + u(1 - v)p2 + (1-u)vp3 + uvp4,
Vector3f getPoint(Vector3f* p, float u, float v) {
    return (1 - u) * (1 - v) * p[0] + u * (1 - v) * p[1] + (1 - u) * v * p[2] + u * v * p[3];
}

vector<vector<double>> matrixMultiply(vector<vector<double>> M1, vector<vector<double>> M2) {
    vector<vector<double>> product(M1.size(), vector<double>(M2[0].size()));
    for (int row = 0; row < M1.size(); row++) 
        for (int col = 0; col < 3; col++)            
            for (int inner = 0; inner < M2[0].size(); inner++) 
                product[row][col] += M1[row][inner] * M2[inner][col];
                    
    return product;
}

Vector3f rotate(Vector3f p, double angle, String axis) {
    vector<double> a1, a2, a3;
    if (axis == "X") {
        a1 = vector<double>{ 1, 0, 0 };
        a2 = vector<double>{ 0, cos(angle), -sin(angle) };
        a3 = vector<double>{ 0, sin(angle), cos(angle) };
    }
    if (axis == "Y") {
        a1 = vector<double>{ cos(angle), 0, sin(angle) };
        a2 = vector<double>{ 0, 1, 0 };
        a3 = vector<double>{ -sin(angle), 0 , cos(angle) };
    }
    if (axis == "Z") {
        a1 = vector<double>{ cos(angle), -sin(angle), 0 };
        a2 = vector<double>{ sin(angle), cos(angle), 0 };
        a3 = vector<double>{ 0, 0 , 1 };
    }

    vector<vector<double>> rotateMatrix;
    rotateMatrix.push_back(a1);
    rotateMatrix.push_back(a2);
    rotateMatrix.push_back(a3);

    vector<vector<double>> M1;
    M1.push_back(vector<double>{p.x, p.y, p.z});

    vector <vector<double>> res = matrixMultiply(M1, rotateMatrix);

    return Vector3f(res[0][0], res[0][1], res[0][2]);
}


vector<vector<Vector3f>> getGrid(vector<vector<Vector3f>> grid, double angle, String axis) {
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
            grid[i][j] = rotate(grid[i][j], angle, axis);

    return grid;
}


int main()
{
    RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D Cube Rotation");
    Clock clock;
    float angle = 0;

    //определяющие точки поверхности
    Vector3f p1(0, 0, 0);
    Vector3f p2(0, 200, 330);
    Vector3f p3(150, 200, 0);
    Vector3f p4(400, 0, 500);
    Vector3f p[] = { p1, p2, p4, p3 };

    //построение сетки поверхности
    vector<vector<Vector3f>> grid;
    for (float u = 0; u <= 1; u += 0.06) {
        vector<Vector3f> tempLine;
        for (float v = 0; v <= 1; v += 0.06) {
            tempLine.push_back(getPoint(p, u, v));
        }
        grid.push_back(tempLine);
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

        //определение оси вращения
        angle = 0;

        if (Keyboard::isKeyPressed(Keyboard::Up))
            angle = 0.03f, grid = getGrid(grid, angle, "X");

        if (Keyboard::isKeyPressed(Keyboard::Down))
            angle = -0.03f, grid = getGrid(grid, angle, "X");

        if (Keyboard::isKeyPressed(Keyboard::Left))
            angle = -0.03f, grid = getGrid(grid, angle, "Y");

        if (Keyboard::isKeyPressed(Keyboard::Right))
            angle = 0.03f, grid = getGrid(grid, angle, "Y");

        if (Keyboard::isKeyPressed(Keyboard::W))
            angle = 0.03f, grid = getGrid(grid, angle, "Z");

        if (Keyboard::isKeyPressed(Keyboard::S))
            angle = -0.03f, grid = getGrid(grid, angle, "Z");


        window.clear();
        window.draw(OXLine, 2, Lines);
        window.draw(OYLine, 2, Lines);

        //громоздкая отрисовка сетки
        for (int i = 0; i < grid.size(); i++) {
            Vertex* line1 = new Vertex[grid.size()];
            Vertex* line2 = new Vertex[grid[0].size()];

            for (int j = 0; j < grid[0].size(); j++) {
                line1[j].position.x = OX + grid[i][j].x;
                line1[j].position.y = OY - grid[i][j].y;

                line2[j].position.x = OX + grid[j][i].x;
                line2[j].position.y = OY - grid[j][i].y;
            }

            window.draw(line1, grid.size(), LineStrip);
            window.draw(line2, grid[0].size(), LineStrip);

            delete[] line1;
            delete[] line2;
        }

        window.display();
    }
    return 0;
}