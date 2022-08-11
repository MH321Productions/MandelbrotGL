cmake_minimum_required(VERSION 3.16)

project(mandelbrot LANGUAGES CXX)

include_directories(src)
include_directories(lib)
include_directories(include)

#Subdirectories allgemein
set(SDL_LIBC ON)
include_directories(lib/stb)
add_subdirectory(lib/freetype)
include_directories(lib/freetype/include)
add_subdirectory(lib/glm)
include_directories(lib/glm)
add_subdirectory(lib/glew/build/cmake)
include_directories(lib/glew/include)
add_subdirectory(lib/SDL)
include_directories(lib/SDL/include)
#include_directories(lib/portable-file-dialogs)


#Subdirectories Plattformabhängig
if(WIN32)
else()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)

add_compile_definitions(NO_SDL_GLEXT GLEW_STATIC UNICODE _UNICODE SDL_MAIN_HANDLED)

#Wenn Der Build im Debug-Modus ist
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
endif()

#Unterlibraries
add_library(App src/App/App.cpp src/App/InitHandler.cpp src/App/EventLoopHandler.cpp src/App/CleanupHandler.cpp src/App/RenderHandler.cpp)
add_library(Window src/Window/Window.cpp src/Window/Window.hpp)
add_library(Shader src/Shader/Shader.cpp src/Shader/ShaderLoader.cpp)
add_library(Buffer src/Buffer/Buffer.cpp src/Buffer/BufferLoader.cpp)
add_library(Texture src/Texture/Texture.cpp src/Texture/TextureLoader.cpp src/Texture/TextureManager.cpp)
add_library(2D src/2D/Color.cpp src/2D/Renderer.cpp src/2D/Font.cpp src/2D/FontManager.cpp src/2D/String32.cpp src/2D/Geometry.cpp)
add_library(UI src/UI/Overlay.cpp src/UI/Button.cpp src/UI/UIEvents.cpp src/UI/CheckButton.cpp src/UI/RadioButton.cpp)

list(APPEND libs
	App Window Shader Buffer
	Texture UI 2D
	stb SDL2main SDL2-static glew_s
	glm	freetype
)

#Externe Libraries
if (WIN32)
    set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    set(CMAKE_CXX_FLAGS -no-pie)
endif()

#Hauptprogramm
add_executable(Program src/Main.cpp)
set_target_properties(Program PROPERTIES OUTPUT_NAME "Mandelbrot")

if (WIN32)
    target_link_libraries(Program PRIVATE "${libs}")
	
	set_target_properties(${libs} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
	set_target_properties(Program PROPERTIES WIN32_EXECUTABLE TRUE)
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
else()
    target_link_libraries(Program PRIVATE "${libs}")
endif()
