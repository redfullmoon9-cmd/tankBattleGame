#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

namespace tp{
    template<typename T>
    class Vector3{
        private:

        public:

        union{
            struct {T x, y, z; }; 
            struct {T r, g, b; }; 
            T data[3]; 
            T rgb[3]; 
        }; 
        Vector3():x(T()), y(T()), z(T()) {}; 
        Vector3(const T& x, const T& y, const T& z):x(x), y(y), z(z){  }
        ~Vector3()=default; 

        //복사 생성자. 
        Vector3(const Vector3& other){
            x=other.x; 
            y=other.y; 
            z=other.z; 
        }
        //대입 생성자. 
        Vector3& operator= (const Vector3& other){
            x=other.x; 
            y=other.y; 
            z=other.z; 
            return *this; 
        } 
        //[] 연산자 오버로딩. 
        T& operator[] (const int& idx){
            return data[idx]; 
        } 

        void operator += (const Vector3<T> vec){
            x +=vec.x; 
            y +=vec.y; 
            z +=vec.z; 
        }




    };
   
} 

#endif 