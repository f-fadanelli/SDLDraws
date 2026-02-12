#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"
#include "Polygon.h"

class Tree {
private:
    int altura;
    int largura;

    Uint32 corFolhas;
    Uint32 corTronco;

    int localizacaoX;
    int localizacaoY;

    double inclinacao;

    std::vector<Point> pontosTronco = {
        Point(0.4, 0.0), Point(0.4, 0.3), Point(0.6, 0.3), Point(0.6, 0.0)
    };

    std::vector<Point> pontosFolhas = {
        Point(0.2, 0.3), Point(0.4, 0.5), Point(0.3, 0.5), Point(0.5, 0.9), Point(0.7, 0.5), Point(0.6, 0.5), Point(0.8, 0.3)
    };

public:
    Tree(Uint32 corFolhas, Uint32 corTronco,
         int largura, int altura,
         int localizacaoX, int localizacaoY,
         double inclinacao)
        : corFolhas(corFolhas), corTronco(corTronco),
          largura(largura), altura(altura),
          localizacaoX(localizacaoX), localizacaoY(localizacaoY),
          inclinacao(inclinacao) {}

    void draw(SDL_Surface* surface, int tX, int tY, int mX, int mY) {
        if (!surface) return;

        std::vector<Shape*> formas;
        formas.push_back(new Polygon(pontosTronco, corTronco));
        formas.push_back(new Polygon(pontosFolhas, corFolhas));

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

    void setCorFolhas(Uint32 cor) { corFolhas = cor; }
    void setCorTronco(Uint32 cor) { corTronco = cor; }

    void setLocalizacao(int x, int y) { localizacaoX = x; localizacaoY = y; }
    void setLargura(int w) { largura = w; }
    void setAltura(int h) { altura = h; }
    void setInclinacao(double inc) { inclinacao = inc; }
};

#endif


