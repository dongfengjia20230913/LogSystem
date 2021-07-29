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
#ifndef _AILOG_IO_H_
#define _AILOG_IO_H_
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <Thread.h>

namespace aiheart {
//---------------------------------------------------
enum AiLogIoStatus
{
    START,
    STOP
};

// ----------------------------------------------------------------------
#ifndef  LOGFILE_PATH
    #define LOGFILE_PATH         "./data/log"
    #define AI_PREFIX           "aiheart"
    #define LOG_EXT             ".log"
#endif

#ifndef  FILE_FOR_DEBUG
    #define    FILE_FOR_DEBUG     LOGFILE_PATH"/.debug"
#endif

#ifndef  MAX_LOG_LEN
    #define MAX_LOG_LEN (1024 * 13)
#endif

#ifndef  MAX_LOG_FILE_SIZE
    #define MAX_LOG_FILE_SIZE (1024 *1024*5)//limit log file max 10M
#endif
// ----------------------------------------------------------------------

class AiLogIo
{
public:
    AiLogIo();

    virtual ~AiLogIo();

    //init log, rename last logfile, create log thread
    int InitLog();

    //thread call function. write current "msg_queue" content to log file ,and clear "msg_queue"
    void UpdateLogFile();

    //stop log system
    void StopLog();

    //queue log content to "msg_queue"
    int QueueLog(char *buffer);

    //write log content to file , be called by log thread
    int WriteLog();

    //get log file size
    long GetFileSize(const char *path);

    //get last log file that be save by log system start last
    bool GetLastLogFile(const char *basePath);

    //get last log file and rename to current time log file
    void CheckAndRenameLogFile();

    std::string GetLogBuffer(int log_lines);

private:
    time_t GetCurtime(char* outTimeStr);

    std::shared_ptr<Thread> log_thread_;

    AiLogIoStatus log_thread_stop;

    int line_count = 0;

    // char sbuf[MAX_LOG_LEN] = {0};

    std::deque<std::string> msg_queue;

    std::mutex msg_queue_mutex_;

    //only cacle log line > g_cache_max_size, will write log to file
    //debug mode will write log to file every line 
    bool is_debug = false;

    int g_cache_max_size = 512;

    char log_base_name[1024] = {0};

    char old_log_file[1024]={0};

};
//---------------------------------------------------
}
#endif /* _THREAD_H_ */
