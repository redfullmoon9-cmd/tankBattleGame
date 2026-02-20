#make_minimum_required(VERSION 3.10) 
cmake_minimum_required(VERSION 3.31) 
project(TankBattleGame)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

# 1. 소스 파일들을 리스트로 만들기 (source 폴더 내의 모든 cpp)
# file(GLOB_RECURSE SOURCES "source/*.cpp") //glad 추가 이후 설정. 
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")
   
# 2. 패키지 찾기 
find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED) 제거 
find_package(glfw3 REQUIRED)
     
# 3. 실행 파일 생성 (main.cpp가 루트에 있다면 포함, 소스 리스트 추가)
add_executable(TankGame main.cpp ${SOURCES}) 

# 4. 핵심: include 폴더 경로를 컴파일러에게 알려주기 
target_include_directories(TankGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 5. 라이브러리 연결
# target_link_libraries(TankGame OpenGL::GL GLEW::GLEW glfw) //glew
target_link_libraries(TankGame OpenGL::GL glfw)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
