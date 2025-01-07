#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include<future>
#include<vector>
#include<queue>
#include<functional>

/* 最新做法，**完全采用C++11**技术，比如std::thread, mutex, condition_variable, atomic组件，
还有lambda技巧，packaged_task， future等等；
*/
namespace AIL{

  class ThreadsGuard{
  private:
    std::vector<std::thread>& threads;
  public:
    ThreadsGuard(std::vector<std::thread>& threads_)
    :threads(threads_)
    {}
    ~ThreadsGuard(){
      for(size_t i=0;i!=threads.size();++i){
        if(threads[i].joinable())
          threads[i].join();
      }
    }
    ThreadsGuard(ThreadsGuard&& ) = delete;
    ThreadsGuard& operator = (ThreadsGuard&& ) = delete;
    ThreadsGuard(const ThreadsGuard&) = delete;
    ThreadsGuard& operator = (const ThreadsGuard&) = delete;
  };

  class ThreadPool
  {
  public:
    typedef std::function<void()> task_type;
  private:
    std::atomic<bool> is_stopped;
    std::mutex mut;
    std::condition_variable cond;
    std::queue<task_type> task_queue;
    std::vector<std::thread> thread_array;
    ThreadsGuard guards;
  public:
    explicit ThreadPool(int n=0);
    ~ThreadPool()
    {
      stop();
      cond.notify_all();
    }
    void stop()
    {
      is_stopped.store(true,std::memory_order_release);
    }

    template<class Function, class... Args>
    std::future<typename std::result_of<Function(Args...)>::type>
      add(Function&&,Args&&...);
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator = (ThreadPool&&) = delete;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator = (const ThreadPool&) = delete;    
  };

  inline ThreadPool::ThreadPool(int n)
  :is_stopped(false),guards(thread_array)
  {
    int nthreads=n;
    if(nthreads<=0){
      nthreads=std::thread::hardware_concurrency();
      nthreads=(nthreads==0?2:nthreads);
    }

    for(int i=0;i<nthreads;++i){
      thread_array.push_back(std::thread([this]{
        while(!is_stopped.load(std::memory_order_acquire)){
          task_type task;
          {
            std::unique_lock<std::mutex> lk(this->mut);
            this->cond.wait(lk,[this]{
              return is_stopped.load(std::memory_order_acquire)||
                !this->task_queue.empty();
            });
            if(is_stopped.load(std::memory_order_acquire))
              return;
            task=std::move(this->task_queue.front());
            this->task_queue.pop();
          }
          task();
        }
      }));
    }
  }

  template<class Function,class... Args>
  std::future<typename std::result_of<Function(Args...)>::type>
    ThreadPool::add(Function&& fcn,Args&&... args)
  {
    typedef typename std::result_of<Function(Args...)>::type return_type;
    typedef std::packaged_task<return_type()> task;

    auto t=std::make_shared<task>(std::bind(std::forward<Function>(fcn),std::forward<Args>(args)...));
    auto ret=t->get_future();
    {
      std::lock_guard<std::mutex> lk(mut);
      if(is_stopped.load(std::memory_order_acquire))
        throw std::runtime_error("thread pool has stopped");
      task_queue.emplace([t]{(*t)();});
    }
    cond.notify_one();
    return ret;
  }

};




#endif
