#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "Point.h"
#include "Polygon.h"
#include "Primitives.h"

class Ground {
private:
    Uint32 cor;

public:
    Ground(Uint32 cor)
        : cor(cor) {}

    void draw(SDL_Surface* surface) {
        if (!surface) return;

        int larguraTela = surface->w;
        int alturaTela = surface->h;

        int yTopo = static_cast<int>(alturaTela * 0.6);
        int yBase = alturaTela;

        std::vector<Point> pontos = {
            Point(0, yBase),
            Point(0, yTopo),
            Point(larguraTela, yTopo),
            Point(larguraTela, yBase)
        };

        Polygon chao(pontos, cor);
        chao.draw(surface);
        chao.fill(surface);
    }

    void setCor(Uint32 novaCor) { cor = novaCor; }
};

#endif
