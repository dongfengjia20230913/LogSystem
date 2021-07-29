/*
  Author: DongFeng JIA <987293233@qq.com>
  Copyright (c) 2020-2027 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#ifndef _THREAD_H_
#define _THREAD_H_
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace aiheart {
//---------------------------------------------------
class Thread
{
public:
    Thread(std::function<void()>func);
    virtual ~Thread();
    enum State
    {
        Stoped,     ///<停止状态，包括从未启动过和启动后被停止
        Running,    ///<运行状态
        Paused      ///<暂停状态
    };
    State state() const;
    void Start();
    void Stop();
    void pause();
    void resume();
public:
    //检查线程是否退出
    bool check_stop();
    //如果暂停，需要阻塞
    bool wait_pause();
private:
    void run();
    std::function<void()> process;
private:
    std::thread* _thread;
    std::mutex _mutex;
    std::condition_variable _condition;
    volatile std::atomic_bool _pauseFlag;   ///<暂停标识
    volatile std::atomic_bool _stopFlag;   ///<停止标识
    State _state;
};
//---------------------------------------------------
}
#endif /* _THREAD_H_ */
