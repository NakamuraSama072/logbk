# 操作を続ける前に必ずお読みください
1. 本 README.MD は現在も継続的に改善中です。
2. プロジェクトは現在アルファ版段階であり、完全なテストは実施されていません。そのため不具合が発生する可能性があります。問題が見つかった場合は issues を提出してください。

# logbk
C++ で書かれたシンプルな Linux ログアーカイブコマンドラインツール

# システム要件
- オペレーティングシステム：Linux（カーネル 6.x バージョンを推奨）
- コンパイラ：GCC 6.1 以上 / Clang 3.4 以上
- 標準：C++ 14
- ビルドシステム：CMake 3.10 以上

# ビルドと実行
```sh
mkdir build
cd build
cmake ..
cmake --build .
sudo ./logbk
```

# 使用方法：
本プログラムの実行には root 権限が必要です。

使用法：
```sh
logbk <log_file> [backup_directory]
logbk help
```

パラメータ説明：
- log_file：バックアップ対象のログファイルのパス
- backup_directory（オプション）：カスタムバックアップディレクトリ（デフォルト：/root/syslogbackup）

例：
```sh
logbk /var/log/syslog
logbk /var/log/auth.log /root/mybackups
logbk help
```

注意事項：
- バックアップはxz圧縮形式（.tar.xz）で保存されます
- バックアップ後、元のログファイルは空になります
- バックアップファイル名にはタイムスタンプ（YYYYMMDD-HH24MMSS）が含まれます
