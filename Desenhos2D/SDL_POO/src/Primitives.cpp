#include "Primitives.h"
#include <cmath>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
#include <Point.h>

void Primitives::setPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (!surface) return;
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;
    pixels[y * pitch + x] = color;
}

Uint32 Primitives::getPixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:
            return 0;
    }
}

Uint32 Primitives::rgbToUint32(SDL_Surface* surface, int r, int g, int b) {
    return SDL_MapRGB(surface->format, r, g, b);
}

void Primitives::drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void Primitives::drawBezierQuadratica(SDL_Surface* surface, const Point& p0, const Point& p1, const Point& p2, Uint32 color) {
    const int STEPS = 100;
    for (int i = 0; i <= STEPS; i++) {
        float t = i / static_cast<float>(STEPS);
        float xt = (1 - t) * (1 - t) * p0.getX() + 2 * (1 - t) * t * p1.getX() + t * t * p2.getX();
        float yt = (1 - t) * (1 - t) * p0.getY() + 2 * (1 - t) * t * p1.getY() + t * t * p2.getY();
        setPixel(surface, static_cast<int>(xt), static_cast<int>(yt), color);
    }
}

void Primitives::drawBezierCubica(SDL_Surface* surface, const Point& p0, const Point& p1, const Point& p2, const Point& p3, Uint32 color) {
    const int STEPS = 1000;
    for (int i = 0; i <= STEPS; i++) {
        float t = i / static_cast<float>(STEPS);

        float oneMinusT = 1 - t;

        float xt =
            oneMinusT * oneMinusT * oneMinusT * p0.getX() +
            3 * oneMinusT * oneMinusT * t * p1.getX() +
            3 * oneMinusT * t * t * p2.getX() +
            t * t * t * p3.getX();

        float yt =
            oneMinusT * oneMinusT * oneMinusT * p0.getY() +
            3 * oneMinusT * oneMinusT * t * p1.getY() +
            3 * oneMinusT * t * t * p2.getY() +
            t * t * t * p3.getY();

        setPixel(surface, static_cast<int>(xt), static_cast<int>(yt), color);
    }
}

void Primitives::floodFill(SDL_Surface* surface, int x, int y, Uint32 fillColor, Uint32 borderColor) {
    if (!surface) return;
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;

    auto getPixel = [&](int px, int py) -> Uint32 {
        return pixels[py * pitch + px];
    };

    std::stack<std::pair<int,int>> stack;
    stack.push({x,y});

    while (!stack.empty()) {
        auto [px, py] = stack.top();
        stack.pop();

        if (px < 0 || py < 0 || px >= surface->w || py >= surface->h) continue;

        if (getPixel(px, py) != borderColor && getPixel(px, py) != fillColor) {
            setPixel(surface, px, py, fillColor);
            stack.push({px+1, py});
            stack.push({px-1, py});
            stack.push({px, py+1});
            stack.push({px, py-1});
        }
    }
}

std::vector<Point> Primitives::gerarPontosCirculo(double cx, double cy, double radius) {
        int numLados = 20;
       std::vector<Point> pontos;
    double anguloIncremento = 2.0 * M_PI / numLados;

    for (int i = 0; i < numLados; ++i) {
        double angulo = i * anguloIncremento;
        double x = cx + radius * cos(angulo);
        double y = cy + radius * sin(angulo);
        pontos.emplace_back(x, y);
    }

    return pontos;
}

SDL_Color Primitives::uint32ToSDL_COLOR(SDL_Surface* surface, Uint32 color) {

    SDL_Color sdlColor;
    SDL_GetRGBA(color, surface->format,
            &sdlColor.r, &sdlColor.g, &sdlColor.b, &sdlColor.a);
    return sdlColor;
}

