#include "IsoMetricView.hpp"

// 26.1.22 
IsoMetricMap::IsoMetricMap(int w, int h) : mapWidth(w), mapHeight(h) {
    tiles.resize(h, std::vector<Tile>(w));
     // 맵 초기화
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            tiles[y][x].type = 0;
            tiles[y][x].height = 0.0f;
        }
    }
}
// 26.1.22 
void IsoMetricMap::render() {
    // 뒤에서 앞으로 그려야 올바른 겹침
    for(int y = mapHeight - 1; y >= 0; y--) {
        for(int x = 0; x < mapWidth; x++) {
            renderTile(x, y);
        }
    }
}

// 26.1.22 
void IsoMetricMap::renderTile(int x, int y){
    auto isoPos = IsoMetricHelper::worldToIso(x * 1.0f, y * 1.0f);
    float h = tiles[y][x].height;

    glPushMatrix();
    {
        glTranslatef(isoPos.x, isoPos.y + h * 0.5f, 0);
        
        // 타일 그리기
        MyRGB color;
        switch(tiles[y][x].type) {
            case 0: color = MyRGB(100, 200, 100); break;  // 풀
            case 1: color = MyRGB(150, 150, 150); break;  // 벽
            case 2: color = MyRGB(100, 100, 255); break;  // 물
        }
        
        // 아이소메트릭 타일 (다이아몬드)
        glBegin(GL_QUADS);
        {
            glColor3fv(color.rgb);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(-0.5f, 0.0f);
            glVertex2f(0.0f, -0.5f);
            glVertex2f(0.5f, 0.0f);
        }
        glEnd();
        
        // 높이가 있으면 측면 그리기
        if(h > 0) {
            glBegin(GL_QUADS);
            {
                // MyRGB darkColor(color.x * 0.7f, color.g * 0.7f, color.b * 0.7f);
                MyRGB darkColor(color.x * 0.7f, color.y * 0.7f, color.z * 0.7f);
                glColor3fv(darkColor.rgb);
                
                // 왼쪽 면
                glVertex2f(-0.5f, 0.0f);
                glVertex2f(-0.5f, -h * 0.5f);
                glVertex2f(0.0f, -0.5f - h * 0.5f);
                glVertex2f(0.0f, -0.5f);
            }
            glEnd();
        }
    }
    glPopMatrix();
        
} 