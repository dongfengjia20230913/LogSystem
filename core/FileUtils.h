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
#ifndef _FILE_UTILS_
#define _FILE_UTILS_
#include <string>
#include <vector>
using namespace std;

namespace aiheart
{
//-------------------------------------
std::vector<std::string> Split(const std::string& s, const char* ch);

std::string GetModleName(const std::string& model_path);

bool IsDir(string &path);

std::string GetLastName(string &path);

void GetDirImages(string& path, vector<string>& images,
                  const char* file_suffix);

bool IsFileExist(string& path);
bool IsFileExist(const char *file_name);

std::string JoinPath(const std::string &path, const std::string &filename);

bool FileExist(const std::string &path);
int CreateDir(const char* path);
int CreateDirs(const char* path);
//-------------------------------------
};
#endif //_ALGO_UTILS_H
