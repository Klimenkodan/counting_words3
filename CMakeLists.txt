cmake_minimum_required(VERSION 3.15)
project(counting_words3)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

find_package(Boost REQUIRED COMPONENTS filesystem locale system)

find_package(LibArchive REQUIRED)

find_package(TBB REQUIRED)

set(Boost_USE_MULTITHREADED ON)

set(HEADER_FILES include/config_parser.h include/operations_with_text.h include/safeQueue.h include/operations_with_files.h include/states_conveyor.h)


add_executable(counting_words3 main.cpp src/config_parser.cpp src/operations_with_text.cpp src/operations_with_files.cpp src/states_conveyor.cpp)
target_link_libraries(counting_words3 Threads::Threads Boost::filesystem Boost::locale Boost::system ${LibArchive_LIBRARIES} TBB::tbb)






#set(CMAKE_CXX_STANDARD_LIBRARIES -lboost_locale)



#find_package(Threads REQUIRED)

#add_executable(counting_words1 main.cpp)

#target_link_libraries(counting_words1 Threads::Threads ${LibArchive_LIBRARIES})
