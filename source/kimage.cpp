#include "kimage.h"
#include "stb/stb_image.h"

std::unique_ptr<Kimage> Kimage::load(const std::string &filePath)
{
    auto image =std::unique_ptr<Kimage> (new Kimage()); 
    if(!image ->LoadWidthStb(filePath)) image=nullptr; 

    return std::move(image); 
}

Kimage::~Kimage()
{
    if(m_data) stbi_image_free(m_data); 
}

bool Kimage::LoadWidthStb(const std::string &filePath)
{
    m_data=stbi_load(filePath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if(!m_data){
        SPDLOG_INFO("Failed to load image {}", filePath); 
        return false; 
    } 
    return true; 
}
