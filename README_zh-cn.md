# 继续操作前请阅读
1. 本 README.MD 仍在持续完善中。
2. 项目目前处于 alpha 版本阶段，尚未经过全面测试，因此错误在所难免。若发现问题请提交 issues.

# logbk
一款用 C++ 编写的简易 Linux 日志归档命令行程序

# 系统要求
- 操作系统：Linux（建议使用 6.x 版本内核）
- 编译器：GCC 6.1 及以上 / Clang 3.4 及以上
- 标准：C++ 14
- 构建系统：CMake 3.10 及以上

# 构建与运行
```sh
mkdir build
cd build
cmake ..
cmake --build .
sudo ./logbk
```

# 使用说明：
运行该程序需要 root 权限。

用法：
```sh
logbk <log_file> [backup_directory]
logbk help
```

参数说明：
- log_file：待备份清理的日志文件路径
- backup_directory（可选）：自定义备份目录（默认：/root/syslogbackup）

示例：
```sh
logbk /var/log/syslog
logbk /var/log/auth.log /root/mybackups
logbk help
```

注意事项：
- 备份采用 xz 压缩格式（.tar.xz）
- 备份后原始日志文件将被清空
- 备份文件名包含时间戳（YYYYMMDD-HH24MMSS）
