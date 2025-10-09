# !/bin/bash

# 查找 knowledge 目录下所有的 Makefile 文件
makefiles=$(find ./ -name "Makefile" -type f)

# 遍历所有找到的 Makefile 文件
for makefile in $makefiles; do

    # 提取 Makefile 所在的目录
    makefile_dir=$(dirname "$makefile")
    
    echo "正在处理目录: $makefile_dir"
    
    # 进入 Makefile 所在的目录
    cd "$makefile_dir" || {
        echo "无法进入目录 $makefile_dir，脚本终止。"
        exit 1
    }
    
    # 执行 make 命令
    make
    
    # 检查 make 命令的返回状态
    if [ $? -ne 0 ]; then
        echo "在目录 $makefile_dir 中执行 make 命令失败，脚本终止。"
        exit 1
    fi
    
    echo "在目录 $makefile_dir 中执行 make 命令成功。"
    
    # 返回上级目录，继续下一个 Makefile 的处理
    cd - > /dev/null
done

echo "所有 Makefile 都已成功执行。"