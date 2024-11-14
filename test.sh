#!/bin/bash

# 定义测试命令
COMMAND="./philo 5 800 200 200"

# 定义最大测试时间（秒）
MAX_TIME=7

# 循环执行测试
while true; do
    # 使用临时文件存储输出
    OUTPUT_FILE=$(mktemp)

    # 执行命令并将输出重定向到临时文件
    timeout $MAX_TIME $COMMAND > $OUTPUT_FILE 2>&1 &
    PID=$!

    # 等待命令执行完成或超时
    wait $PID
    STATUS=$?

    # 检查是否超时
    if [ $STATUS -eq 124 ]; then
        echo "测试超过 ${MAX_TIME} 秒，结束当前测试，进入下一循环..."
        # 结束超时进程
        kill -9 $PID 2>/dev/null
        continue
    fi

    # 检查输出中是否有 "died"
    if grep -q "died" $OUTPUT_FILE; then
        echo "检测到哲学家死亡，测试终止。输出如下："
        cat $OUTPUT_FILE
        # 删除临时文件
        rm -f $OUTPUT_FILE
        exit 0
    fi

    # 删除临时文件
    rm -f $OUTPUT_FILE
done
