#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include"threadpool.h"


class MyTask: public AIL::Task
{
public:
  MyTask()
  {}
  int run(){
    printf("thread[%lu]:%s\n",pthread_self(),(char*)this->args);
    sleep(1);
    return 0;
  }
};

int main()
{
  char szTemp[]="hello world";

  MyTask taskObj;
  taskObj.setArg((void*)szTemp);

  AIL::ThreadPool pool(10);
  for(int i=0;i<20;++i){
    pool.addTask(&taskObj);
  }  

  while(1){
    printf("there are still %d tasks need to process\n",pool.size());
    if(pool.size()==0){
      pool.stop();
      printf("Now i will exit from main\n");
      break;
    }
    sleep(2);
  }
}
