
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
// #include "TankGame.hpp"
#include <iostream>
#include <cmath>
#include "Shader.hpp"
namespace ref{

    int testMain(void)
    {
        // topView, IsoMetric view 시작시에 세팅.
        //  TankGame tankgame("TopView");
        //  tankgame.run();
        //  return 0;
        SPDLOG_INFO(" Window init"); 
    
        if (!glfwInit())
            return -1;
    
        // OpenGL 버전 명시 (3.3 이상 권장)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 새로운 방식으로
    
        GLFWwindow *window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
    
        glfwMakeContextCurrent(window);
    
        // GLEW 초기화 대신 GLAD 초기화 사용 - 현재 Wayland 화면구성에서는 사용못함.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        
        // const GLubyte* glGetString(GLenum name) 반환된 타입은 unsigned char  
        // 현재의 반환타입을 주소는 그대로 두고 signed char로 변환 하기 위해서 포인터를 타입캐스팅. 
        auto glVersion=reinterpret_cast<const char*>(glGetString(GL_VERSION)); 
        SPDLOG_INFO("openGL context version {}",  glVersion); 
        auto glVender =reinterpret_cast<const char*>(glGetString(GL_VENDOR)); 
        SPDLOG_INFO("openGL vender {}",  glVender); 
        auto glShader = reinterpret_cast<const char*>(glGetString(GL_RENDERER)); 
    
    
        // 2. 쉐이더 객체 생성 - 각 파일들을 읽어 온다.
        Shader ourShader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");
    
        // 3. 삼각형 정점 데이터 정의 (NDC 좌표계: -1.0 ~ 1.0)
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // 왼쪽 아래
            0.5f, -0.5f, 0.0f,  // 오른쪽 아래
            0.0f, 0.5f, 0.0f    // 위쪽 중앙
        };
    
        // 4. VAO(Vertex Array Object), VBO(Vertex Buffer Object) 생성 및 설정
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    
        // VAO 바인드 (지금부터 설정하는 VBO 정보가 VAO에 기록됨)
        glBindVertexArray(VAO);
    
        // VBO 바인드 및 데이터 복사
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
        // 정점 속성 설정 (0번 속성, 3개 요소(x,y,z), float타입, 간격, 시작점)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    
        while (!glfwWindowShouldClose(window))
        {
            // glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
    
            // 쉐이더 프로그램 사용
            ourShader.use();
    
            // 저장해둔 VAO 바인드 후 그리기
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
    
            glfwSwapBuffers(window);
            glfwPollEvents();
    
            // 여기에 렌더링 코드 작성
    
            // glfwSwapBuffers(window);
            // glfwPollEvents();
        }
        // 리소스 정리
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    
        glfwTerminate();
        return 0;
    }
}
