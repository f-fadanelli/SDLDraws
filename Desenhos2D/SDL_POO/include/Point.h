#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
private:
    float x, y;

public:
    Point(float newX = 0, float newY = 0) {
        x = newX;
        y = newY;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    void setX(float newX){
        x = newX;
    }
    void setY(float newY) {
        y = newY;
    }

    void print() {
      std::cout << "Point(" << x << "," << y << ")" << std::endl;
    }
};

#endif

