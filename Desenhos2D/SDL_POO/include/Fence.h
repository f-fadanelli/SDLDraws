#ifndef FENCE_H_INCLUDED
#define FENCE_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"
#include "Polygon.h"

class Fence {
private:
    int largura;
    int altura;

    Uint32 cor;

    int localizacaoX;
    int localizacaoY;

    double inclinacao;

    std::vector<Point> pontosCerca = {
        Point(0.0, 0.0),
        Point(0.0, 0.7),
        Point(0.25, 0.9),
        Point(0.5, 0.7),
        Point(0.75, 0.9),
        Point(1.0, 0.7),
        Point(1.0, 0.0),
    };

    std::vector<Point> pontoCandidato = {Point(0.1, 0.1)};

public:
    Fence(Uint32 cor, int largura, int altura, int localizacaoX, int localizacaoY, double inclinacao)
        : cor(cor), largura(largura), altura(altura),
          localizacaoX(localizacaoX), localizacaoY(localizacaoY),
          inclinacao(inclinacao) {}

    void draw(SDL_Surface* surface, int tX, int tY, int mX, int mY) {
        if (!surface) return;


        Polygon cerca(pontosCerca, cor);
        cerca.scale(largura, altura);
        cerca.rotate(inclinacao);
        cerca.translate(localizacaoX, localizacaoY);
        cerca.clipp(mX, mY);
        cerca.transformaTela(tX, tY, mX, mY);
        cerca.draw(surface);
        cerca.fill(surface);

        Polygon pcCerca(pontoCandidato, cor);
        pcCerca.scale(largura, altura);
        pcCerca.rotate(inclinacao);
        pcCerca.translate(localizacaoX, localizacaoY);
        pcCerca.transformaTela(tX, tY, mX, mY);
        std::vector<Point> pts = pcCerca.getPoints();
        Point p = pts[0];

        cerca.fillFromPoint(surface, p);

    }

    void setCor(Uint32 novaCor) { cor = novaCor; }
    void setLocalizacao(int x, int y) { localizacaoX = x; localizacaoY = y; }
    void setLargura(int w) { largura = w; }
    void setAltura(int h) { altura = h; }
    void setInclinacao(double inc) { inclinacao = inc; }
};

#endif
