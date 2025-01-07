#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<deque>
#include<string>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

// C++98: 使用面向对象的做法，每一个job都是Task继承类的对象
// pthreads 每个线程不断执行循环，从task_queue获得task，并执行task。

namespace AIL{
  class Task
  {
  protected:
    void* args;
    std::string task_name;
  public:
    Task(void* args_=NULL,const std::string& task_name_="")
    :args(args_),task_name(task_name_)
    {}
    //确保在通过基类指针或引用删除派生类对象时，能够正确调用派生类的析构函数.
    virtual ~Task()
    {}
    void setArg(void* args_){
      args=args_;
    }
    // 纯虚函数
    virtual int run()=0;
  };

  class ThreadPool{
    
  private:
    bool is_running;
    int thread_num;
    pthread_t* thread_array;

    std::deque<Task*> task_queue;
    pthread_mutex_t mut;
    pthread_cond_t cond;
  public:
    ThreadPool(int thread_num_=10);
    ~ThreadPool();

    size_t addTask(Task* task);
    void stop();
    // taskqueue size
    int size();
    Task* take();
  private:
    int initThreads();
    static void* threadFunc(void* args);
    ThreadPool(const ThreadPool&)=delete;
    ThreadPool& operator=(const ThreadPool&)=delete;

  };

};


#endif
