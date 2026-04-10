
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "kcontext.h"

/** 강원대 openGL강좌 테스트 플랫폼.  */
namespace ref{

    void OnFrameBuffersizeChange(GLFWwindow* window, int width, int height){
        SPDLOG_INFO("Frame size changed: ( {} x {})", width, height); 
        glViewport(0, 0, width, height); 
    }

    void OnKeyEvent(GLFWwindow* window,
        int key, int scancode, int action, int mods) {
        SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
            key, scancode,
            action == GLFW_PRESS ? "Pressed" :
            action == GLFW_RELEASE ? "Released" :
            action == GLFW_REPEAT ? "Repeat" : "Unknown",
            mods & GLFW_MOD_CONTROL ? "C" : "-",
            mods & GLFW_MOD_SHIFT ? "S" : "-",
            mods & GLFW_MOD_ALT ? "A" : "-");
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    GLFWwindow* window=nullptr; 
    unsigned int WINDOW_WIDTH=800, WINDOW_HEIGHT=600; 

    int testMain(void)
    {
        SPDLOG_INFO(" Window init"); 
        
        if (!glfwInit())
            return -1;

        // OpenGL 버전 명시 (3.3 이상 권장)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 새로운 방식으로

        GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello OpenGL", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        
        glfwMakeContextCurrent(window);
        
        // GLEW 초기화 대신 GLAD 초기화 사용 - 현재 Wayland 화면구성에서는 glew 사용못함.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return -1;
        }

        // const GLubyte* glGetString(GLenum name) 반환된 타입은 unsigned char  
        // 현재의 반환타입을 주소는 그대로 두고 signed char로 변환 하기 위해서 포인터를 타입캐스팅. 
        // auto glVersion=reinterpret_cast<const char*>(glGetString(GL_VERSION)); 
        // SPDLOG_INFO("openGL context version {}",  glVersion); 
        // auto glVender =reinterpret_cast<const char*>(glGetString(GL_VENDOR)); 
        // SPDLOG_INFO("openGL vender {}",  glVender); 
        // auto glShader = reinterpret_cast<const char*>(glGetString(GL_RENDERER)); 
        // SPDLOG_INFO("openGL shader {}", glShader); 

        //callback fuction 추가. 
        OnFrameBuffersizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT); 
        glfwSetFramebufferSizeCallback(window, OnFrameBuffersizeChange); 
        glfwSetKeyCallback(window, OnKeyEvent);

        //1: 삼각형 2: 사각형. 3.리펙토링 이후의 사각형.  
        uint32_t mode =3; 

        SPDLOG_INFO(" after refactoring, draw rectangle   mode = {} ", mode); 
        auto context =KContext::CreateContext(mode); 
        if(!context){
            SPDLOG_INFO("failed to create context "); 
            glfwTerminate(); 
            return -1; 
        }
        
        while (!glfwWindowShouldClose(window))
        {
            // 여기에 렌더링 코드 작성
            if(mode ==1) { //삼각형 그리기 
                context->RenderRef();

            }else if(mode == 2){
                context->RenderRef2(); 
                
            }
            else if(mode ==3 ) { //refactoring 사각형 그리기 
                context->Render();  
                // context->RenderRef3(); //쉐이더 파일을 이용한 색상변화 
            }
            //프론트 버퍼와 백버퍼를 바뀌치는 방식으로 그린다. 더블 버퍼링방식, 부드러운 화면이 가능.  
            glfwSwapBuffers(window); 
            glfwPollEvents();
            
        }
        context=nullptr; 
        // 리소스 정리
        // glDeleteVertexArrays(1, &VAO);
        // glDeleteBuffers(1, &VBO);

        glfwTerminate();
        return 0;

    }
}
