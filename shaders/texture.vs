#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//glm을 이용한 변화 
uniform mat4 transform; 

out vec4 vertexColor;
out vec2 texCoord;

void main() {
    // gl_Position = vec4(aPos, 1.0);

    //선형변환을 적용해서 
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = vec4(aColor, 1.0);
    texCoord = aTexCoord;
}