#define SDL_MAIN_HANDLED
#include "App.h"
#include "CSVreader.h"
#include <string>

int main() {

    CSVreader reader;

    std::string file = "CSVs/Exemplo.csv";
    //std::string file = "CSVs/Entrega.csv";

    if (!reader.readFile(file)) {
        std::cerr << "Erro ao ler o arquivo CSV." << std::endl;
        return 0;
   }

    auto& objects = reader.getObjects();

    ScreenConfig config = reader.getScreenConfig();
    std::cout << "Tela: " << config.width << "x" << config.height
            << ", Metros: " << config.metrosX << "x" << config.metrosY
              << ", Cor: " << config.color << std::endl;

    App app("Desenhos 2D", config.width, config.height, config.metrosX, config.metrosY);
    app.run(config.color, objects);
    return 0;
}



