#include<iostream>
#include<unistd.h>

#include"ThreadPool.h"



class MyTask
{
public:
  MyTask(){}

  int run(int i,const char* p){
    printf("thread[%llu]:(%d,%s)\n",pthread_self(),i,(char*)p);
    sleep(1);
    return 0;
  }
};

int main()
{
  AIL::ThreadPool pool(10);
  MyTask taskObj[20];
  for(size_t i=0;i<20;++i){
    pool.addTask(std::bind(&MyTask::run,&taskObj[i],i,"helloword"));
  }

  while(1){
    printf("there are still %d tasks need to process\n",pool.size());
    if(pool.size()==0){
      pool.stop();
      printf("Now i will exit from main\b");
      break;
    }
    sleep(2);
  }
  return 0;
}
