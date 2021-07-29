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
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include "FileUtils.h"
#include "AiLog.h"

namespace aiheart
{
//-------------------------------------

std::vector<std::string> Split(const std::string& s, const char* ch)
{
    uint32_t start = 0;
    uint32_t len = 0;
    std::vector<std::string> ret;
    for (uint32_t i = 0; i < s.length(); i++) {
        if (s[i] == *ch) {
            ret.push_back(s.substr(start, len));
            start = i + 1;
            len = 0;
        } else {
            len++;
        }
    }
    if (start < s.length())
        ret.push_back(s.substr(start, len));
    return ret;
}

//../aaa.model ---> AAA.MODEL
std::string GetModleName(const std::string& model_path)
{
    std::vector<std::string> vec_str = Split(model_path, "/");
    std::string model_name = vec_str[vec_str.size() - 1];

    std::transform(model_name.begin(), model_name.end(), model_name.begin(), ::toupper);

    return model_name;
}

bool IsDir(std::string &path)
{
    struct stat info;
    int r=stat(&path[0], &info);
    return S_ISDIR(info.st_mode);
}

std::string GetLastName(std::string &path)
{
    int dir_size = path.size();
    if(IsDir(path))
    {
        int last_separation_index = path.find_last_of('/');
        int last_is_separation = (last_separation_index == dir_size-1);
        if(last_is_separation)
        {
             std::string temp = path.substr(0, last_separation_index);
             return temp.substr(temp.find_last_of('/')+1, temp.size());
        }
        else
        {
            return path.substr(path.find_last_of('/')+1, path.size());
        }
    }
    else
    {
        std::string temp =  path.substr(path.find_last_of('/')+1, path.size());
        return temp.substr(0, temp.find_last_of('.'));
    }

}

void GetDirFiles(string& path, vector<string>& files_ret, const char* file_suffix)
{
    if(!IsDir(path))
    {
        fprintf(stderr, "path[%s] is not dir!", path.c_str());
        return;
    }

    struct dirent * filendirent;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir(&path[0]);
    if( NULL == dir )
    {
        fprintf(stderr, "Can not open path[%s]!", path.c_str());
        return;
    }

    size_t path_size = path.size();
    size_t last_separation_index = path.find_last_of('/');
    int last_is_separation = (last_separation_index == path_size-1);

    while((filendirent = readdir(dir)) != NULL )
    {
        // get rid of "." and ".."
         char* filename = filendirent->d_name;
        if(strcmp(filename, "." ) == 0 ||  strcmp(filename, "..") == 0)
        {
            continue;
        }

        if(0 == strcmp(strchr(filename, '.'), &file_suffix[0]))
        {
            string image = filename;
            if(last_is_separation)
            {
                files_ret.push_back(path + image);
            }
            else
            {
                files_ret.push_back(path +'/'+ image);
            }
        }
    }
    closedir(dir);
};

bool IsFileExist(string& path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool IsFileExist(const char *file_name)
{
    struct stat buffer;
    return (stat(file_name, &buffer) == 0);
}

std::string JoinPath(const std::string &path, const std::string &filename)
{
    int dir_size = path.size();

    int last_sprt_index = path.find_last_of('/');
    int last_is_sprt = (last_sprt_index == dir_size-1);

    if(last_is_sprt)
    {
         return path + filename;
    }
    else
    {
        return path + "/" + filename;
    }
}

bool FileExist(const std::string &path)
{
    struct stat statbuf;
    int ret = lstat(path.c_str(), &statbuf);

    return ret==0;
}

int CreateDir(const char* path)
{
    if (path == NULL)
    {
        return -1;
    }

    DIR* dir = NULL;
    if ((dir = opendir(path)) == NULL)
    {
        if (mkdir(path, 0777) == -1)
        {
            //perror("mkdir failed");
            return -1;
       }
    }
    closedir(dir);
    return 0;
}

int CreateDirs(const char* path) {
    if (path == NULL)
    {
        return -1;
    }

    // First, try to use the full path.
    if (CreateDir(path) == 0)
    {
        printf("create path %s success\n", path);
        return 0;
    }

    if (errno != ENOENT)
    {
        fprintf(stderr, "Could not create path %s\n", path);
        return -1;
    }

    char* src = (char*) path;
    char* start;
    do
    {
        start = strchr(src, '/');

        //最后一次循环，创建最后一层目录
        if (start == NULL)
        {
            return CreateDir(path);
        }

        //路径的第一个字符是/，所以跳过
        if (start == path)
        {
          src = start + 1;
            continue;
        }

        //遍历创建中间层级目录
        char sub_path[PATH_MAX];
        int len = start - path;
        strncpy(sub_path, path, len);
        if (CreateDir(sub_path) != 0)
        {
            printf("Could not create sub path %s\n", sub_path);
            return -1;
        }
        src = start + 1;
    } while(start);

    printf("create path %s success\n", path);
    return 0;
}

//-------------------------------------
}

