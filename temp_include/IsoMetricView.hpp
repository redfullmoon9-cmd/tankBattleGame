#pragma once 
#include <vector>
#include "MyVector.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderIsoMetric.hpp"

class IsoMetricMap{
private:
    struct Tile {
        int type;  // 0: 빈 공간, 1: 벽, 2: 물
        float height;  // 높이 (0.0 ~ 1.0)
    };
    
    std::vector<std::vector<Tile>> tiles;
    int mapWidth, mapHeight;
    void renderTile(int x, int y); 

public: 
    IsoMetricMap(int w, int h);  
    void render();      

}; 