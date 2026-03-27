#version 330 core
uniform vec4 color;
// in vec4 vertextColor;  //vs로 입력된 변수., 
out vec4 fragColor; //최종 출력 색상. 

void main() {
    // fragColor = vec4(1.0, 0.0, 1.0, 1.0);
    fragColor = color; //프로그램 코드에서 직접 입력하는 방식으로 세팅. 
}