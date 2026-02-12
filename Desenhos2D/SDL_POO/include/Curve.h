#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED

#include <cmath>
#include <vector>
#include <SDL2/SDL.h>
#include "Shape.h"
#include "Point.h"
#include "Primitives.h"

class Curve: public Shape {
private:
    std::vector<Point> points;
    Uint32 color;

public:
    Curve(std::vector<Point> points, Uint32 color)
        : points(std::move(points)), color(color) {}

    double toRadians(double degrees)
    {
        return degrees * 0.017453293;
    }

    void draw(SDL_Surface* surface) {
        if (!surface || points.size() != 4) return;

        Point p0 = points[0];
        Point p1 = points[1];
        Point p2 = points[2];
        Point p3 = points[3];
        Primitives::drawBezierCubica(surface, p0, p1, p2, p3, color);
        Primitives::drawLine(surface, p0.getX(), p0.getY(), p3.getX(), p3.getY(), color);
    }

    void fill(SDL_Surface* surface) override  {
        if (!surface || points.size() < 3) return;

        int sumX = 0, sumY = 0;
        for (const auto& point : points) {
            sumX += point.getX();
            sumY += point.getY();
        }

        int cx = sumX / points.size();
        int cy = sumY / points.size();

        if (cx < 0 || cx >= surface->w || cy < 0 || cy >= surface->h) return;

       Primitives::floodFill(surface, cx, cy, color, color);
    }

    void translate(int tx, int ty) override {

        for (auto& point : points){
            point.setX(point.getX() + tx);
            point.setY(point.getY() + ty);
        }
    }

    void scale(double Ex, double Ey) override {
        if (points.empty()) return;

        for (int i = 0; i < points.size(); ++i) {
            float dx = points[i].getX();
            float dy = points[i].getY();
            points[i].setX(dx * Ex);
            points[i].setY(dy * Ey);
        }
    }

    void rotate(double angle) override {
        if (points.empty()) return;

        double x, y, radians, cosTheta, sinTheta;

        radians  = toRadians(angle);
        cosTheta = cos(radians);
        sinTheta = sin(radians);

        for (int i = 0; i < points.size(); ++i) {
            x = points[i].getX();
            y = points[i].getY();
            points[i].setX(x * cosTheta - y * sinTheta);
            points[i].setY(x * sinTheta + y * cosTheta);
        }
    }

    void transformaTela(int tX, int tY, int mX, int mY) override {
        for (int i = 0; i < points.size(); ++i) {
            float px = points[i].getX();
            float py = points[i].getY();

            points[i].setX((px*tX)/mX);
            points[i].setY(tY-((py*tY)/mY));
        }
    }

    void describe() {

        for (auto& point : points){
            std::cout << point.getX() << "," << point.getY() << std::endl;
        }
    }

    void clipp(int width, int height) override {
        if (points.size() < 3) return;

        int sumX = 0, sumY = 0;
        for (const auto& point : points) {
            sumX += point.getX();
            sumY += point.getY();
        }

        int cx = sumX / points.size();
        int cy = sumY / points.size();

        if (cx >= 0 && cx < width && cy >= 0 && cy < height)
            return;

        auto clipEdge = [](const std::vector<Point>& input, auto inside, auto intersect) {
            std::vector<Point> output;
            if (input.empty()) return output;

            Point S = input.back();
            for (const Point& E : input) {
                if (inside(E)) {
                    if (!inside(S)) {
                        output.push_back(intersect(S, E));
                    }
                    output.push_back(E);
                }
                else if (inside(S)) {
                    output.push_back(intersect(S, E));
                }
                S = E;
            }
            return output;
        };

        std::vector<Point> clippedPoints = points;

        clippedPoints = clipEdge(clippedPoints,
            [](const Point& p) { return p.getX() >= 0; },
            [](const Point& p1, const Point& p2) {
                float x1 = p1.getX(), y1 = p1.getY();
                float x2 = p2.getX(), y2 = p2.getY();
                float x = 0;
                float y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
                return Point(x, y);
            });

        clippedPoints = clipEdge(clippedPoints,
            [width](const Point& p) { return p.getX() <= width - 1; },
            [width](const Point& p1, const Point& p2) {
                float x1 = p1.getX(), y1 = p1.getY();
                float x2 = p2.getX(), y2 = p2.getY();
                float x = width - 1;
                float y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
                return Point(x, y);
            });

        clippedPoints = clipEdge(clippedPoints,
            [](const Point& p) { return p.getY() >= 0; },
            [](const Point& p1, const Point& p2) {
                float x1 = p1.getX(), y1 = p1.getY();
                float x2 = p2.getX(), y2 = p2.getY();
                float y = 0;
                float x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
                return Point(x, y);
            });

        clippedPoints = clipEdge(clippedPoints,
            [height](const Point& p) { return p.getY() <= height - 1; },
            [height](const Point& p1, const Point& p2) {
                float x1 = p1.getX(), y1 = p1.getY();
                float x2 = p2.getX(), y2 = p2.getY();
                float y = height - 1;
                float x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                return Point(x, y);
            });

        points = clippedPoints;
    }

    const std::vector<Point> getPoints() const {
        return points;
    }

    void setColor(Uint32 newColor) {
        color = newColor;
    }
};
#endif

