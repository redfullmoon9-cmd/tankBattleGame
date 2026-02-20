#pragma once 
// #ifndef RENDER_HPP
// #define RENDER_HPP

// #include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MyVector.hpp"
#include <vector>
#include <cmath>
#include "ComponentType.hpp"

// 1.22 추가 --> 1.24 render 이동 
class TankRendererComponent{
    public: 
    //기본 테스트 버전 
    void render();  
    ///포탑회전 보관용 버전 
    void render(std::string st, const TankApperance& tankApp, MyVector<float>& center, const float& rotation); 

    //포탑회전,  파라미터 간소화를 구조체 버전
    void render(const TankApperance& tankApp, MyVector<float>& center, const float& rotation); 

}; 

/** 1.9 그림기 함수 모두를 전역구조로 MyGame2D로 독립적으로 모아 놓은 방식으로 구조변경.  */
namespace PrimitiveRenderer{
    using myVec=MyVector<float>; 
    
    //라디언 구하기 gl에서 degree 대신 라디안 값 사용. 
    inline float getMyRadian(const float& degree){
        return degree / 180.0f * 3.141592f;   
    }

    //makeRegularConvexPolygon() 대신에 꼭짓점 구하기 함수 분석용으로 추가. 
    inline std::vector<myVec> makeConvex(const float& radius, const float& start_theta){
		const float pi= 3.141592f; 
		//꼭짓점. 
		const int convex=5; 
		std::vector<myVec> vertices; 
		vertices.resize(convex); 

		// 2pi안에 꼭짓점이 5개 일때 각각의 degree=theta. 값. 
		const float d_theta  = pi* 2.0f / static_cast<float>(convex);
		float theta =getMyRadian(start_theta); 

		for(int i =0; i< convex; i++){
			vertices[i]= myVec{ radius * cosf(theta), radius * sinf(theta), 0.0f}; 
			theta +=d_theta; 
		}
		return vertices; 
    }

        
    // 한변의 길이와 변의 숫자를 입력 받아서 외접한 원의 radius 반지름을 구하기 
    inline float edgeToRadius(const float edge, const int side){
        float angle =3.141592f/ side; // π/n 
        return edge/ (2.0f * sinf(angle)); //일반화된 공식 -> a / (2sin(π/n))
        /*
            float radius_triangle = edgeToRadius(0.25f, 3);  // 삼각형
            float radius_square = edgeToRadius(0.25f, 4);    // 사각형
            float radius_pentagon = edgeToRadius(0.25f, 5);  // 오각형
        */
    } 

    //1.5 이곳으로 옮김.-->  그림기 함수. 
    inline void drawStarFilled(const MyRGB& color, const float& outRadius, const float& inRadius ){
        
        //꼭짓점 갯수. 
        const int vertex =5; 
        //외곽 꼭짓점 얻어 오기 
        auto outerVertices= makeConvex(outRadius, 90.0f); 
        // 내부 꼭짓점 얻어오기  
        auto innerVertices= makeConvex(inRadius, 90.0f - 360.0f*0.5f/ vertex); 
        
        // MyRGB color(255, 0, 0); //빨강. 

        glColor3fv(color.data); 

        glBegin(GL_TRIANGLES); 
        {
            for(int i =0; i< vertex; ++i){
                glVertex2fv(outerVertices[i].data); 
                glVertex2fv(innerVertices[i].data); 
                glVertex2fv(innerVertices[(i+1)%5].data); 
                
                glVertex2f(0.0f, 0.0f); 
                glVertex2fv(innerVertices[i].data); 
                glVertex2fv(innerVertices[(i+1)%5].data); 

            }
        }
        glEnd(); 
    } 

    //1.7 add 점 그리기 
    inline void drawMyPoint(const MyRGB& color , const MyVector<float>& position, const float& size){
        glColor3fv(color.data); 
        glPointSize(size); 
        glBegin(GL_POINTS); 
        {
            glVertex2fv(position.data); 
        }
        glEnd(); 
    } 

