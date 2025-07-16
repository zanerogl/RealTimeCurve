cmake_minimum_required(VERSION 3.5)

project(RealTimeCurve LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt5 包
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

# 设置源文件
set(SOURCES
    main.cpp
    basewidget.cpp
    3rd/qcustomplot/qcustomplot.cpp
)

# 设置头文件
set(HEADERS
    basewidget.h
    3rd/qcustomplot/qcustomplot.h
        zWidgets/zcustomplot.cpp zWidgets/zcustomplot.h)

# 运行 moc 处理需要 Q_OBJECT 的头文件
set(MOC_HEADERS
    basewidget.h
    3rd/qcustomplot/qcustomplot.h
)

# 生成 MOC 文件
qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${MOC_SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
        3rd/qcustomplot
)

# 链接 Qt5 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::PrintSupport
)

# 安装目标（可选）
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
