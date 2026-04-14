#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex2; //두번째 텍스쳐 

void main() {
    //텍스쳐 2장을 사용하기 위해서 텍스쳐 컬러를 4:1로 섞어서 사용한다. 
    fragColor = texture(tex, texCoord) * 0.8 + texture(tex2, texCoord) * 0.2;
    // fragColor = texture(tex, texCoord);
}