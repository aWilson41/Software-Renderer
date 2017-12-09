cmake_minimum_required(VERSION 3.7)

project(SoftwareRenderer)

file(GLOB H_FILES *.h)
file(GLOB CPP_FILES *.cpp)
file(GLOB LODEPNG LodePNG/*.h LodePNG/*.cpp)

add_executable(SoftwareRenderer ${CPP_FILES} ${H_FILES} ${LODEPNG})

source_group("LodePNG" FILES ${LODEPNG})

find_path(SDL2 SDL)
find_path(ASSIMP Assimp)
target_include_directories(SoftwareRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${ASSIMP}/include ${SDL2}/include)

target_link_libraries(SoftwareRenderer debug ${SDL2}/build/Debug/SDL2d.lib optimized ${SDL2}/build/Release/SDL2.lib)
target_link_libraries(SoftwareRenderer debug ${SDL2}/build/Debug/SDL2maind.lib optimized ${SDL2}/build/Release/SDL2main.lib)
target_link_libraries(SoftwareRenderer debug ${ASSIMP}/lib/assimp-vc140-mt.lib optimized ${ASSIMP}/lib/assimp-vc140-mt.lib)

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(SoftwareRenderer PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
  endif()
endif()