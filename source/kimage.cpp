#include "kimage.h"
#include "stb/stb_image.h"

std::unique_ptr<Kimage> Kimage::load(const std::string &filePath)
{
    auto image =std::unique_ptr<Kimage> (new Kimage()); 
    if(!image ->LoadWidthStb(filePath)) image=nullptr; 

    return std::move(image); 
}

std::unique_ptr<Kimage> Kimage::Create(int width, int height, int channelCount)
{
    auto image= std::unique_ptr<Kimage> (new Kimage());
    if(!image->Allocate(width, height, channelCount)){
        return nullptr; 
    } 
    return std::move(image); 
}

Kimage::~Kimage()
{
    if(m_data) stbi_image_free(m_data); 
}

void Kimage::setCheckImage(int gridX, int gridY)
{
    for (int j =0; j < m_height; j++){
        for( int i =0; i<m_width; i++){
            int pos =(j* m_width+i) *m_channelCount; 
            bool even =((i/gridX) + (j/gridY)) %2 ==0 ; 
            uint8_t value = even ? 255 :0; //255 횐색 0 검은색. 
            for (int k =0; k<m_channelCount; k++){
                m_data[pos+k] =value; 
                if(m_channelCount >3) m_data[3]=255; 
            }
        }
    }
}

bool Kimage::LoadWidthStb(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(true); //상하 위치가 반전되어 나오는 것을 바로 나오게 
    m_data=stbi_load(filePath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if(!m_data){
        SPDLOG_INFO("Failed to load image {}", filePath); 
        return false; 
    } 
    return true; 
}

bool Kimage::Allocate(int width, int height, int channelCount)
{
    m_width =width; 
    m_height=height; 
    m_channelCount = channelCount; 
    m_data=(uint8_t*)malloc(m_width* m_height* m_channelCount); 
    return m_data ? true: false;
}
