cmake_minimum_required(VERSION 3.12)

# 工程信息
project(ctp CXX)

# 设置版本号
#set(ctp_VERSION_MAJOR 1)

# C++标准
set(CMAKE_CXX_STANDARD 11)

# 项目根目录
set(project_dir "/chan/ctp")

# 编译文件目录
set(src_dir "/chan/ctp/src")

# 添加头文件
include_directories(${project_dir}/ctp_api
                    ${project_dir}/src
                    ${project_dir}/cppkafka)

# 添加库文件目录
link_directories(${project_dir}/ctp_api
                 ${project_dir}/cppkafka) 

# 将需要编译的CPP加入list
set(src_list ${src_dir}/main.cpp
             ${src_dir}/md_spi.cpp
             )

# 编译CPP文件
add_executable(ctp ${src_list})

# 链接动态库
target_link_libraries(ctp thostmduserapi.so cppkafka.so)