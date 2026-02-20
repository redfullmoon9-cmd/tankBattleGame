#pragma once 

#include <iostream> 
#include <string> 
#include <vector> 
#include <list>
#include <bits/stdc++.h> //forward_list 

/** STL 기본 컨테이너 vector/list 등과  predicate 함수 등 테스트 위해서  */

namespace test{
    void containerTest(){
        std::vector<int> euler {-1, -2, -3, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; 
        
        std::cout<< std::endl; 
        std::cout<< std::endl; 
        
        for(const auto& item: euler){
            std::cout<<item<< ", " ; 
        }
        
        std::cout<< std::endl; 
        
        // auto en =remove_if(euler.begin(), euler.end(),  [](int i) { return i < 0; } ); 
        // euler.erase(en, euler.end());

        //위의 두줄을 한줄로 람다 함수 처럼 사용.  
        euler.erase(remove_if(euler.begin(), euler.end(),  [](int i) { return i < 0; } ), euler.end());
        

        for(const auto& item: euler){
        std::cout <<item<< ", " ; 
        }
    }; 

    void vect1Test(){
        const int NUM =5; 
        std::vector<int> rating(NUM); 
        std::vector<std::string> titles(NUM); 
        std::cout << " y will enter " <<NUM <<  " book title an your rating(1~10)" << std::endl; 
        int i;  
        for(i =0; i < NUM; i++){
            std::cout << "Enter tile " << i+1 << ": "; 
            getline(std::cin, titles[i]); 
            std::cout << " enter your rating (1 ~10) : "; 
            std::cin >> rating[i];  
            std::cin.get(); 
        } 

        for(i=0 ; i <NUM; i++){
            std::cout << " rating :" << rating[i] << "title " << titles[i] << std::endl; 
        }
    }; 

    struct Review{
        std::string title; 
        int rating; 
       
    }; 

    bool FillReview( Review& review){
        std::cout<< "Enter book Title: (quit to quit):"; 
        std::getline(std::cin, review.title); 
        if(review.title == "quit"){
            return false; 
        }

        std::cout << "rating : "; 
        std::cin >> review.rating; 

        if(!std::cin){
            return false; 
        }
        while(std::cin.get() != '\n'){
            continue; 
        }
        return true; 
    }; 

    void ShowReview(const Review& review){
        std::cout <<  review.rating << " ,  " << review.title << std::endl; 
    }; 

    bool operator<(const Review& r1, const Review& r2){
        if(r1.title < r2.title) return true; 
        else if(r1.title == r2.title && r1.rating < r2.rating) return true; 
        else return false; 
    } ; 

    bool worseThan(const Review& r1, const Review& r2){
        if(r1.rating < r2.rating) return true;
        else return false;  
    }; 

    std::vector<Review> books ; 
    std::vector<Review>& FillData(){
        books.push_back(Review{"abook", 10}); 
        books.push_back(Review{"bbook", 4}); 
        books.push_back(Review{"cbook", 12}); 
        books.push_back(Review{"dbook", 5}); 
        books.push_back(Review{"ebook", 6}); 
        books.push_back(Review{"fbook", 0}); 
        books.push_back(Review{"gbook", 0}); 
        return books; 
    }; 

    void vect3Test(){
        // Review temp; 
        /* while(FillReview(temp)){
            books.push_back(temp); 
        } */

       std::vector<Review> books=FillData(); 

        if(books.size() > 0 ){
            std::cout << "book size " << books.size() << std::endl; 
            for_each(books.begin(), books.end(), ShowReview); 

            sort(books.begin(), books.end(), worseThan); 
            std::cout << "sorted by Rating : \n Rating \t book "<< std::endl;  
            for_each(books.begin(), books.end(), ShowReview); 

            //람다 함수는 컨체이너 전체가 아니라 그속에 포함된 각각의 자료를 람다 함수에 전달
            // .. 따라서 내부에 자료형인 구조체 명을 써야. 
            //자료형에 const Review&로 사용해야 불필요한 복사가 방지. 
            //논리적 구성 1. 조건에 맞지 않는 데이터들 앞으로 당기고 그 끝위치의 iterator반환 여기서 en
            //실제 벡터의 삭제는 erase에서 일어남. 

            auto en =remove_if(books.begin(), books.end(), [](const Review& rv ) { return  rv.rating == 0; } ); 
            //실제 삭제는 여기서 일어난다. 
            books.erase(en, books.end());
            std::cout << "afer erase " << std::endl; 
            std::cout << std::endl;  
            for_each(books.begin(), books.end(), ShowReview); 
            
            std::cout << std::endl; 
            std::cout << "after insert" << std::endl; 
            
            books.insert(books.begin(), Review{"eebook", 0}); 
            books.insert(books.end(), Review{"ttest100", 10}); 
            for_each(books.begin(), books.end(), ShowReview); 
            /* random_shuffle(books.begin(), books.end()); 
            std::cout << " After Random shuffle \nRagint \tbook " << std::endl ;
            for_each(books.begin(), books.end(), ShowReview);   */
            
            std::cout << "after insert" << std::endl; 
            books.erase(remove_if(books.begin(), books.end(), [](Review& rv){ return rv.rating < 10; }), books.end()); 
            for(const auto& item: books){
                // ShowReview(item); 
                std::cout << item.rating << " , " << item.title << std::endl; 
            }
       }
    }; 

    template<typename T> 
    class TooBig{
    private:
        T cutOff; 

    public: 
        TooBig(const T& t): cutOff(t) {}; 
        bool operator() (const T& v){ return v > cutOff; }; 
    }; 

    void outint(int i ){ std::cout << i << ", "; }; 
    //funtor 
    bool biggerThan(int i1, int i2) { 
        if(i1 < i2) return true; 
        else return false; 
    }; 
    bool smallerThan(int i1, int i2){
        if(i1 > i2) return true; 
        else return false; 
    }
    //C++ primver plus 1028 
    void functorTest(){
        TooBig f100(100); 
        int vals[10] ={50, 100, 90, 180, 60, 70, 80, 110, 240, 330}; 
        // std::list intList(vals, vals+10); 
        // std::list intList2(vals, vals+10);  
        std::list intList1 ={50, 100, 90, 180, 60, 70, 80, 110, 240, 330}; 
        std::list intList2 ={50, 100, 90, 180, 60, 70, 80, 110, 240, 330}; 

        std::cout << std::endl; 
        std::cout << std::endl; 
        for_each(intList1.begin(), intList1.end(), outint); 
        
        std::cout << std::endl; 
        std::cout<< "after sort" << std::endl; 
        //sort(initList1.begin(), intList1.end(), biggerThan) --> 작동안하는 문제 해결. 원래 안되는 것 임. list와 vector의 차이 
        // intList2.sort([](const int& i1, const int& i2){ return i1 < i2; } ); 
        intList2.sort(biggerThan); //오름차순
        for_each(intList2.begin(), intList2.end(), outint); 
        
        std::cout << std::endl; 
        intList1.sort(smallerThan); //내림차순 
        for_each(intList1.begin(), intList1.end(), outint); 
        std::cout << std::endl; 
        
        std::cout << " after remove f100" << std::endl; 
        intList1.remove_if(f100); 
        for_each(intList1.begin(), intList1.end(), outint); //use a named functor 
        std::cout << std::endl; 
        
        std::cout << " after remove over 200" << std::endl; 
        intList2.remove_if(TooBig<int>(200)); 
        for_each(intList2.begin(), intList2.end(), outint); //use a named functor 
        
    }; 
}