    /*1.5 add 
    1.6 선 일부만 나옴. --> 해결. 
    */
    inline void drawMyLine(const MyRGB& color1, const MyVector<float>& vec1, 
        const MyRGB& color2, const MyVector<float>& vec2  ){
        // std::cout << "in drawMyLine color1 = "<< color1 << " MyVector =" << vec1.x <<","<<vec1.y <<"," << vec1.z <<  std::endl; 
        glBegin(GL_LINES); //오류 수정. 
        {
            glColor3fv(color1.rgb); 
            glVertex2fv(vec1.data); 
            glColor3fv(color2.rgb);
            glVertex2fv(vec2.data);  
        }    
        glEnd(); 
    }

    //1.5. add  color = Colors::gray, dx = 0.5f  
    inline void drawMyGrid(const MyRGB& color, const float& dx){
        
        // if(isKeyPressedAndReleased(GLFW_KEY_G)) {
        //     draw_grid =!draw_grid; 
        //     // std::cout<< "in drawMyGrid isKey  "<< draw_grid << std::endl; 
        // }
        // if(draw_grid){
        // }
        
        glLineWidth(static_cast<float>(0.8f));

        const float max=2.0f; 
        const float dy=dx; 
        //수직선 --세로선. 
        for(float x=0.0f ;  x < max; x +=dx ){ //선이 3개만 그려짐. --> 좌표 오류.. 
            drawMyLine(color, myVec(x, -max, 0.0f), color, myVec(x, +max, 0.0f) ); 
        }
        for(float x= -dx; x > -max; x -= dx){
            drawMyLine(color, myVec(x, -max, 0.0f), color, myVec(x, +max, 0.0f)); 
        }

        //수평선 --가로선
        for(float y=0.0f; y< max; y += dy) {
            drawMyLine(color, myVec(-max, y, 0.0f), color, myVec(max, y, 0.0f)); 
        } 
        for(float y = -dy; y > -max; y -= dy){ 
            drawMyLine(color, myVec(-max, y, 0.0f), color, myVec(max, y, 0.0f) ); 
        }
    } 

    //1.4 추가 
    inline void drawCircleFilled(){
        
    } 

    //1.6 원그리기 할때 좀더 부드러운 원을 그리도록 --> 시각적으로 잘 모르겠음. 
    // 삼각형을 그릴때도 호출함. ?? 
    inline void drawRegularConvexPolygonWired(const MyRGB& color, const float& radius, 
        const float& startTheta, const int& segments){
            const float d_theta = 3.141592f * 2.0f / static_cast<float>(segments); 
            glColor3fv(color.rgb); 
            glBegin(GL_LINE_LOOP); 
            {
                float theta =getMyRadian(startTheta); 
                for(int i=0; i<segments; ++i){
                    glVertex2f(radius * cosf(theta), radius *sinf(theta)); 
                    theta +=d_theta; 
                }
            }
            glEnd(); 
    }

    //1.6 add 원을 그릴때 다각형을 사용해서 여러개의 꼭짓점을 만들어 그린다. 
    inline void drawRegularConvexPolygonFilled(const MyRGB& color, const float& radius, 
        const float& startTheta, const int& numConvex){
        const float d_theta =3.141592f *2.0f/ static_cast<float> (numConvex);    
        // std::cout << "drawRegularConvexPolygonFilled d_theta ="<< d_theta << std::endl;     
        glColor3fv(color.rgb); 
        glBegin(GL_TRIANGLE_FAN); 
        {
            float theta= getMyRadian(startTheta); 
            for(int i =0; i< numConvex; ++i){
                glVertex2f(radius * cosf(theta), radius * sinf(theta)); 
                theta += d_theta; 
            }
        }
        glEnd(); 
    }