Uint32 Primitives::getColorByName(SDL_Surface* surface, const std::string& name)  {
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);

    if (n == "branco")    return Primitives::rgbToUint32(surface, 255, 255, 255);
    if (n == "branca")    return Primitives::rgbToUint32(surface, 255, 255, 255);
    else if (n == "prata")   return Primitives::rgbToUint32(surface, 192, 192, 192);
    else if (n == "cinza")   return Primitives::rgbToUint32(surface, 128, 128, 128);
    else if (n == "preto")   return Primitives::rgbToUint32(surface, 0, 0, 0);
    else if (n == "preta")   return Primitives::rgbToUint32(surface, 0, 0, 0);
    else if (n == "vermelho")return Primitives::rgbToUint32(surface, 255, 0, 0);
    else if (n == "vermelha")return Primitives::rgbToUint32(surface, 255, 0, 0);
    else if (n == "marrom")  return Primitives::rgbToUint32(surface, 128, 0, 0);
    else if (n == "amarelo") return Primitives::rgbToUint32(surface, 255, 255, 0);
    else if (n == "amarela") return Primitives::rgbToUint32(surface, 255, 255, 0);
    else if (n == "oliva")   return Primitives::rgbToUint32(surface, 128, 128, 0);
    else if (n == "lima")    return Primitives::rgbToUint32(surface, 0, 255, 0);
    else if (n == "verde")   return Primitives::rgbToUint32(surface, 0, 128, 0);
    else if (n == "aqua")    return Primitives::rgbToUint32(surface, 0, 255, 255);
    else if (n == "teal")    return Primitives::rgbToUint32(surface, 0, 128, 128);
    else if (n == "azul")    return Primitives::rgbToUint32(surface, 0, 0, 255);
    else if (n == "marinho") return Primitives::rgbToUint32(surface, 0, 0, 128);
    else if (n == "rosa")    return Primitives::rgbToUint32(surface, 255, 0, 255);
    else if (n == "roxo")    return Primitives::rgbToUint32(surface, 128, 0, 128);
    else if (n == "roxa")    return Primitives::rgbToUint32(surface, 128, 0, 128);
    else if (n == "laranja") return Primitives::rgbToUint32(surface, 255, 165, 0);
    else if (n == "aliceblue") return Primitives::rgbToUint32(surface, 240, 248, 255);
    else if (n == "antiquewhite") return Primitives::rgbToUint32(surface, 250, 235, 215);
    else if (n == "aqua") return Primitives::rgbToUint32(surface, 0, 255, 255);
    else if (n == "aquamarine") return Primitives::rgbToUint32(surface, 127, 255, 212);
    else if (n == "azure") return Primitives::rgbToUint32(surface, 240, 255, 255);
    else if (n == "beige") return Primitives::rgbToUint32(surface, 245, 245, 220);
    else if (n == "bisque") return Primitives::rgbToUint32(surface, 255, 228, 196);
    else if (n == "black") return Primitives::rgbToUint32(surface, 0, 0, 0);
    else if (n == "blanchedalmond") return Primitives::rgbToUint32(surface, 255, 235, 205);
    else if (n == "blue") return Primitives::rgbToUint32(surface, 0, 0, 255);
    else if (n == "blueviolet") return Primitives::rgbToUint32(surface, 138, 43, 226);
    else if (n == "brown") return Primitives::rgbToUint32(surface, 165, 42, 42);
    else if (n == "burlywood") return Primitives::rgbToUint32(surface, 222, 184, 135);
    else if (n == "cadetblue") return Primitives::rgbToUint32(surface, 95, 158, 160);
    else if (n == "chartreuse") return Primitives::rgbToUint32(surface, 127, 255, 0);
    else if (n == "chocolate") return Primitives::rgbToUint32(surface, 210, 105, 30);
    else if (n == "coral") return Primitives::rgbToUint32(surface, 255, 127, 80);
    else if (n == "cornflowerblue") return Primitives::rgbToUint32(surface, 100, 149, 237);
    else if (n == "cornsilk") return Primitives::rgbToUint32(surface, 255, 248, 220);
    else if (n == "crimson") return Primitives::rgbToUint32(surface, 220, 20, 60);
    else if (n == "cyan") return Primitives::rgbToUint32(surface, 0, 255, 255);
    else if (n == "darkblue") return Primitives::rgbToUint32(surface, 0, 0, 139);
    else if (n == "darkcyan") return Primitives::rgbToUint32(surface, 0, 139, 139);
    else if (n == "darkgoldenrod") return Primitives::rgbToUint32(surface, 184, 134, 11);
    else if (n == "darkgray") return Primitives::rgbToUint32(surface, 169, 169, 169);
    else if (n == "darkgreen") return Primitives::rgbToUint32(surface, 0, 100, 0);
    else if (n == "darkkhaki") return Primitives::rgbToUint32(surface, 189, 183, 107);
    else if (n == "darkmagenta") return Primitives::rgbToUint32(surface, 139, 0, 139);
    else if (n == "darkolivegreen") return Primitives::rgbToUint32(surface, 85, 107, 47);
    else if (n == "darkorange") return Primitives::rgbToUint32(surface, 255, 140, 0);
    else if (n == "darkorchid") return Primitives::rgbToUint32(surface, 153, 50, 204);
    else if (n == "darkred") return Primitives::rgbToUint32(surface, 139, 0, 0);
    else if (n == "darksalmon") return Primitives::rgbToUint32(surface, 233, 150, 122);
    else if (n == "darkseagreen") return Primitives::rgbToUint32(surface, 143, 188, 143);
    else if (n == "darkslateblue") return Primitives::rgbToUint32(surface, 72, 61, 139);
    else if (n == "darkslategray") return Primitives::rgbToUint32(surface, 47, 79, 79);
    else if (n == "darkturquoise") return Primitives::rgbToUint32(surface, 0, 206, 209);
    else if (n == "darkviolet") return Primitives::rgbToUint32(surface, 148, 0, 211);
    else if (n == "deeppink") return Primitives::rgbToUint32(surface, 255, 20, 147);
    else if (n == "deepskyblue") return Primitives::rgbToUint32(surface, 0, 191, 255);
    else if (n == "dimgray") return Primitives::rgbToUint32(surface, 105, 105, 105);
    else if (n == "dodgerblue") return Primitives::rgbToUint32(surface, 30, 144, 255);
    else if (n == "firebrick") return Primitives::rgbToUint32(surface, 178, 34, 34);
    else if (n == "floralwhite") return Primitives::rgbToUint32(surface, 255, 250, 240);
    else if (n == "forestgreen") return Primitives::rgbToUint32(surface, 34, 139, 34);
    else if (n == "fuchsia") return Primitives::rgbToUint32(surface, 255, 0, 255);
    else if (n == "gainsboro") return Primitives::rgbToUint32(surface, 220, 220, 220);
    else if (n == "ghostwhite") return Primitives::rgbToUint32(surface, 248, 248, 255);
    else if (n == "gold") return Primitives::rgbToUint32(surface, 255, 215, 0);
    else if (n == "goldenrod") return Primitives::rgbToUint32(surface, 218, 165, 32);
    else if (n == "gray") return Primitives::rgbToUint32(surface, 128, 128, 128);
    else if (n == "green") return Primitives::rgbToUint32(surface, 0, 128, 0);
    else if (n == "greenyellow") return Primitives::rgbToUint32(surface, 173, 255, 47);
    else if (n == "honeydew") return Primitives::rgbToUint32(surface, 240, 255, 240);
    else if (n == "hotpink") return Primitives::rgbToUint32(surface, 255, 105, 180);
    else if (n == "indianred") return Primitives::rgbToUint32(surface, 205, 92, 92);
    else if (n == "indigo") return Primitives::rgbToUint32(surface, 75, 0, 130);
    else if (n == "ivory") return Primitives::rgbToUint32(surface, 255, 255, 240);
    else if (n == "khaki") return Primitives::rgbToUint32(surface, 240, 230, 140);
    else if (n == "lavender") return Primitives::rgbToUint32(surface, 230, 230, 250);
    else if (n == "lavenderblush") return Primitives::rgbToUint32(surface, 255, 240, 245);
    else if (n == "lawngreen") return Primitives::rgbToUint32(surface, 124, 252, 0);
    else if (n == "lemonchiffon") return Primitives::rgbToUint32(surface, 255, 250, 205);
    else if (n == "lightblue") return Primitives::rgbToUint32(surface, 173, 216, 230);
    else if (n == "lightcoral") return Primitives::rgbToUint32(surface, 240, 128, 128);
    else if (n == "lightcyan") return Primitives::rgbToUint32(surface, 224, 255, 255);
    else if (n == "lightgoldenrodyellow") return Primitives::rgbToUint32(surface, 250, 250, 210);
    else if (n == "lightgray") return Primitives::rgbToUint32(surface, 211, 211, 211);
    else if (n == "lightgreen") return Primitives::rgbToUint32(surface, 144, 238, 144);
    else if (n == "lightpink") return Primitives::rgbToUint32(surface, 255, 182, 193);
    else if (n == "lightsalmon") return Primitives::rgbToUint32(surface, 255, 160, 122);
    else if (n == "lightseagreen") return Primitives::rgbToUint32(surface, 32, 178, 170);
    else if (n == "lightskyblue") return Primitives::rgbToUint32(surface, 135, 206, 250);
    else if (n == "lightslategray") return Primitives::rgbToUint32(surface, 119, 136, 153);
    else if (n == "lightsteelblue") return Primitives::rgbToUint32(surface, 176, 196, 222);
    else if (n == "lightyellow") return Primitives::rgbToUint32(surface, 255, 255, 224);
    else if (n == "lime") return Primitives::rgbToUint32(surface, 0, 255, 0);
    else if (n == "limegreen") return Primitives::rgbToUint32(surface, 50, 205, 50);
    else if (n == "linen") return Primitives::rgbToUint32(surface, 250, 240, 230);
    else if (n == "magenta") return Primitives::rgbToUint32(surface, 255, 0, 255);
    else if (n == "maroon") return Primitives::rgbToUint32(surface, 128, 0, 0);
    else if (n == "mediumaquamarine") return Primitives::rgbToUint32(surface, 102, 205, 170);
    else if (n == "mediumblue") return Primitives::rgbToUint32(surface, 0, 0, 205);
    else if (n == "mediumorchid") return Primitives::rgbToUint32(surface, 186, 85, 211);
    else if (n == "mediumpurple") return Primitives::rgbToUint32(surface, 147, 112, 219);
    else if (n == "mediumseagreen") return Primitives::rgbToUint32(surface, 60, 179, 113);
    else if (n == "mediumslateblue") return Primitives::rgbToUint32(surface, 123, 104, 238);
    else if (n == "mediumspringgreen") return Primitives::rgbToUint32(surface, 0, 250, 154);
    else if (n == "mediumturquoise") return Primitives::rgbToUint32(surface, 72, 209, 204);
    else if (n == "mediumvioletred") return Primitives::rgbToUint32(surface, 199, 21, 133);
    else if (n == "midnightblue") return Primitives::rgbToUint32(surface, 25, 25, 112);
    else if (n == "mintcream") return Primitives::rgbToUint32(surface, 245, 255, 250);
    else if (n == "mistyrose") return Primitives::rgbToUint32(surface, 255, 228, 225);
    else if (n == "moccasin") return Primitives::rgbToUint32(surface, 255, 228, 181);
    else if (n == "navajowhite") return Primitives::rgbToUint32(surface, 255, 222, 173);
    else if (n == "navy") return Primitives::rgbToUint32(surface, 0, 0, 128);
    else if (n == "oldlace") return Primitives::rgbToUint32(surface, 253, 245, 230);
    else if (n == "olive") return Primitives::rgbToUint32(surface, 128, 128, 0);
    else if (n == "olivedrab") return Primitives::rgbToUint32(surface, 107, 142, 35);
    else if (n == "orange") return Primitives::rgbToUint32(surface, 255, 165, 0);
    else if (n == "orangered") return Primitives::rgbToUint32(surface, 255, 69, 0);
    else if (n == "orchid") return Primitives::rgbToUint32(surface, 218, 112, 214);
    else if (n == "palegoldenrod") return Primitives::rgbToUint32(surface, 238, 232, 170);
    else if (n == "palegreen") return Primitives::rgbToUint32(surface, 152, 251, 152);
    else if (n == "paleturquoise") return Primitives::rgbToUint32(surface, 175, 238, 238);
    else if (n == "palevioletred") return Primitives::rgbToUint32(surface, 219, 112, 147);
    else if (n == "papayawhip") return Primitives::rgbToUint32(surface, 255, 239, 213);
    else if (n == "peachpuff") return Primitives::rgbToUint32(surface, 255, 218, 185);
    else if (n == "peru") return Primitives::rgbToUint32(surface, 205, 133, 63);
    else if (n == "pink") return Primitives::rgbToUint32(surface, 255, 192, 203);
    else if (n == "plum") return Primitives::rgbToUint32(surface, 221, 160, 221);
    else if (n == "powderblue") return Primitives::rgbToUint32(surface, 176, 224, 230);
    else if (n == "purple") return Primitives::rgbToUint32(surface, 128, 0, 128);
    else if (n == "rebeccapurple") return Primitives::rgbToUint32(surface, 102, 51, 153);
    else if (n == "red") return Primitives::rgbToUint32(surface, 255, 0, 0);
    else if (n == "rosybrown") return Primitives::rgbToUint32(surface, 188, 143, 143);
    else if (n == "royalblue") return Primitives::rgbToUint32(surface, 65, 105, 225);
    else if (n == "saddlebrown") return Primitives::rgbToUint32(surface, 139, 69, 19);
    else if (n == "salmon") return Primitives::rgbToUint32(surface, 250, 128, 114);
    else if (n == "sandybrown") return Primitives::rgbToUint32(surface, 244, 164, 96);
    else if (n == "seagreen") return Primitives::rgbToUint32(surface, 46, 139, 87);
    else if (n == "seashell") return Primitives::rgbToUint32(surface, 255, 245, 238);
    else if (n == "sienna") return Primitives::rgbToUint32(surface, 160, 82, 45);
    else if (n == "silver") return Primitives::rgbToUint32(surface, 192, 192, 192);
    else if (n == "skyblue") return Primitives::rgbToUint32(surface, 135, 206, 235);
    else if (n == "slateblue") return Primitives::rgbToUint32(surface, 106, 90, 205);
    else if (n == "slategray") return Primitives::rgbToUint32(surface, 112, 128, 144);
    else if (n == "snow") return Primitives::rgbToUint32(surface, 255, 250, 250);
    else if (n == "springgreen") return Primitives::rgbToUint32(surface, 0, 255, 127);
    else if (n == "steelblue") return Primitives::rgbToUint32(surface, 70, 130, 180);
    else if (n == "tan") return Primitives::rgbToUint32(surface, 210, 180, 140);
    else if (n == "teal") return Primitives::rgbToUint32(surface, 0, 128, 128);
    else if (n == "thistle") return Primitives::rgbToUint32(surface, 216, 191, 216);
    else if (n == "tomato") return Primitives::rgbToUint32(surface, 255, 99, 71);
    else if (n == "turquoise") return Primitives::rgbToUint32(surface, 64, 224, 208);
    else if (n == "violet") return Primitives::rgbToUint32(surface, 238, 130, 238);
    else if (n == "wheat") return Primitives::rgbToUint32(surface, 245, 222, 179);
    else if (n == "white") return Primitives::rgbToUint32(surface, 255, 255, 255);
    else if (n == "whitesmoke") return Primitives::rgbToUint32(surface, 245, 245, 245);
    else if (n == "yellow") return Primitives::rgbToUint32(surface, 255, 255, 0);
    else if (n == "yellowgreen") return Primitives::rgbToUint32(surface, 154, 205, 50);

    return Primitives::rgbToUint32(surface, 0, 0, 0);
}
