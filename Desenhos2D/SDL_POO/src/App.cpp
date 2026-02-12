#include "App.h"
#include "Shape.h"
#include "Curve.h"
#include "Polygon.h"
#include "Primitives.h"
#include "CSVreader.h""
#include "House.h"
#include "Sun.h"
#include "Tree.h"
#include "Fence.h"
#include "Ground.h"


App::App(const std::string& title, int w, int h, int mX, int mY)
    : window(nullptr), surface(nullptr), running(false),
      width(w), height(h), mX(mX), mY(mY), title(title) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    surface = SDL_GetWindowSurface(window);
}

App::~App() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::run(const std::string& color, std::vector<ObjectData>& objects) {
    running = true;

    Primitives primitives;

    Uint32 backgroundColor = primitives.getColorByName(surface, color);

    while (running) {

        clear(backgroundColor);

        Uint32 groundColor = primitives.getColorByName(surface, "Verde");
        Ground chao(groundColor);
        chao.draw(surface);

        for (const auto& obj : objects) {

            if(obj.type=="Casa"){
                Uint32 corParede = primitives.getColorByName(surface, obj.properties.at("CorParede"));
                Uint32 corTelhado = primitives.getColorByName(surface, obj.properties.at("CorTelhado"));
                Uint32 corPorta = primitives.getColorByName(surface, obj.properties.at("CorPorta"));
                int x1 = std::stoi(obj.properties.at("LocalizacaoX"));
                int y1 = std::stoi(obj.properties.at("LocalizacaoY"));
                int largura = std::stoi(obj.properties.at("Largura"));
                int altura = std::stoi(obj.properties.at("Altura"));
                float inclinacao = 0.0f;
                auto it = obj.properties.find("Inclinacao");
                if (it != obj.properties.end()) {
                    inclinacao = std::stof(it->second);
                }

                House casa(corParede, corPorta, corTelhado, largura, altura, x1, y1, inclinacao);
                casa.draw(surface,  width, height, mX, mY);
            }

            if(obj.type=="Arvore"){
                Uint32 corFolhas = primitives.getColorByName(surface, obj.properties.at("CorFolhas"));
                Uint32 corTronco = primitives.getColorByName(surface, obj.properties.at("CorTronco"));
                int x1 = std::stoi(obj.properties.at("LocalizacaoX"));
                int y1 = std::stoi(obj.properties.at("LocalizacaoY"));
                int largura = std::stoi(obj.properties.at("Largura"));
                int altura = std::stoi(obj.properties.at("Altura"));
                float inclinacao = 0.0f;
                auto it = obj.properties.find("Inclinacao");
                if (it != obj.properties.end()) {
                    inclinacao = std::stof(it->second);
                }

                Tree arvore(corFolhas, corTronco, largura, altura, x1, y1, inclinacao);
                arvore.draw(surface, width, height, mX, mY);
            }

            if(obj.type=="Cerca"){
                Uint32 corCerca = primitives.getColorByName(surface, obj.properties.at("Cor"));
                int x1 = std::stoi(obj.properties.at("LocalizacaoX"));
                int y1 = std::stoi(obj.properties.at("LocalizacaoY"));
                int largura = std::stoi(obj.properties.at("Largura"));
                int altura = std::stoi(obj.properties.at("Altura"));
                float inclinacao = 0.0f;
                auto it = obj.properties.find("Inclinacao");
                if (it != obj.properties.end()) {
                    inclinacao = std::stof(it->second);
                }

                Fence cerca(corCerca, largura, altura, x1, y1, inclinacao);
                cerca.draw(surface, width, height, mX, mY);
            }

            if(obj.type=="Sol"){
                Uint32 corSol = primitives.getColorByName(surface, obj.properties.at("Cor"));
                int x1 = std::stoi(obj.properties.at("LocalizacaoX"));
                int y1 = std::stoi(obj.properties.at("LocalizacaoY"));
                int largura = std::stoi(obj.properties.at("Largura"));
                int altura = std::stoi(obj.properties.at("Altura"));
                float inclinacao = 0.0f;
                auto it = obj.properties.find("Inclinacao");
                if (it != obj.properties.end()) {
                    inclinacao = std::stof(it->second);
                }

                Sun sol(corSol, largura, altura, x1, y1, inclinacao);
                sol.draw(surface,  width, height, mX, mY);
            }
        }

        handleEvents();

        present();
    }
}

void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            surface = SDL_GetWindowSurface(window);
            width = surface->w;
            height = surface->h;
            std::cout << "Size changed: " << width << ", " << height << "\n";
        }

    }
}

void App::clear(Uint32 color) {
    SDL_FillRect(surface, NULL, color);
}

void App::present() {
    SDL_UpdateWindowSurface(window);
}
