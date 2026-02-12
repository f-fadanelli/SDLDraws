#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include <string>
#include <SDL2/SDL.h>
#include "Point.h"
#include <vector>

class Primitives {
public:
    static void setPixel(SDL_Surface* surface, int x, int y, Uint32 color);

    static Uint32 getPixel(SDL_Surface* surface, int x, int y);

    static Uint32 rgbToUint32(SDL_Surface* surface, int r, int g, int b);

    static void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);

    static void drawBezierQuadratica(SDL_Surface* surface, const Point& p0, const Point& p1, const Point& p2, Uint32 color);

    static void drawBezierCubica(SDL_Surface* surface, const Point& p0, const Point& p1, const Point& p2, const Point& p3, Uint32 color);

    static void floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor, Uint32 borderColor);

    static std::vector<Point> gerarPontosCirculo(double cx, double cy, double radius);

    static SDL_Color uint32ToSDL_COLOR(SDL_Surface* surface, Uint32 color);

    static Uint32 getColorByName(SDL_Surface* surface, const std::string& name);
};

#endif
