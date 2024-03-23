project(OpencvExample)
cmake_minimum_required(VERSION 3.16)
find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
)


add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS})


add_executable(moda1 src/moda1.cpp)
target_link_libraries(moda1 ${OpenCV_LIBS})

add_executable(moda2 src/moda2.cpp)
target_link_libraries(moda2 ${OpenCV_LIBS})

add_executable(moda3 src/moda3.cpp)
target_link_libraries(moda3 ${OpenCV_LIBS})

add_executable(moda4 src/moda4.cpp)
target_link_libraries(moda4 ${OpenCV_LIBS})

add_executable(moda5 src/moda5.cpp)
target_link_libraries(moda5 ${OpenCV_LIBS})