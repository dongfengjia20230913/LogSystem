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
#ifndef __AILOG_H__
#define __AILOG_H__

#include <string>
#include "AiLogIo.h"

#ifndef LOG_TAG
#define LOG_TAG "AI"
#endif

namespace aiheart
{

// ----------------------------------------------------------------------

class Log
{
public:
    static const int kVerbose;

    static const int kDebug;

    static const int kInfo;

    static const int kWarning;

    static const int kError;

    static int level_;

    static void Print(int prio,
                      const char *level_desc,
                      const char *tag,
                      const char *file,
                      const int line,
                      const char *fmt, ...);

    static std::shared_ptr<AiLogIo> aiLogIo;

    static void StartAiLogIo();
    static void StopAiLogIo();

private:
    static void GetTimeFormat(char *buffer);
    static std::string GetFileBase(const char *filename);
};

// ----------------------------------------------------------------------

#define LOGV(...) Log::Print(aiheart::Log::kVerbose, "V", LOG_TAG, __FILE__, __LINE__, __VA_ARGS__)

#define LOGD(...) Log::Print(aiheart::Log::kDebug, "D", LOG_TAG, __FILE__, __LINE__, __VA_ARGS__)

#define LOGI(...) Log::Print(aiheart::Log::kInfo, "I", LOG_TAG, __FILE__, __LINE__, __VA_ARGS__)

#define LOGW(...) Log::Print(aiheart::Log::kWarning, "W", LOG_TAG, __FILE__, __LINE__, __VA_ARGS__)

#define LOGE(...) Log::Print(aiheart::Log::kError, "E", LOG_TAG, __FILE__, __LINE__, __VA_ARGS__)

#define LOG(...)  printf(__VA_ARGS__)


// ----------------------------------------------------------------------

}; // namespace aicore
#endif //__AICORE_LOG_H__
