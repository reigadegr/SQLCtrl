#!/bin/sh
#push之前进行项目的启动测试
#如果失败了则不进行push
#mvn spring-boot:run || exit 1

rm -rf $(find $(pwd) -name "*.bak")

#通用推送模板
git gc --prune=all
git add .
git commit -m "$1"
git push origin main