    //1.6 add. radius 1.0f
    inline void drawCircleFilled(const MyRGB& color, const float& radius){
        //color, radius - 1e-4f, 0.0f, 30
        // std::cout << "in drawCircleFilled" << std::endl; 
        drawRegularConvexPolygonFilled(color, radius - 1e-4f, 0.0f, 30 ); 
        //좀더 부드럽게 한다나... 
        drawRegularConvexPolygonWired(color, radius, 0.0f, 30); 
    } 

    //1.8 속이빈 원 
    inline void drawCircleWired(const MyRGB& color, const float& radius){
        drawRegularConvexPolygonWired(color, radius, 0.0f, 30); 
    }

    // 1.7 사각형그리기. 
    inline void drawBoxWired(const MyRGB& color, const float& width, const float& height){
        const myVec left_bottom(-0.5f * width, -0.5f* height, 0.0f); //중심 구하기 = 크기의 절반 계산. 
        glColor3fv(color.rgb); 
        glLineWidth(static_cast<float>(1.5)); 

        glBegin(GL_LINE_LOOP); 
        {
            glVertex2f(left_bottom.x, left_bottom.y); 
            glVertex2f(left_bottom.x+width, left_bottom.y); 
            glVertex2f(left_bottom.x+width, left_bottom.y+height); 
            glVertex2f(left_bottom.x, left_bottom.y+height); 
        }
        glEnd(); 
    }

    //1.18 포지션 값을 입력 받아 그곳에 그리는 .. gemini 
    //0.5f곱하는 이유... -> 절반을 구하기 위해서 
    inline void drawBoxWired(const MyRGB& color, const MyVector<float> pos, const float& width, const float& height){
        // const myVec left_bottom(-0.5f * width, -0.5f* height, 0.0f); 

        const float halfW =width * 0.5f; //현재 길이의 절반을 구함. 
        const float halfH =height * 0.5f; //현재 높이의 절반을 구함. 

        glColor3fv(color.rgb); 
        glLineWidth(static_cast<float>(1.5)); 

        glBegin(GL_LINE_LOOP); 
        {
            //중심점에서 각 방향을 절반 만큼만 이동한 좌표를 지정한다. 
            glVertex2f(pos.x - halfW, pos.y - halfH); //좌측 하단. 
            glVertex2f(pos.x +halfW, pos.y -halfH); //우측 하단. 
            glVertex2f(pos.x +halfW, pos.y+halfH); //우측 상단
            glVertex2f(pos.x -halfW, pos.y+halfH); //좌측 상단 
        }
        glEnd(); 
    } 

    //1.19 추가. 벽이 포탄에 맞으면 컬러가 변하는 것으로 구현 
    // const MyVector<float> pos 시작 위치를 표현 
    inline void drawBoxFilled(const MyRGB& color, const MyVector<float> pos, const float& width, const float& height){
        // const myVec left_bottom(-0.5f * width, -0.5f* height, 0.0f); 

        const float halfW =width * 0.5f; //현재 길이의 절반을 구함. 
        const float halfH =height * 0.5f; //현재 높이의 절반을 구함. 

        glColor3fv(color.rgb); 
        glLineWidth(static_cast<float>(1.5)); 

        glBegin(GL_QUADS); 
        {
            //중심점에서 각 방향을 절반 만큼만 이동한 좌표를 지정한다. 
            glVertex2f(pos.x - halfW, pos.y - halfH); //좌측 하단. 
            glVertex2f(pos.x +halfW, pos.y -halfH); //우측 하단. 
            glVertex2f(pos.x +halfW, pos.y+halfH); //우측 상단
            glVertex2f(pos.x -halfW, pos.y+halfH); //좌측 상단 
        }
        glEnd(); 
    } 

