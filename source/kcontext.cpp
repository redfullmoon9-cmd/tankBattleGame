#include "kcontext.h"
#include "kimage.h"
#include "ktexture.h"

std::unique_ptr<KContext> KContext::CreateContext(uint32_t mode)
{
    SPDLOG_INFO("create context"); 
    auto context =std::unique_ptr<KContext> (new KContext()); 
    if(mode ==1){ 
        //삼각형. 사각형 그리기. 
        if(!context->InitRef1()) return nullptr; 
    } else if(mode ==2){
        // 인덱스 버퍼를 이용한 사각형 그리기. 
        if(!context->InitRef2()) return nullptr; 
        
    }else if(mode ==3){
        //refactoring,  사각형 그리기. 
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
    // glDrawArrays(GL_LINE_STRIP, 0, 4); //스트립 삼각형의 경우 점을 4개
    // glDrawArrays(GL_LINE_STRIP, 0, 6); //스트립 삼각형을 2개 사각형을 그리므로  정점을 6개
    glDrawArrays(GL_TRIANGLES, 0, 6); //스트립 삼각형을 2개 그려 사각형을 그리므로  정점을 6개

    // --> 사각형을 그리기 위해서 정점을 6개 사용하므로 2개가 낭비  glDrawArrays 함수를 사용하게 될경우 문제 생긴다. 
    // 그래서.정점을 재활용하기 위한 버퍼 Elemnet Buffer Object(EBO), 인덱스 버퍼와 glDrawElements 로 사각형 그리기..--> RenderRef2() 로 
}

//인덱스 버퍼를사용해서 사각형 그리기. 
void KContext::RenderRef2()
{
    glClear(GL_COLOR_BUFFER_BIT); 
    // 쉐이더 프로그램 사용 
    m_programPtr->Use(); 
    //인덱스 버퍼사용할 것이므로 glDrawArrays를 사용하지 않음. 
    //primitive, 인덱스의 갯수, 데이터 타입, 오프셋/포인터 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //사각형 그리기. 6은 인덱스의 갯수. 
}

//60분의 1초 실행되는 애니메이션 그리기 
void KContext::RenderRef3()
{
    glClear(GL_COLOR_BUFFER_BIT);
    static float time=0.0f; 
    float t =sinf(time) *0.5f + 0.5f; 
    auto loc=glGetUniformLocation(m_programPtr->Get(), "color"); 
    m_programPtr->Use(); 
    glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);  
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //사각형 그리기

    time += 0.016f; 

}

KContext::~KContext()
{

}

//glm을 위한 테스트 코드. 
void KContext::testGlm()
{
    //위치 (1, 0, 0)에 대한 동차좌표계 사용하기. 
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); 
    // glm::mat4(1.0f) 대각성분이 1로 만들어진 단위 행렬을 구성하고, glm:vec3(1,1,0) 만큼(x, y 를 각각 1만큼) 평행이동
    auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)); 
    //단위 행렬기준 glm::radians() 값 만큼 z 축으로 회전 
    auto rot=glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //단위 행렬값의 크기를 각각 3만큼 확대  시키. 
    auto scale= glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)); 
    vec= trans* rot* scale * vec; 
    
    SPDLOG_INFO("transformed vec : [{}, {}, {}]", vec.x, vec.y, vec.z); 
}

