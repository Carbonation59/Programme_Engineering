cmake_minimum_required(VERSION 3.2)

SET (EXAMPLE_BINARY "lab_06")
SET (WRITER_BINARY  "event_writer")

find_package(OpenSSL)
find_package(Threads)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL Data JWT MongoDB)
find_package(CppKafka REQUIRED)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

SET(STD_CXX "c++17")
SET(REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${EXAMPLE_BINARY} main.cpp
                                 config/config.cpp 
                                 database/database.cpp 
                                 database/user.cpp
                                 database/product.cpp
                                 database/cache.cpp)

add_executable(${WRITER_BINARY} writer_main.cpp
                                 config/config.cpp 
                                 database/database.cpp 
                                 database/user.cpp
                                 database/product.cpp
                                 database/cache.cpp)



target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoDataPostgreSQL"
                             "rdkafka++"
                             CppKafka::cppkafka)

target_include_directories(${WRITER_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${WRITER_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${WRITER_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${WRITER_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${WRITER_BINARY} PRIVATE 
                            ${CMAKE_THREAD_LIBS_INIT} 
                            ${Poco_LIBRARIES}
                            "PocoDataPostgreSQL"
                            "rdkafka++"
                            CppKafka::cppkafka)
                             



