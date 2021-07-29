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
#include "Thread.h"
namespace aiheart {
//---------------------------------------------------
Thread::Thread(std::function<void()> func)
    : _thread(nullptr),
    _pauseFlag(false),
    _stopFlag(false),
    _state(Stoped)
{
    process = func;
}

Thread::~Thread()
{
    Stop();
}

Thread::State Thread::state() const
{
    return _state;
}

void Thread::Start()
{
    if (_thread == nullptr)
    {
        _thread = new std::thread(&Thread::run, this);
        _pauseFlag = false;
        _stopFlag = false;
        _state = Running;
    }
}

void Thread::Stop()
{
    if (_thread != nullptr)
    {
        _pauseFlag = false;
        _stopFlag = true;
        _condition.notify_all();  // Notify one waiting thread, if there is one.
        if (_thread->joinable())
            _thread->join(); // wait for thread finished
        delete _thread;
        _thread = nullptr;
        _state = Stoped;
    }
}

void Thread::pause()
{
    if (_thread != nullptr)
    {
        _pauseFlag = true;
        _state = Paused;
    }
}

void Thread::resume()
{
    if (_thread != nullptr)
    {
        _pauseFlag = false;
        _condition.notify_all();
        _state = Running;
    }
}

void Thread::run()
{
    process();

    _pauseFlag = false;
    _stopFlag = false;
}

//检查线程是否退出
bool Thread::check_stop()
{
    return _stopFlag;
}
//如果暂停，需要阻塞
bool Thread::wait_pause()
{
    if (_pauseFlag)
    {
        std::unique_lock<std::mutex> locker(_mutex);
        while (_pauseFlag)
        {
            _condition.wait(locker); // Unlock _mutex and wait to be notified
        }
        locker.unlock();
    }

    return !_stopFlag;
}
//---------------------------------------------------
}
