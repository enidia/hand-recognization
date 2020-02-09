cmake_minimum_required(VERSION 3.10)
project(opencv)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

# 找到在当前目录下的所有代码文件，存储到"SOURCE_FILES"变量中
aux_source_directory(. SOURCE_FILES)

# 定义需要include的头文件的位置
include_directories(${OpenCV_INCLUDE_DIRS})

# 添加个可执行的输出，第一个参数是输出名，必须唯一，第二个源文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES} MoveHandle.cpp MoveHandle.h HotKey.cpp HotKey.h)

# 用来链接库，注意这个函数要在"add_executable"之后使用
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})