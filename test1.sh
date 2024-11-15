#!/bin/bash

# 测试次数
total_tests=500
failures=0

# 开始测试
for ((i = 1; i <= total_tests; i++)); do
    echo "Running test $i / $total_tests"

    # 使用超时机制，限制程序最多运行 15 秒
    output=$(timeout 15s ./philo 4 310 200 100)

    # 检查输出中是否包含 "died"
    if echo "$output" | grep -q "died"; then
        echo "Test $i failed: A philosopher died."
        failures=$((failures + 1))
        continue  # 检测到失败后跳过本次测试，进入下一次
    fi

    echo "Test $i passed: No philosopher died."
done

# 打印最终结果
echo "Total failures: $failures / $total_tests"
