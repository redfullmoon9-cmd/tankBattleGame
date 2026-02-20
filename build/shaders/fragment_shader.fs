#version 330 core
out vec4 FragColor; // 최종 출력 색상

void main() {
    // RGBA 순서 (주황색: R=1.0, G=0.5, B=0.2, A=1.0)
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}