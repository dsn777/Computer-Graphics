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

vector<vector<double>> matrixMultiply(vector<vector<double>> M1, vector<vector<double>> M2) {
    vector<vector<double>> product(M1.size(), vector<double>(M2[0].size()));
    for (int row = 0; row < M1.size(); row++) 
        for (int col = 0; col < M2[0].size(); col++) 
            for (int inner = 0; inner < M1[0].size(); inner++) 
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
    else if (axis == "Y") {
        a1 = vector<double>{ cos(angle), 0, sin(angle) };
        a2 = vector<double>{ 0, 1, 0 };
        a3 = vector<double>{ -sin(angle), 0 , cos(angle) };
    }
    else if (axis == "Z") {
        a1 = vector<double>{ cos(angle), -sin(angle), 0 };
        a2 = vector<double>{ sin(angle), cos(angle), 0 };
        a3 = vector<double>{ 0, 0 , 1 };
    }
    else
        return p;
   
    vector<vector<double>> rotateMatrix;
    rotateMatrix.push_back(a1);
    rotateMatrix.push_back(a2);
    rotateMatrix.push_back(a3);

    vector<vector<double>> M1;
    M1.push_back(vector<double>{p.x, p.y, p.z});

    vector <vector<double>> res = matrixMultiply(M1, rotateMatrix);

    return Vector3f(res[0][0], res[0][1], res[0][2]);
}

double scalarMultiple(Vector3f a, Vector3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Edge {
    vector<Vector3f> vertices;
    Vector3f vectorMultiple(Vector3f a, Vector3f b, Vector3f O) {
        a -= O; b -= O;
        return Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

public:
    Edge(vector<Vector3f> vertices) {
        this->vertices = vertices;
    }
    Edge(){}

    bool isVisible() {
        return scalarMultiple(getNormal(), Vector3f(0, 0, 100)) > 0;
    }

    Vector3f getNormal() { 
        return vectorMultiple(vertices[1], vertices[3], vertices[0]);
    }

    Vertex* getEdgeXOY() {
        Vertex* Vertices2D = new Vertex[vertices.size() + 1];       
        for (int i = 0; i < vertices.size(); i++)
            Vertices2D[i] = Vertex(Vector2f(OX + vertices[i].x, OY - vertices[i].y));

        Vertices2D[vertices.size()] = Vertices2D[0];//

        return Vertices2D;
    }
    
};


int main()
{
    RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D Cube Rotation");
    Clock clock;

    int vertSize = 8;
    Vector3f* vertices = new Vector3f[8];
    int cubeLen = 200;
    vertices[0] = Vector3f(0, 0, 0);
    vertices[1] = Vector3f(0, 0, cubeLen);
    vertices[2] = Vector3f(cubeLen, 0, cubeLen);
    vertices[3] = Vector3f(cubeLen, 0, 0);

    vertices[4] = Vector3f(0, cubeLen, 0);
    vertices[5] = Vector3f(0, cubeLen, cubeLen);
    vertices[6] = Vector3f(cubeLen, cubeLen, cubeLen);
    vertices[7] = Vector3f(cubeLen, cubeLen, 0);
   
    for (int i = 0; i < 8; i++) {
        vertices[i].x -= cubeLen / 2;
        vertices[i].y -= cubeLen / 2;
        vertices[i].z -= cubeLen / 2;
    }

    int edgeSize = 6;
    Edge* edges = new Edge[edgeSize];

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

        if (Keyboard::isKeyPressed(Keyboard::Down))
            for (int i = 0; i < vertSize; i++)
                vertices[i] = rotate(vertices[i], -0.003, "X");

        if (Keyboard::isKeyPressed(Keyboard::Up))
            for (int i = 0; i < vertSize; i++)
                vertices[i] = rotate(vertices[i], 0.003, "X");

        if (Keyboard::isKeyPressed(Keyboard::Left))
            for (int i = 0; i < vertSize; i++)
                vertices[i] = rotate(vertices[i], 0.003, "Y");

        if (Keyboard::isKeyPressed(Keyboard::Right))
            for (int i = 0; i < vertSize; i++)
                vertices[i] = rotate(vertices[i], -0.003, "Y");
      
        edges[0] = Edge({ vertices[5], vertices[6], vertices[7], vertices[4] });
        edges[1] = Edge({ vertices[0], vertices[3], vertices[2], vertices[1] });
        edges[2] = Edge({ vertices[1], vertices[2], vertices[6], vertices[5] });
        edges[3] = Edge({ vertices[2], vertices[3], vertices[7], vertices[6] });
        edges[4] = Edge({ vertices[3], vertices[0], vertices[4], vertices[7] });
        edges[5] = Edge({ vertices[0], vertices[1], vertices[5], vertices[4] });

        window.clear();
        //window.draw(OXLine, 2, Lines);
        //window.draw(OYLine, 2, Lines);

        vector<int> isVisible;
        for (int i = 0; i < edgeSize; i++) {
            if (edges[i].isVisible())
                isVisible.push_back(i);
            else {
                Vertex* drawVertices = edges[i].getEdgeXOY();
                ConvexShape rectangle(4);
                rectangle.setPoint(0, drawVertices[0].position);
                rectangle.setPoint(1, drawVertices[1].position);
                rectangle.setPoint(2, drawVertices[2].position);
                rectangle.setPoint(3, drawVertices[3].position);
                rectangle.setOutlineThickness(0.1);
                rectangle.setOutlineColor(Color::Yellow);
                rectangle.setFillColor(Color::Transparent);

                window.draw(rectangle);

                delete[] drawVertices;
            }
        }

        for (int i = 0; i < isVisible.size(); i++) {
            Vertex* drawVertices = edges[isVisible[i]].getEdgeXOY();
            window.draw(drawVertices, 5, LineStrip);
            delete[] drawVertices;
        }
               
        window.display();
    }
    
    delete[] edges;
    delete[] vertices;
 
    return 0;
}