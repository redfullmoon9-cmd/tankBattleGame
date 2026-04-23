cmake_minimum_required(VERSION 3.10) 
set(PROJECT_NAME TankBattleGame)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

# 1. 소스 파일들을 리스트로 만들기 (source 폴더 내의 모든 cpp)
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")
   
# 2. 패키지 찾기 
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# 방법 A: 소스를 직접 포함 (가장 빠름)
set(SPDLOG_BUILD_ALL OFF) 
add_subdirectory(external/spdlog)

# 3. 실행 파일 생성 (main.cpp가 루트에 있다면 포함, 소스 리스트 추가)
add_executable(${PROJECT_NAME} main.cpp testmain.cpp ${SOURCES}) 

# 4. include 폴더 경로를 컴파일러에게 알려주기 
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/stb
)

# 5. 라이브러리 연결
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw spdlog::spdlog glm::glm)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
