#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <CSVreader.h>
#include <string>
#include <iostream>
#include <vector>


class App {
private:
    SDL_Window* window;
    SDL_Surface* surface;
    bool running;
    int width, height, mX, mY;
    std::string title;

public:
    App(const std::string& title, int w, int h, int mX, int mY);
    ~App();

    void run(const std::string& color, std::vector<ObjectData>& objects);
    void handleEvents();
    void clear(Uint32 color);
    void present();

    SDL_Surface* getSurface() { return surface; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getMX() const { return mX; }
    int getMY() const { return mY; }
};

#endif
