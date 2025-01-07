#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<deque>
#include<pthread.h>
#include<functional> // for std::function,std::bind

// 使用C++03/C++0x 语言规范实现的线程池：基于对象做法，每个job都是一个fuction对象
// 使用C++03语言规范，还有C++0x（特指std::function + std::bind）

namespace AIL{

  class ThreadPool
  {
  public:
    typedef std::function<void()> Task;
  private:
    bool is_running;
    int thread_num;
    pthread_t* thread_array;
    std::deque<Task> task_queue;
    pthread_mutex_t mut;
    pthread_cond_t cond;
  public:
    ThreadPool(int thread_num=10);
    ~ThreadPool();
    size_t addTask(const Task& task);
    void stop();
    int size();// task_queue size
    Task take();
  private:
    ThreadPool(const ThreadPool&)=delete;
    ThreadPool& operator=(const ThreadPool&)=delete;
    int initThreads();
    static void* threadFunc(void* thread_data);
  };
}











#endif
