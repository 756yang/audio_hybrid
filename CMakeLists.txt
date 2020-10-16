cmake_minimum_required (VERSION 2.8)
project(audio_hybrid)
set(CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_FLAGS  "-municode -fexec-charset=UTF-8 -static -O3 -m64")

# 设置链接库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加include路径及源文件路径
include_directories(${PROJECT_SOURCE_DIR}/Include)
aux_source_directory(./src SrcFiles)
aux_source_directory(./ SrcFiles)

# 链接指定库文件
#link_libraries(${PROJECT_NAME})

# 生成可执行文件并设置输出目录
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/Bin)
add_executable(ahc ${SrcFiles})
