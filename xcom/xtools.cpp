#include "xcom.h"
#include "xtools.h"

#ifdef _WIN32
#include <io.h>
#endif

using namespace std;

std::string GetDirData(std::string path){
    string data = "";
#ifdef _WIN32
    // 存储文件信息
    _finddata_t file;
    string dirpath = path + "/*.*";
    // 目录上下文
    intptr_t dir = _findfirst(dirpath.c_str(), &file);
    if (dir < 0) {
        return data;
    }
    do{
        // 子目录
        if (file.attrib & _A_SUBDIR){
            continue;
        }
        char buf[1024] = {0};
        sprintf(buf,"%s,%u;",file.name,file.size);
        data += buf;
    }while (_findnext(dir, &file) == 0);
#else

#endif
    // 去掉结尾分号
    if (!data.empty()){
        data = data.substr(0, data.size() - 1);
    }
    return data;
}