// 텍스쳐 정보 가져와서 읽기. 정점정보에 xyx rgb값 이외에 텍스쳐 정보
bool KContext::Init()
{
    SPDLOG_INFO(" context init  mode =3, 텍스쳐 그리기"); 

    //정점 정보는 텍스쳐를 담기 위해서 x,y,z,r,g,b, s, t 값을 저장해야. s,t는 텍스쳐 코디네이트 
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f 
    };
    // 위 정점의 인덱스를 아래 구조체에서 저장 처음 정점 0 1 3 을 사용해서 삼각형 하나를 그리고, 다음 정점 인덱스 1 2 3 으로 그림. 
    //이 사각형의 예제에서는 일레멘트(혹 인덱스 어레이 )버퍼를 사용한다. (이전의 삼각형 예에서는 어레이버퍼사용 Array Buffer)
    uint32_t indices[] = { // 삼각형 2개로 사각형을 그림
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexPtr=KVertex::CreateVertex(); 
    m_vertexBufferPtr=KBuffer::CreateBufferWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*32); 


    //vertices가 어떤 모양새로 생겼는지를 기술해 주는 부분. 
    //어트리뷰트 0을 정점, 1은 컬러값을 가져 가도록, stride, offset 값 기술
    //어트리뷰트 2는 텍스쳐에 관한 정보이고 텍스쳐 코디네이트는 2차원, 앞에서 6개의 정보를 뛰어 넘어야 처음 나온다. 
    m_vertexPtr->setAttribute(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0); 
    m_vertexPtr->setAttribute(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*3); 
    m_vertexPtr->setAttribute(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, sizeof(float)*6); 
    
    m_indexBufferPtr=KBuffer::CreateBufferWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6 ); 

    
    // 2. 쉐이더 객체 생성 - 각 파일들을 읽어 온다.
    std::shared_ptr<KShader> vertexShader = KShader::CreateFromFile("shaders/texture.vs", GL_VERTEX_SHADER);  
    std::shared_ptr<KShader> fragmentShader  = KShader::CreateFromFile("shaders/texture.fs", GL_FRAGMENT_SHADER);  
    
    if(!vertexShader || !fragmentShader) {
        return false; 
    }
    
    SPDLOG_INFO(" vertex shader id: {}", vertexShader->Get()); 
    SPDLOG_INFO(" fragment shader id: {}", fragmentShader->Get()); 
    
    m_programPtr =KProgram::CreateProgram({vertexShader, fragmentShader}); 
    if(!m_programPtr) return false; 
    SPDLOG_INFO(" program id {}", m_programPtr->Get()); 

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f); 

    auto image =Kimage::load("resources/image/container.jpg");
    if(!image) return false; 
    SPDLOG_INFO("image {}X{}, {}channels", image->getWidth(), image->getHeight(), image->getChannelCount());  
    
    //체커 이미지를 생성한다. 
    // auto image =Kimage::Create(512, 512); 
    // image->setCheckImage(16, 16); 
    
    //texture --> kTexture class 로 이동
    m_texturePtr= KTexture::CreateFromImage(image.get()); 
    
    //현재는 마지막 것만 나온다. 이유는 VS FS 쪽에서 하나만 선언하고 있기에. 
    auto image2=Kimage::load("resources/image/awesomeface.png"); 
    if(!image2) return false; 
    SPDLOG_INFO("image2 {}X{}, {}channels", image->getWidth(), image->getHeight(), image->getChannelCount());  
    
    m_texturePtr2= KTexture::CreateFromImage(image2.get()); 

    //여러장의 텍스쳐를 사용하기 위해서. 
    //텍스쳐 슬롯의 번호를 가르쳐 준다. 0번 슬롯. 
    glActiveTexture(GL_TEXTURE0); 
    //2d 테스쳐를 처음에 만든 텍스쳐 아이디 1 에 바인딩. 
    glBindTexture(GL_TEXTURE_2D, m_texturePtr->Get()); 

    //1번 슬롯으로 다시 바꾼다. 어썸페이스 
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, m_texturePtr2->Get()); 
    
    m_programPtr->Use(); 

    glUniform1i(glGetUniformLocation(m_programPtr->Get(), "tex"), 0); 
    glUniform1i(glGetUniformLocation(m_programPtr->Get(), "tex2"), 1); 

    //GLM test code 
    // testGlm(); 
    // auto transform=glm::mat4(1.0f); //메트릭스가 단위 행렬이 된다. 

    // x축으로 0.3 만큼 이동
    // auto transform=glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.2f, 0.0f) ); 

    // 0.5만큼 축소하고, z축을 기준으로 90도 만큼 회전 시키기
    auto transform = glm::rotate(
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), 
        glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.1f)
    ); 

    //texture.vs에 사용된 변수 명 transform으로 찾는다. 
    auto transformLoc=glGetUniformLocation(m_programPtr-> Get(), "transform"); 
    //트랜스폼 매트릭스의 값에 다음의 값들을 세팅힌다. 
    // --> LOC의 위치,  1 메트릭스 하나, 트랜스포즈여부, 밸류의 포인터 값을 얻어와서 gpu에 세팅
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform) ); 

    return true;
}

