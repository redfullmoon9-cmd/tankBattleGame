#ifndef __KIMAGE_H__
#define __KIMAGE_H__

#include "kcommon.h"

//테스쳐 파일 불러오기
class Kimage{
public:
    static std::unique_ptr<Kimage> load(const std::string& filePath ); 
    ~Kimage(); 
    const uint8_t* getData() const {return m_data; }
    int getWidth() const { return  m_width; }
    int getHeight() const  { return m_height;  }
    int getChannelCount() const{ return m_channelCount; } 

private:
    Kimage(){}
    bool LoadWidthStb(const std::string& filePath); 
    uint8_t* m_data{nullptr}; 
    int m_width{0}; 
    int m_height{0}; 
    int m_channelCount{0}; 
}; 

#endif //__KIMAGE_H__