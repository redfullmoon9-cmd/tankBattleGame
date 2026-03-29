#version 330 core
layout (location = 0) in vec3 aPos; //location 0 이 VAO attribute 0번을 의미. 

// out vec4 vertextColor;  

void main() {
    // gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
    gl_Position = vec4(aPos, 1.0); //glVertexAttribPointer 의 설정값으로 이곳에 데이값 들어옴. 
    
    // vertextColor =vec4(0.5, 0.0, 0.0, 1.0); 

} 