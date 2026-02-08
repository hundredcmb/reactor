# reactor

### 1、概述

+ 仅支持 Linux 系统
+ `tiny_reactor` 目录实现了单线程 reactor 模型
  + 特性：非阻塞 IO、epoll边缘触发
+ `main.c` 实现了一个简单的 echo 服务器

### 2、构建
 
+ 直接运行项目自带的脚本 `build.sh` 会创建一个 `./build/` 目录构建项目
+ 启动 `./echo_server`，默认监听 8080 端口，可以在 `main.c` 中修改
