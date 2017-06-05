cmake_minimum_required (VERSION 3.7)

project(daedalusx3)

set(CMAKE_CXX_STANDARD 14)
#add_compile_options(-Wall -O3 -DNDEBUG)

set(BOOST_ROOT ~/packages/boost)
find_package(Boost)
find_package(Boost REQUIRED system)

if(Boost_FOUND)

    file(GLOB CALC_SRC
        "src/*.cpp"
        "src/*/*.cpp")

    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(src)
    include_directories(src/visitors)
    add_executable(daedalusx3 ${CALC_SRC})
    target_link_libraries(daedalusx3 ${Boost_SYSTEM_LIBRARY})

endif()



