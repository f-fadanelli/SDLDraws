#ifndef CSVREADER_H
#define CSVREADER_H

#pragma once
#include <string>
#include <vector>
#include <map>

struct ScreenConfig {
    float width = 10;
    float height = 20;
    float metrosX = 30;
    float metrosY = 40;
    std::string color;
};

struct ObjectData {
    std::string type;
    std::map<std::string, std::string> properties;
};

class CSVreader {
public:
    bool readFile(const std::string& filename);

    ScreenConfig getScreenConfig() const;
    std::vector<ObjectData>& getObjects();

private:
    ScreenConfig screenConfig;
    std::vector<ObjectData> objects;

    std::vector<std::string> splitLine(const std::string& line, char delimiter);
};

#endif
