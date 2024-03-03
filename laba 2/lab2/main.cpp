#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int OX = 10,
OY = 590,
dx = 1,
dy = 1;

Vertex OXLine[] = {
	Vertex(Vector2f(OX, OY)),
	Vertex(Vector2f(800 - OX, OY)),
};

Vertex OYLine[] = {
	Vertex(Vector2f(OX, OY)),
	Vertex(Vector2f(OX, 600 - OY)),
};

static int selectedPoint = 0;

class bezierCurve {
public:
	vector<Vector2f> P;
	vector<Vector2f> drawPoints;

	bezierCurve(vector<Vector2f> points, int drawPointsSize) {
		this->P = points;

		//B(t) = (1 - t)^3*P0 + 3t(1-t)^2*P1 + 3t^2*(1-t)*P2 + t^3*P3;	
		Vector2f pushingPoint(1, 2);

		double tStep = 1.f / (drawPointsSize - 1);
		for (float t = 0; t <= 1 + 1e-3; t += tStep) {
			pushingPoint = P[0] * (1 - t) * (1 - t) * (1 - t) + P[1] * 3.f * t * (1 - t) * (1 - t) + P[2] * 3.f * t * t * (1 - t) + P[3] * t * t * t;
			drawPoints.push_back(pushingPoint);
		}
	}

	Vertex* getDrawVertex() {
		Vertex* v = new Vertex[drawPoints.size()];

		for (int i = 0; i < drawPoints.size(); i++) {
			drawPoints[i].x += OX;
			drawPoints[i].y = OY - drawPoints[i].y;
			v[i] = Vertex(drawPoints[i]);
		}

		return v;
	}
};



int main()
{
	RenderWindow window(sf::VideoMode(800, 600), "lab2");
	
	Vector2f P0(10, 10);
	Vector2f P1(100, 500);
	Vector2f P2(500, 400);
	Vector2f P3(700, 100);

	CircleShape P0circle(2); 
	CircleShape P1circle(2);
	CircleShape P2circle(2);
	CircleShape P3circle(2);
	
	const int drawPointsSize = 100;
	Vector2f* ptrP = &P0;

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Num0)) {
			ptrP = &P0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num1)) {
			ptrP = &P1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2)) {
			ptrP = &P2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num3)) {
			ptrP = &P3;
		}

		

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			ptrP->y += 1;
			//ptrCircle->setPosition(pos);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			ptrP->y -= 1;
			//ptrCircle->setPosition(pos);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			ptrP->x += 1;
			//ptrCircle->setPosition(pos);
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			ptrP->x -= 1;
			//ptrCircle->setPosition(pos);
		}	

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
			
				if (event.mouseButton.button == sf::Mouse::Left)
				{


					//ptrP.setPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
					ptrP->x = OX + event.mouseButton.x;
					ptrP->y = OY - event.mouseButton.x;

				}
	
			} 
		}

		window.clear();	
		window.draw(OXLine, 2, Lines);
		window.draw(OYLine, 2, Lines);

	//здесь нужно зациклить на случай если будет больше точек и все
	//так же между 3 и 4 точками добавить доп.топчку через которую будет строиться кривая
		bezierCurve curve(vector<Vector2f>{P0, P1, P2, P3}, drawPointsSize);
		Vertex* v = curve.getDrawVertex();
		for (int i = 0; i < drawPointsSize - 1; i++) {
			Vertex line[] = { v[i], v[i + 1] };
			window.draw(line, 2, Lines);
		}
	//
		
		window.display();	
		delete[] v;
	}
	
	return 0;
}

