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
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "AiLogIo.h"
#include "FileUtils.h"
#include "AiLog.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
namespace aiheart {
//---------------------------------------------------

AiLogIo::AiLogIo()
{

}

AiLogIo::~AiLogIo()
{

}

int AiLogIo::InitLog()
{
    fprintf(stdout, "%s\n", __func__);

    CheckAndRenameLogFile();

    //in debug mode，while write every line to log file
    is_debug = IsFileExist(FILE_FOR_DEBUG);
    if(is_debug)
    {
        Log::level_ = Log::kVerbose;
    }

    //check and create log file dir
    int ret = 0;//sucess is 0, fail is -1
    ret |= CreateDirs(LOGFILE_PATH);
    if(ret != 0)
    {
        fprintf(stderr, "couldn't create log dir, errno %d\n", ret);
        return -1;
    }

    //start log thread
    log_thread_ = std::make_shared<Thread>(std::bind(&AiLogIo::UpdateLogFile, this));
    log_thread_->Start();
    return 0;
}

void AiLogIo::UpdateLogFile()
{
    while(!log_thread_->check_stop())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        WriteLog();
    }

}

int AiLogIo::WriteLog()
{
    int log_lines = msg_queue.size();
    // printf("log_lines[%d] [%d]\n",log_lines, is_debug);
    if(log_lines > g_cache_max_size || is_debug)
    {

        // fprintf(stdout, "log_cache size: %d\n", log_cache.size());
        std::string log_cache = GetLogBuffer(log_lines);
        int log_len = log_cache.size();
        // fprintf(stderr," \n log_cache len:%d\n",log_len);
        if(log_len)
        {
            int fd = open(log_base_name, O_CREAT|O_APPEND |O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
            if(fd>=0)
            {
                write(fd, log_cache.c_str(), log_len); //deal with EINTR later.
                close(fd);
            }else
            {
                fprintf(stderr," \n open logfile error:%s\n",log_base_name);
            }

            long file_size = GetFileSize(log_base_name);
            if(file_size > MAX_LOG_FILE_SIZE)
            {
                remove(log_base_name);
            }
            // fprintf(stdout, "log_cache: %s, file_size: %d\n", log_cache.c_str(), file_size);
        }
    }
    // fprintf(stdout, "log_cache size %d line:%d\n", log_cache.size(), log_lines);
    return 1;

}

std::string AiLogIo::GetLogBuffer(int log_lines)
{
    const std::lock_guard<std::mutex> _lock(msg_queue_mutex_);
    std::string log_cache;
    log_cache.reserve(log_lines*100);
    while(log_lines > 0)
    {
        std::string msginfo = msg_queue.front();
        msg_queue.pop_front();
        log_lines--;
        if(msginfo.length() > 0)
        {
            log_cache.append(msginfo);
        }
    }
    return log_cache;

}

int AiLogIo::QueueLog(char *buffer)
{
    if(buffer != NULL)
    {
        const std::lock_guard<std::mutex> _lock(msg_queue_mutex_);
        msg_queue.push_back(buffer);
    }
    return 0;
}

bool AiLogIo::GetLastLogFile(const char *basePath)
{
    #define PATH_LEN 1000
    DIR *dir;
    struct dirent *ptr;

    if((dir = opendir(basePath)) == NULL)
    {
        return false;
    }

    int tmp_prefix_len = strlen(AI_PREFIX);
    int log_ext_len = strlen(LOG_EXT);
    while((ptr=readdir(dir)) != NULL)
    {
        char * pn = ptr->d_name;
        if(strcmp(pn,".") == 0 || strcmp(pn,"..") == 0)
            continue;
        else if(ptr->d_type == DT_REG)    ///file
        {
            int filelen = strlen(pn);

            if(filelen < PATH_LEN)
            {
                if(!strncmp(pn, AI_PREFIX, tmp_prefix_len)
                    && !strncmp(pn + filelen - log_ext_len, LOG_EXT, log_ext_len))
                {
                    sprintf(old_log_file,"%s/%s",basePath, pn);
                    printf("%s\n", old_log_file);
                }
            }
        }
    }
    closedir(dir);
    return strlen(old_log_file) != 0;
    // return oldname;
}

void AiLogIo::CheckAndRenameLogFile()
{
    char currtime[30] = {0};
    GetCurtime(currtime);
    sprintf(log_base_name,"%s/%s_%s%s", LOGFILE_PATH, "aiheart_", currtime, LOG_EXT);

    if(GetLastLogFile(LOGFILE_PATH))
    {
        rename(old_log_file, log_base_name);
        printf("rename old[%s] to newfile[%s]\n", old_log_file, log_base_name);
    }
}

long AiLogIo::GetFileSize(const char *path)
{
    unsigned long file_size = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0)
    {
        return file_size;
    }else{  
        file_size = statbuff.st_size;
    }  
    return file_size;
}

time_t AiLogIo::GetCurtime(char* outTimeStr)
{
    time_t tTime = 0;
    struct tm *tmTime = NULL;

    tTime = time(NULL);
    if(outTimeStr != NULL)
    {
        tmTime = localtime(&tTime);
        sprintf(outTimeStr, "%04d%02d%02d%02d%02d%02d",
            tmTime->tm_year + 1900, tmTime->tm_mon + 1,
            tmTime->tm_mday, tmTime->tm_hour, tmTime->tm_min, tmTime->tm_sec);
    }

    return tTime;
}

void AiLogIo::StopLog()
{
    fprintf(stdout, "%s\n", __func__);
    if(log_thread_ != nullptr)
    {
        log_thread_->Stop();
    }
}


//---------------------------------------------------
}

