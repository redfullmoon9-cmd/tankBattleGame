#pragma once 
#include <iostream>
/* 12.30 vector3와 RGB클래스 분석을 위한 테스트 클래스 
향후 복사 연산자와 대입 연산자 보충. 
*/

namespace ref{
    
    template<typename T> 
    class MyVector{
    private: 
        int capacity; 
        int length; 
    
    public: 
    union {
            struct {T x, y, z;   }; 
            struct {T v0, v1, v2;  }; 
            struct {T r, g, b; } ; 
            T data[3]; 
            T rgb[3]; 
        }; 
    public:     
        MyVector() : capacity(1), length(0) {} 
        MyVector(int n) : capacity(n), length(0), x(0), y(0), z(0){ }; 
        MyVector(const T& x, const T& y, const T& z): capacity(3), length(3), x(x), y(y), z(z){ }
    
        //복사 생성자 추가 
        MyVector(const MyVector& other){
            x= other.x; 
            y= other.y; 
            z= other.z; 
        }
        //1.4 대입 생성자 수정 const로 
        MyVector& operator=(const MyVector& other){
            x=other.x; 
            y=other.y; 
            z=other.z; 
            return *this; 
        }
    
        ~MyVector(){}
    
        T& operator[] (const int& idx ){
            return data[idx]; 
        }
    
        const T& operator[] (const int& idx) const{
            return data[idx]; 
        }
        //friend키워드가 있을때 만 작동 아니면 전역으로 
        friend std::ostream& operator<<(std::ostream& os, const MyVector<T>& vec){
            os<< vec.x << " " << vec.y << " " << vec.z ; 
            return os; 
        }
        //1.11 
        MyVector<T> operator*(const T& value){
            return MyVector<T>(x*value, y*value, z*value); 
        }
    
        MyVector<T> operator+(const T& value){
            return MyVector<T> ( x+value, y+value , z+value); 
        }
        
        //1.18 add 
        MyVector<T> operator+(const MyVector<T>& v){
            return MyVector<T> ( x+v.x, y+v.y , z+v.z); 
        }
        MyVector<T> operator-(const MyVector<T>& v){
            return MyVector<T> (x - v.x, y -v.y, z -v.z); 
        }
        
        //1.22 
        MyVector<T> operator/(const MyVector<T>& v){
            return MyVector<T> (x/v.x , y / v.y, z / v.z ); 
        }
    
    
        //1. 11 
        void operator += (const MyVector<T> vec){
            x +=vec.x; 
            y +=vec.y; 
            z +=vec.z; 
        }
    };
    
    class MyRGB : public MyVector<float>{
        public: 
        MyRGB(){} 
        MyRGB(const float r, const float g, const float b) :MyVector(r, g, b){} 
        MyRGB(const int ir, const int ig, const int ib): MyVector(
                static_cast<float>(ir)/ 255.0f, static_cast<float> (ig)/255.0f, static_cast<float> (ib) /255.0f ) {}
        ~MyRGB(){} 
    
    
    }; 

}