#需要配置交叉编译  TODO
set(CMAKE_SYSTEM_NAME Linux) #设置目标系统名字
set(CMAKE_SYSTEM_PROCESSOR arm ) #设置目标处理器架构

# 避免 try_compile 运行 ARM 程序
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


# 指定编译器的 sysroot 路径
#set(TOOLCHAIN_DIR /home/tronlong/RK3576/rk3576_linux6.1_release/ubuntu/sysroots)
#set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/armv8a-linux)

# 指定交叉编译器 arm-linux-gcc 和 arm-linux-g++
#set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/armv8a-linux/usr/bin/aarch64-linux-gnu-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/armv8a-linux/usr/bin/aarch64-linux-gnu-g++)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)  #注意一定要先配置好交叉编译  aarch64-linux-gnu-gcc --version
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

cmake_minimum_required(VERSION 3.22)
project("ForeBLT_NodeController")

#添加公共库
add_subdirectory(can)
add_subdirectory(pwm)
add_subdirectory(usart)
add_subdirectory(gpio)
add_subdirectory(common)