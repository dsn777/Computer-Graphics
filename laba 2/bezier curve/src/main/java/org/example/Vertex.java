package org.example;

public class Vertex {
    public double x;
    public double y;
    public static Vertex mult(Vertex a, double scalar) {
        return new Vertex(a.x * scalar, a.y * scalar);
    }
    public static Vertex plus(Vertex a, Vertex b) {
        return new Vertex(a.x + b.x, a.y + b.y);
    }
    Vertex(double x, double y) {
        this.x = x;
        this.y = y;
    }
}
