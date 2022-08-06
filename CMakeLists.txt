cmake_minimum_required(VERSION 3.23)
project(Lesson_Multithreading)

#set (BOOST_ROOT "C:/my/library/boost_1_79_0/")
set (CMAKE_CXX_STANDARD 20)
#set (Boost_NO_SYSTEM_PATHS ON)
#set (Boost_USE_MULTITHREADED ON)
#set (Boost_USE_STATIC_LIBS ON)
#set (Boost_USE_STATIC_RUNTIME OFF)
#set (BOOST_ALL_DYN_LINK OFF)

#INCLUDE_DIRECTORIES("C:/my/library/boost_1_79_0/")

#find_package (Boost REQUIRED COMPONENTS Algorithms REQUIRED)


#if (Boost_FOUND)
#    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
#    add_executable (Boost_test "main.cpp")
#    target_link_libraries (Boost_test ${Boost_LIBRARIES})
#endif ()

add_executable(Lesson_Multithreading main.cpp IOFile.cpp IOFile.h util.cpp util.h)

project(Lesson_Multithreading_2)
add_executable(Lesson_Multithreading_2 topic_word.cpp)