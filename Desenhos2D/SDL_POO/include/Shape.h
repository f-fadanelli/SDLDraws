#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>

class Shape {
public:
    virtual ~Shape() {}
    virtual void draw(SDL_Surface* surface) = 0;
    virtual void fill(SDL_Surface* surface) = 0;
    virtual void translate(int tx, int ty) = 0;
    virtual void scale(double Ex, double Ey) = 0;
    virtual void rotate(double angle) = 0;
    virtual void transformaTela(int tX, int tY, int mX, int mY) = 0;
    virtual void clipp(int width, int height) = 0;
    virtual void describe() = 0;
};

#endif


