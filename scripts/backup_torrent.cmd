chcp 1251
echo off
mkdir D:\Backup\uTorrent\Options\BackUp_%date%
copy "%appdata%\uTorrent\" "D:\Backup\uTorrent\Options\BackUp_%date%"
echo backup finished.