# version 330 core
layout (location = 0) in vec3 aPos; // 0번 속성: 위치

void main() {
    // gl_Position은 출력 변수로, 정점의 최종 위치를 저장합니다.
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}