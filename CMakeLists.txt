cmake_minimum_required(VERSION 3.18)

project("PlantsvsZombies-TwoPlayerEdition")

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.h include/*.hpp)

include_directories("D:/visual-studio/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/include")
link_directories("D:/visual-studio/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64")

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} gdi32 EasyXa.lib)
