#pragma once
#include "MyVector.hpp"
#include "Render.hpp"
#include <vector>

using myVec = MyVector<float>;

// 기본 맵 인터페이스
class GameMap {
public:
    virtual ~GameMap() = default;
    virtual void render() = 0;
    virtual bool isWalkable(const myVec& pos) = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

// Top-Down 맵
class TopDownMap : public GameMap {
private:
    struct Tile {
        int type;  // 0: 빈공간, 1: 벽, 2: 물
        MyRGB color;
    };
    
    std::vector<std::vector<Tile>> tiles;
    int mapWidth, mapHeight;
    float tileSize;
    
public:
    TopDownMap(int w, int h, float size = 0.5f) 
        : mapWidth(w), mapHeight(h), tileSize(size) {
        tiles.resize(h, std::vector<Tile>(w));
        initMap();
    }
    
    void initMap() {
        // 맵 초기화 - 테두리는 벽, 중간은 빈 공간
        for(int y = 0; y < mapHeight; y++) {
            for(int x = 0; x < mapWidth; x++) {
                if(x == 0 || x == mapWidth - 1 || y == 0 || y == mapHeight - 1) {
                    // 테두리
                    tiles[y][x].type = 1;
                    tiles[y][x].color = MyRGB(100, 100, 100);
                } else if((x % 5 == 0 && y % 5 == 0) && (x > 5 && x < mapWidth - 5)) {
                    // 중간에 몇 개 장애물
                    tiles[y][x].type = 1;
                    tiles[y][x].color = MyRGB(150, 100, 50);
                } else {
                    // 빈 공간
                    tiles[y][x].type = 0;
                    tiles[y][x].color = MyRGB(50, 150, 50);
                }
            }
        }
    }
    
    void render() override {
        for(int y = 0; y < mapHeight; y++) {
            for(int x = 0; x < mapWidth; x++) {
                float worldX = (x - mapWidth / 2.0f) * tileSize;
                float worldY = (y - mapHeight / 2.0f) * tileSize;
                
                glPushMatrix();
                {
                    glTranslatef(worldX, worldY, 0);
                    
                    // 타일 그리기
                    PrimitiveRenderer::drawBoxFilled(
                        tiles[y][x].color,
                        tileSize * 0.95f,  // 약간 작게 (격자 효과)
                        tileSize * 0.95f
                    );
                    
                    // 벽은 테두리 추가
                    if(tiles[y][x].type == 1) {
                        PrimitiveRenderer::drawBoxWired(
                            MyRGB(80, 80, 80),
                            tileSize,
                            tileSize
                        );
                    }
                }
                glPopMatrix();
            }
        }
    }
    
    bool isWalkable(const myVec& pos) override {
        // 월드 좌표를 타일 인덱스로 변환
        int x = (int)((pos.x / tileSize) + mapWidth / 2.0f);
        int y = (int)((pos.y / tileSize) + mapHeight / 2.0f);
        
        // 범위 체크
        if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
            return false;
        }
        
        // 벽이 아니면 걸을 수 있음
        return tiles[y][x].type == 0;
    }
    
    int getWidth() const override { return mapWidth; }
    int getHeight() const override { return mapHeight; }
};

// Isometric 맵
class IsometricMap : public GameMap {
private:
    struct Tile {
        int type;
        float height;
        MyRGB color;
    };
    
    std::vector<std::vector<Tile>> tiles;
    int mapWidth, mapHeight;
    
public:
    IsometricMap(int w, int h) : mapWidth(w), mapHeight(h) {
        tiles.resize(h, std::vector<Tile>(w));
        initMap();
    }
    
    void initMap() {
        for(int y = 0; y < mapHeight; y++) {
            for(int x = 0; x < mapWidth; x++) {
                if(x == 0 || x == mapWidth - 1 || y == 0 || y == mapHeight - 1) {
                    tiles[y][x].type = 1;
                    tiles[y][x].height = 0.3f;
                    tiles[y][x].color = MyRGB(120, 120, 120);
                } else {
                    tiles[y][x].type = 0;
                    tiles[y][x].height = 0.0f;
                    tiles[y][x].color = MyRGB(80, 200, 80);
                }
            }
        }
    }
    
    void render() override {
        // 뒤에서 앞으로 그려야 올바른 겹침
        for(int y = mapHeight - 1; y >= 0; y--) {
            for(int x = 0; x < mapWidth; x++) {
                renderTile(x, y);
            }
        }
    }
    
    bool isWalkable(const myVec& pos) override {
        int x = (int)(pos.x + mapWidth / 2.0f);
        int y = (int)(pos.y + mapHeight / 2.0f);
        
        if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
            return false;
        }
        
        return tiles[y][x].type == 0;
    }
    
    int getWidth() const override { return mapWidth; }
    int getHeight() const override { return mapHeight; }
    
private:
    myVec worldToIso(float x, float y) {
        myVec iso;
        iso.x = (x - y) * 0.5f;
        iso.y = (x + y) * 0.25f;
        iso.z = 0.0f;
        return iso;
    }
    
    void renderTile(int x, int y) {
        float worldX = x - mapWidth / 2.0f;
        float worldY = y - mapHeight / 2.0f;
        auto isoPos = worldToIso(worldX, worldY);
        float h = tiles[y][x].height;
        
        glPushMatrix();
        {
            glTranslatef(isoPos.x, isoPos.y, 0);
            
            // 타일 상단 (다이아몬드)
            glBegin(GL_QUADS);
            {
                glColor3fv(tiles[y][x].color.rgb);
                glVertex2f(0.0f, 0.25f + h);
                glVertex2f(-0.5f, 0.0f + h);
                glVertex2f(0.0f, -0.25f + h);
                glVertex2f(0.5f, 0.0f + h);
            }
            glEnd();
            
            // 높이가 있으면 측면 그리기
            if(h > 0.01f) {
                MyRGB darkColor(
                    tiles[y][x].color.r * 0.6f,
                    tiles[y][x].color.g * 0.6f,
                    tiles[y][x].color.b * 0.6f
                );
                
                // 왼쪽 면
                glBegin(GL_QUADS);
                {
                    glColor3fv(darkColor.rgb);
                    glVertex2f(-0.5f, 0.0f + h);
                    glVertex2f(-0.5f, 0.0f);
                    glVertex2f(0.0f, -0.25f);
                    glVertex2f(0.0f, -0.25f + h);
                }
                glEnd();
                
                // 오른쪽 면
                MyRGB darkerColor(
                    tiles[y][x].color.r * 0.4f,
                    tiles[y][x].color.g * 0.4f,
                    tiles[y][x].color.b * 0.4f
                );
                
                glBegin(GL_QUADS);
                {
                    glColor3fv(darkerColor.rgb);
                    glVertex2f(0.5f, 0.0f + h);
                    glVertex2f(0.5f, 0.0f);
                    glVertex2f(0.0f, -0.25f);
                    glVertex2f(0.0f, -0.25f + h);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }
};