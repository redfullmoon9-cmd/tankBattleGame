#version 330 core
in vec4 vertexColor;  // 입력으로 받은 컬러값을 프레임 버퍼에 세팅하게 한다. 
out vec4 fragColor; //최종 출력 색상. 

void main() {
    fragColor = vertexColor; //프로그램 코드에서 직접 입력하는 방식으로 세팅. 
}