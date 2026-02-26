#include "Game2Dref.hpp"

namespace refcode{

    Game2D::Game2D(const std::string &_title, const int &_width, const int &_height, const std::vector<float> _color) 
        : title(_title), width(_width), height(_height), color(_color) {
    }
    
    Game2D::~Game2D() {
        // 리소스 해제
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (basicShader) delete basicShader;
        
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
    
    Game2D &Game2D::init() {
        // 기본값으로 init 호출
        return init(this->title, this->width, this->height, this->color);
    }
    
    Game2D &Game2D::init(const std::string &title, const int &width, const int &height, std::vector<float> vec) {
        std::cout << " in Game2D init " << std::endl;
    
        if (!glfwInit()) {
            reportError(__FUNCTION__, "glfwinit Failed");
            exit(-1);
        }
    
        // OpenGL 버전 설정 (Core Profile)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window) {
            reportError(__FUNCTION__, "Window creation failed");
            glfwTerminate();
            exit(-1);
        }
    
        glfwMakeContextCurrent(window);
    
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            reportError(__FUNCTION__, "GLAD initial failed");
            exit(-1);
        }
    
        // Viewport 설정 (매 프레임 호출하는 것보다 여기서 한 번 설정하는 것이 효율적)
        glViewport(0, 0, width, height);
    
        // 쉐이더 초기화 (경로는 프로젝트 설정에 맞게 수정 필요)
        basicShader = new Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");
    
        // 기본 VAO/VBO 설정 (테스트용 삼각형 데이터 등)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    
        return *this;
    }
    
    Game2D &Game2D::run() {
        if (!window) {
            reportError(__FUNCTION__, "Window is null. Call init() first.");
            return *this;
        }
    
        while (!glfwWindowShouldClose(window)) {
            // 배경색 설정 (RGBA)
            glClearColor(color[0], color[1], color[2], color.size() > 3 ? color[3] : 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
    
            // 상속받은 클래스에서 정의할 로직들 실행
            update();
            
            // 쉐이더 사용 및 그리기 예시
            basicShader->use();
            render();
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    
        return *this;
    }
    
    void Game2D::reportError(const std::string& functionName, const std::string& message) {
        std::cerr << "[ERROR in " << functionName << "]: " << message << std::endl;
    }
}