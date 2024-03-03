package org.example;


import java.util.ArrayList;
import java.util.List;

class compBezier {
    List<Vertex> P;
    List<Double> P_drawX;
    List<Double> P_drawY;
    List<Double> drawX;
    List<Double> drawY;

    compBezier(List<Vertex> P, double OX, double OY) {
        this.P = P;

        List<Vertex> extentedP = new ArrayList<>(P);
        drawX = new ArrayList<>();
        drawY = new ArrayList<>();
        P_drawX = new ArrayList<>();
        P_drawY = new ArrayList<>();

        //здесь заполняется массив опорных точек Х и У
        for (Vertex p : P) {
            P_drawX.add(OX + p.x);
            P_drawY.add(OY - p.y);
        }

        //здесь добавляются доп.точки для сглаживания стыков между кривыми
        for (int i = 2; i < extentedP.size() - 1; i += 2) {
            Vertex p2 = extentedP.get(i);
            Vertex p3 = extentedP.get(i + 1);
            extentedP.add(i++ + 1, new Vertex((p2.x + p3.x) / 2, (p2.y + p3.y) / 2));
        }

        //здесь в случае нехватки точек для построения последней кривой из четырех точек массив опорных точек дополняется до 4х точек
        while ((extentedP.size() - 1) % 3 != 0)
            extentedP.add(extentedP.get(extentedP.size() - 1));

        for (int i = 0; i < extentedP.size() - 1; i += 3) {
            bezierCurve bezCurve = new bezierCurve(subList(extentedP, i, i + 3), 100, OX, OY);
            drawX.addAll(bezCurve.getDrawX());
            drawY.addAll(bezCurve.getDrawY());
        }
    }

    private List<Vertex> subList(List<Vertex> list, int from, int to) {
        List<Vertex> res = new ArrayList<>();
        for (int i = from; i < list.size() && i <= to; i++)
            res.add(list.get(i));

        return res;
    }

    Integer getContainingV(float mouseX, float mouseY, float OX, float OY) {
        double touchedRadius = 20 * 20;
        for (int i = 0; i < P.size(); i++) {
            Vertex v = P.get(i);
            if ((mouseX - (OX + v.x)) * (mouseX - (OX + v.x)) + (mouseY - (OY - v.y)) * (mouseY - (OY - v.y)) <= touchedRadius)
                return i;
        }
        return null;
    }

    public int[] getDrawX() {
        int[] intX = new int[drawX.size()];
        for (int i = 0; i < intX.length; i++)
            intX[i] = (int) (Math.round(drawX.get(i)));

        return intX;
    }

    public int[] getDrawY() {
        int[] intY = new int[drawY.size()];
        for (int i = 0; i < intY.length; i++)
            intY[i] = (int) (Math.round(drawY.get(i)));

        return intY;
    }

   /* public int[] getP_DrawX() {
        int[] intX = new int[P_drawX.size()];
        for (int i = 0; i < intX.length; i++) {
            intX[i] = (int) (Math.round(P_drawX.get(i)));
        }
        return intX;
    }

    public int[] getP_DrawY() {
        int[] intY = new int[P_drawY.size()];
        for (int i = 0; i < intY.length; i++) {
            intY[i] = (int) (Math.round(P_drawY.get(i)));
        }
        return intY;
    }


    public List<Vertex> getP() {
        return P;
    }*/
}
