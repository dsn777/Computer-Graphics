package org.example;

import javax.swing.*;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.List;


public class Main extends JComponent implements ActionListener, MouseListener {
    private static final int WIDTH = 1200;
    private static final int HEIGHT = 800;
    private final int OX = 25;
    private final int OY = HEIGHT - 85;
    private boolean isDragging = false;
    private compBezier curve;
    int draggedPointIndex = 0;
    int vertexSize = 14;

    public Main() {
        P = new Vertex[vertexSize];
        for (int i = 0; i < vertexSize; i++) {
            P[i] = new Vertex(OX + i * 100, 100);
        }

        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (e.getButton() == MouseEvent.BUTTON1) {
                    float mouseX = e.getX();
                    float mouseY = e.getY();
                    Integer PIndex = curve.getContainingV(mouseX, mouseY, OX, OY);
                    if (PIndex == null)
                        return;
                    draggedPointIndex = PIndex;
                    isDragging = true;
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                if (e.getButton() == MouseEvent.BUTTON1) {
                    isDragging = false;
                }
            }
        });

        addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                if (isDragging) {
                    float mouseX = e.getX(),
                            mouseY = e.getY();

                    P[draggedPointIndex] = new Vertex(mouseX - OX, (OY - mouseY));
                    repaint();
                }
            }
        });
    }

    Vertex[] P;
    int drawPointRadius = 10;

    @Override
    public void paint(Graphics g) {
        g.setColor(Color.BLACK);

        //отрисовка опорных точек + добавление в список для построения кривой
        List<Vertex> v = new ArrayList<>();
        for (Vertex point : P) {
            g.fillOval((int) (OX + point.x - drawPointRadius / 2), (int) (OY - point.y - drawPointRadius / 2), drawPointRadius, drawPointRadius);
            v.add(point);
        }

        //отрисовка осей
        g.setColor(Color.GRAY);
        g.drawLine(0, OY, WIDTH, OY);
        g.drawLine(OX, 0, OX, HEIGHT);

        //создание кривой
        try {
            curve = new compBezier(v, OX, OY);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        //отрисовка кривой
        g.setColor(Color.RED);
        int[] xArr = curve.getDrawX();
        int[] yArr = curve.getDrawY();
        g.drawPolyline(xArr, yArr, xArr.length);
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Draggable Rectangle - Java Swing");
        Main draggableRectangle = new Main();
        frame.add(draggableRectangle);
        frame.setSize(WIDTH, HEIGHT);
        frame.setLocation(300, 150);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }
}
