cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(yamrs VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(yamrs VERSION 7.7.7)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set (SRC_FILES 
                common.hpp
                utils.hpp             utils.cpp
                mapper.hpp            mapper.cpp
                hashSorter.hpp        hashSorter.cpp
                reducer.hpp           reducer.cpp
                min_pref_handler.hpp  min_pref_handler.cpp
                )


set(EXE_FILES main.cpp)

set(TEST_FILES 
                test_MinPrefix.cpp
                test_mapper.cpp
                test_shuffle.cpp
                test_reducer.cpp)

add_executable(yamr ${EXE_FILES} ${SRC_FILES})

set(TESTS   
            test_MinPrefix
            test_mapper
            test_shuffle
            test_reducer)

list(LENGTH TESTS len_TESTS_r)
math(EXPR len_TESTS "${len_TESTS_r} - 1")

foreach(i RANGE ${len_TESTS})
    list(GET TESTS ${i} test_name)
    list(GET TEST_FILES ${i} test_file)

    add_executable(
        ${test_name}
        ${test_file}
        ${SRC_FILES}
    ) 

endforeach(i)

set(CMP_OPTIONS "-lstdc++fs;-pthread;-Wpedantic;-Wall;-Wextra")
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(yamr  PUBLIC -DDEBUG)  # -D removed
    list(APPEND CMP_OPTIONS "-g")
    list(APPEND CMP_OPTIONS "-fsanitize=thread")
    # list(APPEND CMP_OPTIONS "-fsanitize=address")
    # list(APPEND CMP_OPTIONS "-fsanitize=leak")
    message("debug")
endif()

set_target_properties(yamr ${TESTS} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "${CMP_OPTIONS}"
    LINK_FLAGS "-fsanitize=thread"
    # LINK_FLAGS "-fsanitize=address"
    # LINK_FLAGS "-fsanitize=leak"
)

set_target_properties(${TESTS} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


target_link_libraries(yamr
    stdc++fs
    Threads::Threads
)

foreach(i RANGE ${len_TESTS})
    list(GET TESTS ${i} element)
    message(${element})

    target_link_libraries(
        ${element}
        stdc++fs
        Threads::Threads
        ${Boost_LIBRARIES}
    )   

endforeach(i)

install(TARGETS yamr RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT nikseas@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()
add_test(${TESTS} ${TESTS})