// 정점 3개와 4개로 삼각형 그리기, 삼각형 두개로 사각형 그리기. 
//RenderRef 의 그리기 함수와 더불어 사용됨. 
bool KContext::InitRef1()
{
    SPDLOG_INFO(" Contex Ref init  "); 
    // 삼각형 정점 데이터 정의 (NDC 좌표계: -1.0 ~ 1.0)
    // 일반 삼각형 정점 3개 
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, // 왼쪽 아래
    //     0.5f, -0.5f, 0.0f,  // 오른쪽 아래
    //     0.0f, 0.5f, 0.0f     // 위쪽 중앙
    // };

    // 삼각형 스트립의 경우 정점 4개 사용. 
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, // 왼쪽 아래
    //     0.5f, -0.5f, 0.0f,  // 오른쪽 아래
    //     0.0f, 0.5f, 0.0f,     // 위쪽 중앙
    //     -0.5f, -0.5f, 0.0f // 스트립 삼각형의 경우. 첫번째 점을 다시 연결해야. 
    // };


    //삼각형 두개로 사각형 그리기 
    float vertices[] = { 
        //first triangle. 
        -0.5f, 0.5f, 0.0f, // top left                
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        //second triange
        -0.5f, 0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, -0.5f, 0.0f // bottom right
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

    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); /정점 3개의 /삼각형 
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 12, vertices, GL_STATIC_DRAW); //삼각형 스트립을 표현 할 것이므로 정점 3 * 4 =12
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 18, vertices, GL_STATIC_DRAW); //삼각형 2개로 사각형를 표현 할 것이므로 정점 3 * 6 =18

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

//사각형를 4개의 정점과 인텍스 버퍼를 사용해서 그리기. 
//삼각형 그리기 RenderRef2와 함께 사용됨. 
bool KContext::InitRef2()
{
    SPDLOG_INFO(" Contex Ref2 init  "); 

    //삼각형 두개로 사각형 그리기  인덱스 버퍼를 사용해서 정정을 다시 사용한다. 
    //사각형 정점 정의 4개만 정의 하고 아래에서 인덱스를 사용한다.      
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };

    // 위 정점의 인덱스를 아래 구조체에서 저장 처음 정점 0 1 3 을 사용해서 삼각형 하나를 그리고, 다음 정점 인덱스 1 2 3 으로 그림. 
    //이 사각형의 예제에서는 일레멘트(혹 인덱스 어레이 )버퍼를 사용한다. (이전의 삼각형 예에서는 어레이버퍼사용 Array Buffer)
    uint32_t indices[] = { // 삼각형 2개로 사각형을 그림
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    /**
     * VAO(Vertex Array Object), VBO(Vertex Buffer Object) 생성 및 바인딩 설정 
     */
    // VAO 바인드 (지금부터 설정하는 VBO 정보가 VAO에 기록됨)
    glGenVertexArrays(1, &VAO); //1값은 하나를 의미 VAO를 하나 생성. 
    glBindVertexArray(VAO); //생성한 버텍스 어레이를 바인딩해서 사용할 것임을 알림.  

    // VBO 정점 데이터를 담은 버퍼 오브젝트를 저장 - gpu가  접근가능한 메모리에 복사. 
    //보다 다향한 정보 저장함. 만약 삼각형일 경우, 포지션값외에 컬러값, 법선벡터값 등 다양한 정보가 저장된다. 
    glGenBuffers(1, &VBO);  //버텍스 버퍼를 생성 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// 사용하고자 하는 버퍼가 위에서 생성한 VAO임을 버텍스 버퍼에 바인딩 . 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float )*12 , vertices, GL_STATIC_DRAW); //삼각형 2개로 사각형를 표현 할 것이므로 정점 3 * 6 =18
    
    // 정점 속성 설정 각 파라미터별 의미( 0번 속성, 3개 요소(x,y,z), float타입, 노멀라이스 없고, 간격(stride), 시작점) 
    // 첫번째 파라미터 0은 simple.vs에서 layout (location =0) 에서 0을 의미 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void *)0);
   
    /* 
     * 인덱스 버퍼, 엘레먼트 버퍼 오븍젝트 생성 
     */

    //인덱스 버퍼 사용. 버텍스 버퍼와 달리 정수값이 들어올 것이므로 glVertexAttribPointer나 glEnableVertexAttribArray 필요 없음. 
    glGenBuffers(1, &m_indexBufferObejct);  // 인덱스 버퍼를 생성 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObejct);// 사용하고자 하는 버퍼EBO를 바인딩 . 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t )* 6, indices, GL_STATIC_DRAW); //삼각형 2개로 사각형를 표현 할 것이므로 정점 3 * 6 =18



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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //한번만 해도. 

    // glBindVertexArray(0);//리스스 해제 -->뭔가 꼬임 왜? 

    return true;
}

