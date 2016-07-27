cmake_minimum_required(VERSION 3.6)

#set( GTF_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( GTF_OUTPUT_DIR bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GTF_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GTF_OUTPUT_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GTF_OUTPUT_DIR} )

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GTF_OUTPUT_DIR}/${OUTPUTCONFIG} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GTF_OUTPUT_DIR}/${OUTPUTCONFIG} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GTF_OUTPUT_DIR}/${OUTPUTCONFIG} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c89 -pedantic")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -Wall -std=c++11 -pedantic")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")	
add_definitions(-D_GLFW_DARWIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
add_definitions(-D_GLFW_LINUX)
else()	
add_definitions(-D_GLFW_WIN32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#gtf
include_directories(include thirdparty/glm thirdparty/imgui thirdparty/tinylibs external/glfw/include external/FBX/2016.1.2/include)
file(GLOB INCLUDES "include/**/*.h" "src/**/*.h")
file(GLOB SOURCES "src/**/*.c" "src/**/*.cpp"  )
add_library(gtf STATIC ${SOURCES} ${INCLUDES})
target_include_directories(gtf PUBLIC include thirdparty/glm thirdparty/imgui thirdparty/tinylibs external/glfw/include external/FBX/2016.1.2/include)

#gtf_thirdparty
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")	
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
else()	
file(GLOB GLFW_SOURCES external/glfw/src/context.c external/glfw/src/egl_context.c external/glfw/src/init.c external/glfw/src/input.c external/glfw/src/monitor.c external/glfw/src/vulkan.c external/glfw/src/wgl_context.c external/glfw/src/win32_init.c external/glfw/src/win32_joystick.c external/glfw/src/win32_monitor.c external/glfw/src/win32_time.c external/glfw/src/win32_tls.c external/glfw/src/win32_window.c external/glfw/src/window.c)
endif()
file(GLOB INCLUDES "thirdparty/imgui/*.h" "thirdparty/tinylibs/*.h")
file(GLOB SOURCES "thirdparty/imgui/*.c" "thirdparty/imgui/*.cpp" "thirdparty/tinylibs/*.c" "thirdparty/tinylibs/*.cpp" ${GLFW_SOURCES})
add_library(gtf_thirdparty STATIC ${SOURCES} ${INCLUDES})
target_include_directories(gtf_thirdparty PUBLIC include thirdparty/glm thirdparty/imgui thirdparty/tinylibs external/glfw/include external/FBX/2016.1.2/include)


#example - CalcGraph
file(GLOB INCLUDES "examples/CalcGraph/*.h")
file(GLOB SOURCES "examples/CalcGraph/*.c" "examples/CalcGraph/*.cpp" )
add_executable(CalcGraph ${SOURCES} ${INCLUDES})
target_include_directories(CalcGraph PUBLIC include thirdparty/glm thirdparty/imgui thirdparty/tinylibs external/glfw/include external/FBX/2016.1.2/include)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")	
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
else()	
target_link_libraries(CalcGraph gtf)
target_link_libraries(CalcGraph gtf_thirdparty)
target_link_libraries(CalcGraph opengl32)
endif()

#example - NoiseGenerator
file(GLOB INCLUDES "examples/NoiseGenerator/*.h")
file(GLOB SOURCES "examples/NoiseGenerator/*.c" "examples/NoiseGenerator/*.cpp" )
add_executable(NoiseGenerator ${SOURCES} ${INCLUDES})
target_include_directories(NoiseGenerator PUBLIC include thirdparty/glm thirdparty/imgui thirdparty/tinylibs external/glfw/include external/FBX/2016.1.2/include)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")	
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
else()	
target_link_libraries(NoiseGenerator gtf)
target_link_libraries(NoiseGenerator gtf_thirdparty)
target_link_libraries(NoiseGenerator opengl32)
endif()