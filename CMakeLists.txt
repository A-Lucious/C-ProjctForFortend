cmake_minimum_required(VERSION 4.2)

project(HelloWorld)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 定义一个 PROJECT_ROOT_DIR 宏，指向项目根目录
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 1. 预处理库文件：将 src/lib 和inlcude下的源码编译为名为 MyPrivLib 的静态库
aux_source_directory(./src/lib/ LIB_SRCS)
add_library(MyPrivLib STATIC ${LIB_SRCS})


# 2. 指定库的头文件路径
target_include_directories(MyPrivLib PUBLIC 
                            src/lib 
                            include 
                            include/json/single_include
                            include/crow_all)

aux_source_directory(. DIR_SRCS)
add_executable(HelloWorld ${DIR_SRCS} )

target_link_libraries(HelloWorld MyPrivLib sqlite3 )