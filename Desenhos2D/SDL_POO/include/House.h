#ifndef HOUSE_H_INCLUDED
#define HOUSE_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"
#include "Polygon.h"
#include "Curve.h"

class House {
private:
    Uint32 corParede;
    Uint32 corPorta;
    Uint32 corTelhado;

    int largura;
    int altura;

    int localizacaoX;
    int localizacaoY;

    double inclinacao;

    std::vector<Point> pontosParede = {
        Point(0.1, 0.0), Point(0.1, 0.6), Point(0.9, 0.6), Point(0.9, 0.0)
    };

     std::vector<Point> pontosPorta = {
        Point(0.6, 0.1), Point(0.5, 0.6), Point(0.9, 0.6), Point(0.8, 0.1)
    };

    std::vector<Point> pontosTelhado = {
        Point(0.0, 0.6), Point(0.5, 1.0), Point(1.0, 0.6)
    };

    std::vector<Point> pontosChamine = {
        Point(0.2, 0.7), Point(0.2, 1.0), Point(0.3, 1.0), Point(0.3, 0.7)
    };

public:
    House(Uint32 corParede, Uint32 corPorta, Uint32 corTelhado,
          int largura, int altura,
          int localizacaoX, int localizacaoY,
          double inclinacao)
        : corParede(corParede), corPorta(corPorta), corTelhado(corTelhado),
          largura(largura), altura(altura),
          localizacaoX(localizacaoX), localizacaoY(localizacaoY),
          inclinacao(inclinacao) {}

   void draw(SDL_Surface* surface, int tX, int tY, int mX, int mY) {
        if (!surface) return;

        std::vector<Shape*> formas;
        formas.push_back(new Polygon(pontosChamine, corTelhado));
        formas.push_back(new Polygon(pontosTelhado, corTelhado));
        formas.push_back(new Polygon(pontosParede, corParede));
        formas.push_back(new Curve(pontosPorta, corPorta));

        for (Shape* forma : formas) {
            forma->scale(largura, altura);
            forma->rotate(inclinacao);
            forma->translate(localizacaoX, localizacaoY);
            forma->clipp(mX, mY);
            forma->transformaTela(tX, tY, mX, mY);
            forma->draw(surface);
            forma->fill(surface);
        }
    }

    void setCorParede(Uint32 cor) { corParede = cor; }
    void setCorPorta(Uint32 cor) { corPorta = cor; }
    void setCorTelhado(Uint32 cor) { corTelhado = cor; }

    void setLocalizacao(int x, int y) { localizacaoX = x; localizacaoY = y; }
    void setLargura(int w) { largura = w; }
    void setAltura(int h) { altura = h; }
    void setInclinacao(double inc) { inclinacao = inc; }
};

#endif
