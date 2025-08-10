## moudle

### xcom 共有工具模块

- 跨平台的线程池库
- 任务类封装

### xdisk_server 云盘服务器

### xdisk_gui 云盘客户端

## Third Party

**windows**: msvc
**linux**: mingw

- openssl-3.5.1
- libevent-2.1.12-stable
- zlib-1.3.1


## 图图

![](assets\imgs\image.png)

```mermaid
sequenceDiagram
	participant A as xdir
	participant B as xfile_serverxdir
	A ->> B: 请求连接
	activate B
	B -->> A: 接受连接
	deactivate B
	activate A
	A ->> A: 连接成功回调 Connected
	A ->> B: 请求目录 GetDir Write MSG_GETDIR
	deactivate A
	activate B
	B ->> B:  MSG_GETDIR MSG_DIRLIST
	B -->> A : 返回目录 GetDir Write MSG_DIRLIST
	deactivate B
```