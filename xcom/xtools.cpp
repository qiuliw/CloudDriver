#include "xcom.h"
#include "xtools.h"

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace std;

std::string GetDirData(std::string path){
    string data = "";
#ifdef _WIN32
    // Windows 下获取目录文件信息
    _finddata_t file;
    string dirpath = path + "/*.*";
    // 打开目录
    intptr_t dir = _findfirst(dirpath.c_str(), &file);
    if (dir < 0) {
        return data;
    }
    do{
        // 跳过子目录
        if (file.attrib & _A_SUBDIR){
            continue;
        }
        char buf[1024] = {0};
        // 拼接文件名和文件大小
        sprintf(buf,"%s,%u;",file.name,file.size);
        data += buf;
    }while (_findnext(dir, &file) == 0);
#else // linux
    // Linux 下获取目录文件信息
    const char *dir = path.c_str();
    DIR *dp = 0;
    struct dirent *entry = 0;
    struct stat statbuf;
    // 打开目录
    dp = opendir(dir);
    if(dp == NULL) {
        perror("opendir");
        return data;
    }
    // 切换到目标目录
    chdir(dir);
    char buf[1024] = {0};
    // 遍历目录项
    while ((entry = readdir(dp)) != nullptr) {
        // 跳过子目录
        if (entry->d_type == DT_DIR) {
            continue;
        }
        // 拼接文件名和目录项长度
        sprintf(buf,"%s,%u;",entry->d_name,entry->d_reclen);
        data += buf;
    }
    // 关闭目录
    closedir(dp);

#endif
    // 去掉结尾分号
    if (!data.empty()){
        data = data.substr(0, data.size() - 1);
    }
    return data;
}