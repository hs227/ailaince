#include"threadpool.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

namespace AIL{
    ThreadPool::ThreadPool(int thread_num_)
    :is_running(true),thread_num(thread_num_),
    thread_array(NULL)
    {
      pthread_mutex_init(&mut,NULL);
      pthread_cond_init(&cond,NULL);
      initThreads();
    }
    ThreadPool::~ThreadPool(){
      stop();
      task_queue.clear();
    }

    size_t ThreadPool::addTask(const Task& task)
    {
      pthread_mutex_lock(&mut);
      task_queue.push_back(task);
      int size=task_queue.size();
      pthread_mutex_unlock(&mut);
      pthread_cond_signal(&cond);
      return size;
    }
    void ThreadPool::stop()
    {
      if(is_running==false)
        return;

      is_running=false;
      pthread_cond_broadcast(&cond);

      for(int i=0;i<thread_num;++i){
        pthread_join(thread_array[i],NULL);
      }

      free(thread_array);
      thread_array=NULL;

      pthread_mutex_destroy(&mut);
      pthread_cond_destroy(&cond);
    }
    int ThreadPool::size()
    {
      pthread_mutex_lock(&mut);
      int size=task_queue.size();
      pthread_mutex_unlock(&mut);
      return size;
    }
    ThreadPool::Task ThreadPool::take()
    {
      Task task;

      pthread_mutex_lock(&mut);
      while(task_queue.empty()&&is_running){
        pthread_cond_wait(&cond,&mut);
      }
      if(is_running==false){
        pthread_mutex_unlock(&mut);
        return task;
      }

      assert(!task_queue.empty());
      task=task_queue.front();
      task_queue.pop_front();
      pthread_mutex_unlock(&mut);
      return task;
    }
 
    int ThreadPool::initThreads()
    {
      thread_array=(pthread_t*)malloc(sizeof(pthread_t)*thread_num);
      for(int i=0;i<thread_num;++i){
        pthread_create(&thread_array[i],NULL,threadFunc,this);
      }
      return 0;
    }

    void* ThreadPool::threadFunc(void* args)
    {
      pthread_t tid=pthread_self();
      ThreadPool* pool=static_cast<ThreadPool*>(args);// this
      Task task;
      while(pool->is_running){
        Task task=pool->take();
        if(!task){
          printf("thread %llu will exit\n",tid);
          break;
        }
        task();
      }
      return NULL;
    }
    

};