//리팩토링 이후, vertextLayout, program, shader등의 클래스로 분리 
// 유니폼 변수 사용해서 simple.fs 에서 설정값 읽어 오기. 
bool KContext::InitRef3()
{
     SPDLOG_INFO(" context init  mode =3, 인덱스 버퍼를 사용해서 사각형 그리기, refactoring 된 버전"); 

    //사각형 정점 정의 4개만 정의 하고 아래에서 인덱스를 사용한다.      
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };
    // 위 정점의 인덱스를 아래 구조체에서 저장 처음 정점 0 1 3 을 사용해서 삼각형 하나를 그리고, 다음 정점 인덱스 1 2 3 으로 그림. 
    //이 사각형의 예제에서는 일레멘트(혹 인덱스 어레이 )버퍼를 사용한다. (이전의 삼각형 예에서는 어레이버퍼사용 Array Buffer)
    uint32_t indices[] = { // 삼각형 2개로 사각형을 그림
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexPtr=KVertex::CreateVertex(); 
    // m_vertexBufferPtr=KBuffer::CreateBufferWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*18); 
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

    //shader 파일에서 읽어온다. simple.vs 와 simple.fs
    // simple.fs에서 uniform 변수 color 값을 읽어온다. 
    auto loc=glGetUniformLocation(m_programPtr->Get(), "color"); 
    m_programPtr->Use(); 
    //위에서 얻어온  color라는 변수 값에 1.0f, 1.0f, 0.0f, 1.0f 값을 전달. 
    glUniform4f(loc, 1.0f, 0.0f, 0.0f, 1.0f);  
     
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f); //한번만 .. 해도. 

    return true;
}

//인덱스 버퍼를 사용해서 사각형 그리기, refactoring 된 버전 
// 버텍스 정보에 여러가지 담기. vertices정보값이 처음은 좌표 정보, 그다음이 컬러 정보로 구성, 
// per_vertex_color.vs fs를 이용해서 정점별로 색상이 변하는 예제 
bool KContext::InitRef4()
{
    SPDLOG_INFO(" context init  mode =3, 정점좌표와 컬러 좌표 사용해서 사각형 그리기, refactoring 된 버전"); 

    //사각형 정점 정의 4개만 정의 하고 아래에서 인덱스를 사용한다.
    // 처음 3개는 정점 정도, 그 나음 3개는 컬러 정보      
    float vertices[] = { 
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right, red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom right, green 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.1f, // bottom left, blue 
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f// top left, yellow
    };
    // 위 정점의 인덱스를 아래 구조체에서 저장 처음 정점 0 1 3 을 사용해서 삼각형 하나를 그리고, 다음 정점 인덱스 1 2 3 으로 그림. 
    //이 사각형의 예제에서는 일레멘트(혹 인덱스 어레이 )버퍼를 사용한다. (이전의 삼각형 예에서는 어레이버퍼사용 Array Buffer)
    uint32_t indices[] = { // 삼각형 2개로 사각형을 그림
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexPtr=KVertex::CreateVertex(); 
    m_vertexBufferPtr=KBuffer::CreateBufferWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*24); 


    //vertices가 어떤 모양새로 생겼는지를 기술해 주는 부분. 
    //어트리뷰트 0을 정점, 1은 컬러값을 가져 가도록, stride, offset 값 기술
    m_vertexPtr->setAttribute(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0); 
    m_vertexPtr->setAttribute(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*6, sizeof(float)*3); 
    
    m_indexBufferPtr=KBuffer::CreateBufferWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6 ); 

    
    // 2. 쉐이더 객체 생성 - 각 파일들을 읽어 온다.
    std::shared_ptr<KShader> vertexShader = KShader::CreateFromFile("shaders/per_vertex_color.vs", GL_VERTEX_SHADER);  
    std::shared_ptr<KShader> fragmentShader  = KShader::CreateFromFile("shaders/per_vertex_color.fs", GL_FRAGMENT_SHADER);  
    
    if(!vertexShader || !fragmentShader) {
        return false; 
    }
    
    SPDLOG_INFO(" vertex shader id: {}", vertexShader->Get()); 
    SPDLOG_INFO(" fragment shader id: {}", fragmentShader->Get()); 
    
    m_programPtr =KProgram::CreateProgram({vertexShader, fragmentShader}); 
    if(!m_programPtr) return false; 
    SPDLOG_INFO(" program id {}", m_programPtr->Get()); 

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f); //한번만 .. 해도. 

    return true;
}