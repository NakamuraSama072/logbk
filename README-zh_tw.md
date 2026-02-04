# 繼續操作前請詳閱
1. 此 README.MD 文件仍在持續更新中。
2. 專案目前處於 alpha 版本階段，尚未完成全面測試，因此錯誤在所難免。若發現問題請提交 issues.

# logbk
一款以 C++ 編寫的簡易 Linux 日誌歸檔命令列程式

# 系統需求
- 作業系統：Linux（建議 6.x 以上版本内核）
- 編譯器：GCC 6.1 或更新版本 / Clang 3.4 或更新版本
- 標準：C++ 14
- 建置系統：CMake 3.10 或更新版本

# 建置與執行
```sh
mkdir build
cd build
cmake ..
cmake --build .
sudo ./logbk
```

# 使用方式：
你需要具備 root 權限以執行该程式。

使用方式：
```sh
logbk <log_file> [backup_directory]
logbk help
```

參數說明：
- log_file：需備份及清理之日誌檔案路徑
- backup_directory（可選）：自訂備份目錄（預設：/root/syslogbackup）

範例：
```sh
logbk /var/log/syslog
logbk /var/log/auth.log /root/mybackups
logbk help
```

注意事項：
- 備份檔採用 xz 壓縮格式 (.tar.xz)
- 原始日誌檔於備份後自動清空
- 備份檔名稱含時間戳記 (YYYYMMDD-HH24MMSS)
