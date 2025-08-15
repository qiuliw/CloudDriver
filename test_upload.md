# 文件上传功能测试指南

## 修复的问题

1. **内存管理问题**：修复了 `WriteStr` 和相关函数中的内存泄漏问题
2. **日志增强**：添加了详细的日志信息来帮助诊断问题
3. **错误处理**：改进了错误处理和日志记录

## 测试步骤

### 1. 编译项目
```bash
# 在项目根目录下
mkdir build
cd build
cmake ..
make
```

### 2. 启动服务器
```bash
# 在bin目录下运行服务器
./xdisk_server
```

### 3. 启动客户端
```bash
# 在bin目录下运行客户端
./xdisk_gui
```

### 4. 测试文件上传
1. 在客户端界面中设置服务器IP和端口
2. 点击"刷新"按钮获取目录列表
3. 点击"上传"按钮选择文件
4. 观察控制台输出的日志信息

## 预期日志输出

### 客户端日志
```
[INFO] ConnectedCB called, filePath: /path/to/file.txt
[INFO] File opened successfully: /path/to/file.txt, size: 1024 bytes
[INFO] Extracted filename: file.txt
[INFO] Sending upload request: file.txt,1024
[INFO] WriteStr: type=3, data=file.txt,1024, size=13
[INFO] Read message received, type: 4
[INFO] Server accepted upload request
[INFO] Read 1024 bytes from file, sendPos: 0, fileSize: 1024
[INFO] Successfully sent 1024 bytes to server
[INFO] File upload completed
[INFO] Read message received, type: 5
[INFO] upload complete
```

### 服务器日志
```
[INFO] Server received message type: 3
[INFO] Processing MSG_UPLOAD
[INFO] UploadReady called
[INFO] Upload request data: file.txt,1024
[INFO] Parsed filename: file.txt, size: 1024
[INFO] Full file path: ./file.txt
[INFO] File opened successfully for writing
[INFO] Server WriteStr: type=4, data=OK, size=3
[INFO] Upload ready, waiting for file data
[INFO] Attempting to read 1024 bytes, fileRecved: 0, fileSize: 1024
[INFO] Read 1024 bytes from network
[INFO] Written 1024 bytes to file, total received: 1024/1024
[INFO] file recv complete
[INFO] Server WriteStr: type=5, data=OK, size=3
```

## 常见问题排查

### 1. 连接失败
- 检查服务器是否正在运行
- 检查IP地址和端口号是否正确
- 检查防火墙设置

### 2. 文件打开失败
- 检查文件路径是否正确
- 检查文件是否存在
- 检查文件权限

### 3. 服务器文件创建失败
- 检查服务器目录是否存在
- 检查目录权限
- 检查磁盘空间

### 4. 传输中断
- 检查网络连接
- 检查文件大小是否过大
- 检查超时设置

## 性能优化建议

1. **大文件传输**：对于大文件，可以考虑分块传输
2. **并发上传**：可以支持多个文件同时上传
3. **断点续传**：支持传输中断后继续传输
4. **进度显示**：在界面上显示上传进度

## 下一步改进

1. 添加文件上传进度条
2. 支持文件夹上传
3. 添加文件校验（MD5/SHA1）
4. 支持断点续传
5. 添加传输速度限制
