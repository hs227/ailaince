#include<iostream>
#include<string>
#include"threadpool.h"


int main()
{
  std::mutex output_mut;
  try{
    AIL::ThreadPool pool;
    std::vector<std::future<int>> v;
    std::vector<std::future<void>> v1;
    for(int i=0;i<=10;++i){
      std::future<int> ans=pool.add(
        [](int answer){
          return answer;
        },i);
      v.push_back(std::move(ans));
    }
    for(int i=0;i<=5;++i){
      std::future<void> ans=pool.add([&output_mut](
        const std::string& s1,const std::string s2){
        std::lock_guard<std::mutex> lk(output_mut);
        std::cout<<(s1+s2)<<std::endl;
      },"hello","world");
      v1.push_back(std::move(ans));
    }
    for(size_t i=0;i<v.size();++i){
      std::lock_guard<std::mutex> lg(output_mut);
      std::cout<<v[i].get()<<std::endl;
    }
    for(size_t i=0;i<v1.size();++i){
      v1[i].get();
    }
  }catch(std::exception& e){
    std::cout<<e.what()<<std::endl; 
  }
}

