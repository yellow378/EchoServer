# TODO

## 退出MainLoop

## 多进程问题

### 高优先级

- [x] 修复资源泄漏：父进程未关闭 client socket
  - 位置: tcp.c:102-108
  - 父进程 accept 后应关闭 client fd

- [x] 修复资源泄漏：子进程未关闭 server socket
  - 位置: tcp.c:76-77
  - 子进程应关闭继承的 server fd

- [x] 修复僵尸进程问题
  - 位置: tcp.c:107
  - 需要处理 SIGCHLD 信号或调用 wait()

### 中优先级

- [ ] 修复缓冲区溢出风险
  - 位置: tcp.c:94-95
  - recv_size 可能为 100，`buffer[100] = '\0'` 会越界
  - 建议: buffer 设为 101 字节，或限制 recv 长度为 99

## 更多Feature

- [ ] Floating Point
  - feature: show bytes format
- [x] Argument Parser
