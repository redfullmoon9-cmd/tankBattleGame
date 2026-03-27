#ifndef __KCOMMON_H__
#define __KCOMMON_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

std::optional<std::string> loadTextFile(const std::string fileName); 

#endif //__KCOMMON_H__