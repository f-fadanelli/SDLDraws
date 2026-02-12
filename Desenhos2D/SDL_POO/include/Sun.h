#ifndef SUN_H_INCLUDED
#define SUN_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>

#include "Shape.h"
#include "Point.h"
#include "Polygon.h"
#include "Primitives.h"

class Sun {
private:
    Uint32 cor;
    int largura;
    int altura;

    int localizacaoX;
    int localizacaoY;

    double inclinacao;

    double centroNormalizadoX = 0.5;
    double centroNormalizadoY = 0.5;
    double raioCircunferencia = 0.5;

public:
    Sun(Uint32 cor, int largura, int altura, int localizacaoX, int localizacaoY, int inclinacao)
        : cor(cor), largura(largura), altura(altura),
          localizacaoX(localizacaoX), localizacaoY(localizacaoY), inclinacao(inclinacao) {}

    void draw(SDL_Surface* surface, int tX, int tY, int mX, int mY) {
        if (!surface) return;

        std::vector<Point> pontosDisco = Primitives::gerarPontosCirculo(centroNormalizadoX, centroNormalizadoY, raioCircunferencia);

        std::vector<Shape*> formas;
        formas.push_back(new Polygon(pontosDisco, cor));

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

    void setCor(Uint32 novaCor) { cor = novaCor; }
    void setLargura(int w) { largura = w; }
    void setAltura(int h) { altura = h; }
    void setLocalizacao(int x, int y) { localizacaoX = x; localizacaoY = y; }
};

#endif
