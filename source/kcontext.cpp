#include "kcontext.h"

std::unique_ptr<KContext> KContext::CreateContext()
{
    SPDLOG_INFO("create context"); 
    auto context =std::unique_ptr<KContext> (new KContext()); 
    if(!context->Init()) return nullptr; 
    return std::move(context); 
}

void KContext::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // 쉐이더 프로그램 사용 
    m_programPtr->Use(); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void KContext::RenderRef()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // 쉐이더 프로그램 사용 
    m_programPtr->Use(); 
    glDrawArrays(GL_TRIANGLES, 0, 3); //삼각형 그리기. 
}


KContext::~KContext()
{

}

bool KContext::Init()
{
    //사각형 정점정의 
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };
    
    uint32_t indices[] = { // 삼각형 2개로 사각형을 그림
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexPtr=KVertex::CreateVertex(); 
    m_vertexBufferPtr=KBuffer::CreateBufferWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*12); 
    m_vertexPtr->setAttribute(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0); 
    
    m_indexBufferPtr=KBuffer::CreateBufferWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6 ); 

    
    // 2. 쉐이더 객체 생성 - 각 파일들을 읽어 온다.
    std::shared_ptr<KShader> vertexShader = KShader::CreateFromFile("shaders/simple.vs", GL_VERTEX_SHADER);  
    std::shared_ptr<KShader> fragmentShader  = KShader::CreateFromFile("shaders/simple.fs", GL_FRAGMENT_SHADER);  
    
    if(!vertexShader || !fragmentShader) {
        return false; 
    }
    
    SPDLOG_INFO(" vertex shader id: {}", vertexShader->Get()); 
    SPDLOG_INFO(" fragment shader id: {}", fragmentShader->Get()); 
    
    m_programPtr =KProgram::CreateProgram({vertexShader, fragmentShader}); 
    if(!m_programPtr) return false; 
    SPDLOG_INFO(" program id {}", m_programPtr->Get()); 
    
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f); //한번만 .. 해도. 
    // m_vertexPtr->UnBind(); 

    return true;
}

//삼각형 그리기 
bool KContext::InitRef()
{
    SPDLOG_INFO(" Contex init "); 
    // 삼각형 정점 데이터 정의 (NDC 좌표계: -1.0 ~ 1.0)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 왼쪽 아래
        0.5f, -0.5f, 0.0f,  // 오른쪽 아래
        0.0f, 0.5f, 0.0f    // 위쪽 중앙
    };

    // unsigned int VAO, VBO; 

    //VAO(Vertex Array Object), VBO(Vertex Buffer Object) 생성 및 바인딩 설정
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VAO 바인드 (지금부터 설정하는 VBO 정보가 VAO에 기록됨)
    // VBO 바인드 및 데이터 복사
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 정점 속성 설정 (0번 속성, 3개 요소(x,y,z), float타입, 간격, 시작점)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //한번만 .. 해도. 

    glBindVertexArray(0);//리스스 해제 

    return false;
}
