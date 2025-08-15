# Lambda回调修复总结

## 问题分析

### 原始问题
- 程序点击刷新后自动关闭
- 访问违规错误

### 根本原因
- Lambda表达式捕获了 `this` 指针：`[this](std::string dirs)->void`
- 当任务对象被删除时，lambda仍然持有 `this` 指针，导致悬空指针

## 解决方案

### 1. 使用Lambda但不捕获this

**修改前**：
```cpp
task->setDirCallback([this](std::string dirs)->void{
    LOG_INFO("client getDirq, emit SDir signal");
    SDir(dirs); // 直接使用this指针
});
```

**修改后**：
```cpp
task->setDirCallback([](std::string dirs)->void{
    LOG_INFO("client getDir, emit SDir signal");
    // 使用全局函数或静态函数来发送信号
    XDiskClient::Get()->SDir(dirs);
});
```

### 2. 修复XDiskClient类

**添加QObject继承**：
```cpp
class XDiskClient : public QObject{
    Q_OBJECT
    
public:
    static XDiskClient* Get() {
        static XDiskClient xc;
        return &xc;
    }
    
signals:
    void SDir(std::string dirs);
    
private:
    XDiskClient() : QObject(nullptr) {};
    // ...
};
```

## 修复原理

1. **避免this指针捕获** - Lambda表达式不捕获任何外部变量
2. **使用全局访问** - 通过 `XDiskClient::Get()` 静态方法访问实例
3. **Qt信号槽支持** - 继承QObject以支持信号槽机制

## 优势

1. ✅ **无悬空指针** - Lambda不持有任何对象指针
2. ✅ **内存安全** - 避免回调函数访问已删除的对象
3. ✅ **简单可靠** - 使用全局单例模式访问对象
4. ✅ **Qt兼容** - 支持Qt的信号槽机制

## 测试建议

现在可以测试修复后的程序：

1. **启动服务器**：`./xdisk_server.exe`
2. **启动客户端**：`./xdisk_gui.exe`
3. **测试功能**：
   - 点击"刷新"按钮
   - 尝试上传文件
   - 观察程序是否稳定运行

## 预期结果

- 程序应该稳定运行，不再自动关闭
- 刷新和上传功能应该正常工作
- 无访问违规错误

如果还有问题，请提供新的错误信息。
