#version 330 core

//location 0 이 VAO attribute 0번을 의미. 
layout (location = 0) in vec3 aPos; 

//setAttribute 1` 에서 설정한 값을 이곳에서 받아 변수 vertexColor에 넣는다. 
layout (location = 1) in vec3 aColor; 

out vec4 vertexColor; 

void main() {

    //glVertexAttribPointer 의 설정값으로 이곳에 데이값 들어옴. 
    gl_Position = vec4(aPos, 1.0); 
    
    //이곳으로 들어온 값은 레스터라이제이션을 거쳐서 프레그먼트 쉐이더로 값을 전달한다. 
    vertexColor =vec4(aColor, 1.0); 
} 