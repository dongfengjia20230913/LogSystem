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
#include <stdio.h>
#include <stdarg.h>
#include <syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <AiLog.h>
#include <vector>
#include "FileUtils.h"
#include<string.h>

namespace aiheart
{
// ----------------------------------------------------------------------
std::shared_ptr<AiLogIo> Log::aiLogIo;

void Log::StartAiLogIo()
{
    Log::aiLogIo = std::make_shared<AiLogIo>();
    Log::aiLogIo->InitLog();
}

void Log::StopAiLogIo()
{
    Log::aiLogIo->StopLog();
}
// ----------------------------------------------------------------------
#ifndef gettid
pid_t gettid()
{
    return syscall(__NR_gettid);
}
#endif

#define AIHEART_COLOR_NONE "\033[0m" //clear all pervision color
#define AIHEART_RED "\033[1;31;40m" //40 black color, 1:height
#define AIHEART_BLUE "\033[1;34;40m"
#define AIHEART_GREEN "\033[1;32;40m"
#define AIHEART_YELLOW "\033[1;33;40m"

// ----------------------------------------------------------------------

const int Log::kVerbose = 5;

const int Log::kDebug = 4;

const int Log::kInfo = 3;

const int Log::kWarning = 2;

const int Log::kError = 1;

char log_buffer[1024*13] = {0};

int Log::level_ = Log::kVerbose;

void Log::Print(int prio,
                const char *level_desc,
                const char *tag,
                const char *file,
                const int line,
                const char *fmt,
                ...)
{
    if (prio <= level_)
    {
        char time_desc[64];
        char buf[256];
        va_list ap;

        GetTimeFormat(time_desc);
        va_start(ap, fmt);
        vsnprintf(buf, 256, fmt, ap);
        va_end(ap);

        std::string font_color = std::string(AIHEART_COLOR_NONE);
        if(prio == kError)
        {
            font_color = std::string(AIHEART_RED);
        }
        else if(prio == kWarning)
        {
            font_color = std::string(AIHEART_YELLOW);
        }

        sprintf(log_buffer,"%s %d %d %s %s_%d %s %s \n",
               time_desc,
               getpid(),
               gettid(),
               level_desc,
               GetFileBase(file).c_str(),
               line,
               tag,
               buf);

        if(Log::aiLogIo != nullptr)
        {
            Log::aiLogIo->QueueLog(log_buffer);
        }
        printf("%s %s",font_color.c_str(), log_buffer);
    }
}

void Log::GetTimeFormat(char *buffer)
{
    time_t t;
    struct tm *p;
    struct timeval tv;
    int len;
    int millsec;

    t = time(NULL);
    p = localtime(&t);

    gettimeofday(&tv, NULL);
    millsec = (int)(tv.tv_usec / 1000);

    len = snprintf(buffer,
                   32,
                   "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ",
                   p->tm_year + 1900,
                   p->tm_mon + 1,
                   p->tm_mday,
                   p->tm_hour,
                   p->tm_min,
                   p->tm_sec,
                   millsec);

    buffer[len] = '\0';
}

std::string Log::GetFileBase(const char *filename)
{
    std::vector<std::string> vec_str = Split(filename, "/");
    std::string file_base = vec_str[vec_str.size() - 1];
    return &file_base[0];
}

// ----------------------------------------------------------------------

}; // namespace aicore
