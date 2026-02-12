#include "CSVreader.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

std::vector<std::string> CSVreader::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

bool CSVreader::readFile(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;
    ObjectData currentObject;
    bool readingObject = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        auto tokens = splitLine(line, ';');
        if (tokens.empty()) continue;

        if (tokens.size() == 1) {
            if (readingObject) {
                objects.push_back(currentObject);
                currentObject = ObjectData();
            }
            currentObject.type = tokens[0];
            readingObject = true;
        } else {
            if (readingObject && tokens.size() == 2) {
                currentObject.properties[tokens[0]] = tokens[1];
            }
            else if (readingObject && tokens.size() == 3) {
                currentObject.properties[tokens[0]+"X"] = tokens[1];
                currentObject.properties[tokens[0]+"Y"] = tokens[2];
            }
        }


        lineNumber++;
    }

    if (readingObject) {
        objects.push_back(currentObject);
    }

    for (const auto& obj : objects) {

        if(obj.type=="Tela"){
            screenConfig.color = obj.properties.at("Cor");
            screenConfig.width = std::stof(obj.properties.at("ResolucaoX"));
            screenConfig.height = std::stof(obj.properties.at("ResolucaoY"));
            screenConfig.metrosX = std::stof(obj.properties.at("MetrosX"));
            screenConfig.metrosY = std::stof(obj.properties.at("MetrosY"));
        }
    }

    file.close();
    return true;
}

ScreenConfig CSVreader::getScreenConfig() const {
    return screenConfig;
}

std::vector<ObjectData>& CSVreader::getObjects(){
    return objects;
}
