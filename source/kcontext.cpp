#include "kcontext.h"

std::unique_ptr<KContext> KContext::CreateContext(uint32_t mode)
{
    SPDLOG_INFO("create context"); 
    auto context =std::unique_ptr<KContext> (new KContext()); 
    if(mode ==1){ 
        //삼각형 그리기. 
        if(!context->InitRef()) return nullptr; 
    } else if(mode ==2){
        //사각형 그리기. 
        if(!context->Init()) return nullptr; 
        
    }
    return std::move(context); 
}

void KContext::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // 쉐이더 프로그램 사용 
    m_programPtr->Use(); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //사각형 그리기
}

//삼각형 그리기 참조코드 
void KContext::RenderRef()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // 쉐이더 프로그램 사용 
    m_programPtr->Use(); 
    //
    // glDrawArrays(GL_TRIANGLES, 0, 3); //삼각형 그리기. 
    // glDrawArrays(GL_LINE_STRIP, 0, 3); //GL_TRANGLES 대신 
    glDrawArrays(GL_LINE_STRIP, 0, 4); //스트립 삼각형의 경우 점을 4개
}


KContext::~KContext()
{

}

bool KContext::Init()
{
    SPDLOG_INFO(" context init "); 
    //사각형 정점정의 
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };
    
    //이 사각형의 예제에서는 일레멘트(혹 인덱스 어레이 )버퍼를 사용한다. (이전의 삼각형 예에서는 어레이버퍼사용 Array Buffer)
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

//삼각형 그리기 RenderRef 의 그리기 함수와 더불어 
bool KContext::InitRef()
{
    SPDLOG_INFO(" Contex Ref init  "); 
    // 삼각형 정점 데이터 정의 (NDC 좌표계: -1.0 ~ 1.0)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 왼쪽 아래
        0.5f, -0.5f, 0.0f,  // 오른쪽 아래
        0.0f, 0.5f, 0.0f,     // 위쪽 중앙
        -0.5f, -0.5f, 0.0f // 스트립 삼각형의 경우. 첫번째 점을 다시 연결해야. 
    };

    //VAO(Vertex Array Object), VBO(Vertex Buffer Object) 생성 및 바인딩 설정
    // VAO 바인드 (지금부터 설정하는 VBO 정보가 VAO에 기록됨)
    glGenVertexArrays(1, &VAO); //1값은 하나를 의미 VAO를 하나 생성. 
    glBindVertexArray(VAO); //생성한 버텍스 어레이를 바인딩해서 사용할 것임을 알림.  

    // VBO 정점 데이터를 담은 버퍼 오브젝트를 저장 - gpu가  접근가능한 메모리에 복사. 
    //보다 다향한 정보 저장함. 만약 삼각형일 경우, 포지션값외에 컬러값, 법선벡터값 등 다양한 정보가 저장된다. 
    glGenBuffers(1, &VBO);  //버텍스 버퍼를 생성 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// 사용하고자 하는 버퍼가 위에서 생성한 VAO임을 버텍스 버퍼에 바인딩 . 
    
    //실제 데이터를 복사함 GL_STATIC_DRAW 은 버퍼에 세팅한 자료를 변경하지 않을 것임을 옵션으로. 
    //GL_DYNAMIC_DRAW옵션이라면 데이터가 자주 변경될 것이라는 의미로 사용. 

    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 12, vertices, GL_STATIC_DRAW); 

    // 정점 속성 설정 각 파라미터별 의미( 0번 속성, 3개 요소(x,y,z), float타입, 노멀라이스 없고, 간격(stride), 시작점) 
    // 첫번째 파라미터 0은 simple.vs에서 layout (location =0) 에서 0을 의미 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    std::shared_ptr<KShader> vertexShader = KShader::CreateFromFile("shaders/vertex_shader.vs", GL_VERTEX_SHADER);  
    std::shared_ptr<KShader> fragmentShader  = KShader::CreateFromFile("shaders/fragment_shader.fs", GL_FRAGMENT_SHADER);  
    
    if(!vertexShader || !fragmentShader) {
        return false; 
    }

    SPDLOG_INFO(" vertex shader id: {}", vertexShader->Get()); 
    SPDLOG_INFO(" fragment shader id: {}", fragmentShader->Get()); 
    
    m_programPtr =KProgram::CreateProgram({vertexShader, fragmentShader}); 
    if(!m_programPtr) return false; 
    SPDLOG_INFO(" program id {}", m_programPtr->Get()); 

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //한번만 해도. 

    // glBindVertexArray(0);//리스스 해제 -->뭔가 꼬임 왜? 

    return true;
}