    //1.7  꽉찬 사각형. 
    inline void drawBoxFilled(const MyRGB& color, const float& width, const float& height){
        const myVec leftBottom(-0.5f *width, -0.5f*height, 0.0f);
        glColor3fv(color.rgb);  
        glBegin(GL_QUADS); 
        {
            glVertex2f(leftBottom.x, leftBottom.y); //좌측 하단. 
            glVertex2f(leftBottom.x +width, leftBottom.y); //좌측에서 길이만큼 오른쪽으로 
            glVertex2f(leftBottom.x + width, leftBottom.y+height); //오른쪽에서 위쪽으로  
            glVertex2f(leftBottom.x, leftBottom.y+height); 
        }
        glEnd(); 
    } 

    //1 .10 파라미터 2개 짜리 작성. 내부적으로 3개짜리 호출 
    inline void drawBoxFilled(const MyRGB color, const MyVector<float> vec){
        drawBoxFilled(color, vec.x, vec.y); 
    }



    // 1.8 삼각형 그리기 -->내부적으로 
    inline void drawTriangleWired(const MyRGB& color, const float& edgeLength){
        //값이 어떻게 나오는지.? --radius값을 0.176정도의 값으로 계산 됨. --> 수학적으로 잘못된 계산. 
        // edgeLength / root 3
        // drawRegularConvexPolygonWired(color, edgeLength*0.5f*sqrtf(2.0f), 90.0f, 3 ); 
        drawRegularConvexPolygonWired(color, edgeLength/sqrtf(3.0f), 90.0f, 3 ); 
    } 

    //1.8 채워진 삼각형 그리기 
    // - 사각형과 삼각형의 공식이 다르다. 삼각형은 한변의 길이 / 루트 3
    inline void drawTriangleFilled(const MyRGB& color, const float& edgeLength){
        // drawRegularConvexPolygonFilled(color, edgeLength*0.5*sqrtf(2.0f), 90.0f, 3 ); 
        drawRegularConvexPolygonFilled(color, edgeLength/sqrtf(3.0f), 90.0f, 3 ); 
    } 

    inline void drawTriangleWired(const MyRGB& color , const MyVector<float> & v0, const MyVector<float>& v1, const MyVector<float>& v2 ){
        glColor3fv(color.rgb); 
        glBegin(GL_LINE_LOOP); 
        {
            glVertex2fv(v0.data); 
            glVertex2fv(v1.data); 
            glVertex2fv(v2.data); 
        }
        glEnd(); 
    } 

    inline void drawTriangleFilled(const MyRGB& color , const MyVector<float>& v0, const MyVector<float>& v1, const MyVector<float>& v2 ){
        glColor3fv(color.rgb); 
        glBegin(GL_TRIANGLES); 
        {
            glVertex2fv(v0.data); 
            glVertex2fv(v1.data); 
            glVertex2fv(v2.data); 
        }
        glEnd(); 
    } 


    //1.8 add 속빈 사각형  한변의 길이*1/2*루트2 
    inline void drawSquareWired(const MyRGB& color, const float& edgeLength){
        drawRegularConvexPolygonWired(color, edgeLength * 0.5f * sqrtf(2.0f), 45.0f, 4 ); 
    }
    //1.8 add 곽찬 사각형 
    inline void drawSquareFilled(const MyRGB& color, const float& edgeLength){
        drawRegularConvexPolygonFilled(color, edgeLength * 0.5f *sqrtf(2.0f), 45.0f, 4); 

    }
    //1.8 속이 빈 오각형. 
    inline void drawPentagonWired(const MyRGB& color, const float& radius){
        drawRegularConvexPolygonWired(color, radius, 90.0f, 5); 
    }

    //1.8 채워진 오각형. 
    inline void drawPentagonFilled(const MyRGB& color, const float& radius){
        drawRegularConvexPolygonFilled(color, radius, 90.0f, 5); 
        drawRegularConvexPolygonWired(color, radius, 90.0f, 5); //경계 지점을 더 부드럽게 한다고. 
    }

};