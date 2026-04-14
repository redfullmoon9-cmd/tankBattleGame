#ifndef __KIMAGE_H__
#define __KIMAGE_H__

#include "kcommon.h"

//테스쳐 파일 불러오기
class Kimage{
public:
    static std::unique_ptr<Kimage> load(const std::string& filePath ); 
    static std::unique_ptr<Kimage> Create(int width, int height, int channelCount=4); //add 4.14
    ~Kimage(); 
    const uint8_t* getData() const {return m_data; }
    int getWidth() const { return  m_width; }
    int getHeight() const  { return m_height;  }
    int getChannelCount() const{ return m_channelCount; } 

    void setCheckImage(int gridX, int gridY); //체커보드 이미지를 만든다. 

private:
    Kimage(){}
    bool LoadWidthStb(const std::string& filePath); 
    bool Allocate(int width, int height, int channelCount); 
    
    uint8_t* m_data{nullptr}; 
    int m_width{0}; 
    int m_height{0}; 
    int m_channelCount{0}; 
}; 

#endif //__KIMAGE_H__