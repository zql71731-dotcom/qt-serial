cmake_minimum_required(VERSION 3.16)
project(serial CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ★ Qt 特有的三个开关
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ★ 找 Qt5 的模块
find_package(Qt5 COMPONENTS Widgets SerialPort Charts REQUIRED)

add_executable(serial main.cpp)

# ★ 链接 Qt 模块
target_link_libraries(serial PRIVATE
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Charts
)
