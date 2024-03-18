#!/bin/sh
for i in $(find $(pwd) -name "*.java"); do
    echo "开始格式化$i"
    nohup /data/data/com.termux/files/usr/bin/clang-format -i $i >/dev/null 2>&1 &
done
