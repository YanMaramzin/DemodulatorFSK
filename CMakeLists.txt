cmake_minimum_required(VERSION 3.5)

project(DemodFSK LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(TestTask/libs/WavReader)
add_subdirectory(TestTask/libs/DemodulatorFSK)
add_subdirectory(TestTask/libs/Filter)



add_executable(DemodFSK main.cpp)

target_link_libraries(${PROJECT_NAME} WavReader)
target_link_libraries(${PROJECT_NAME} DemodulatorFSK)
target_link_libraries(${PROJECT_NAME} Filter)

