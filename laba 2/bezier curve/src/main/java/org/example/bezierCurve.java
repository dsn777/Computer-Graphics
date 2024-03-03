package org.example;

import java.util.ArrayList;
import java.util.List;

public class bezierCurve {
    private final List<Double> drawX;
    private final List<Double> drawY;

    bezierCurve(List<Vertex> P, int drawPointsSize, double OX, double OY) {
        drawX = new ArrayList<>();
        drawY = new ArrayList<>();

        Vertex pushingPoint;
        double tStep = 1d / (drawPointsSize - 1);
        for (double t = 0; t <= 1 + 1e-3; t += tStep) {
            pushingPoint = Vertex.mult(P.get(0), (1 - t) * (1 - t) * (1 - t));
            pushingPoint = Vertex.plus(pushingPoint, Vertex.mult(P.get(1), 3 * t * (1 - t) * (1 - t)));
            pushingPoint = Vertex.plus(pushingPoint, Vertex.mult(P.get(2), 3 * t * t * (1 - t)));
            pushingPoint = Vertex.plus(pushingPoint, Vertex.mult(P.get(3), t * t * t));

            pushingPoint.x += OX;
            pushingPoint.y = OY - pushingPoint.y;

            drawX.add(pushingPoint.x);
            drawY.add(pushingPoint.y);
        }
    }

    public List<Double> getDrawX() {
        return drawX;
    }
    public List<Double> getDrawY() {
        return drawY;
    }
}
