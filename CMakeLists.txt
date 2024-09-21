cmake_minimum_required (VERSION 3.14)

project (aquamarine)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif (MSVC_VERSION GREATER_EQUAL "1900")
    add_compile_options("/std:c++17")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
# not used, in case of need fno-rtti can be removed
add_compile_options(-fno-rtti)

find_package (Threads REQUIRED)
IF (UNIX OR APPLE)
find_package (SQLite3 REQUIRED)
find_package (JPEG REQUIRED)
ENDIF()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
IF (UNIX OR APPLE)
include_directories(${SQLite3_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
ENDIF()

#add_subdirectory(sh_mem)

set(AM_SOURCE_FILES 
    analyze/AffinityComparer.cpp 
    analyze/ThresholdDiffChecker.cpp 
    analyze/algorithm/ObjectBase.cpp
    analyze/algorithm/Object.cpp
    analyze/algorithm/ObjectRectangle.cpp
    analyze/algorithm/BfsObjectDetector.cpp 
    analyze/algorithm/ObjectDetector.cpp 
    analyze/algorithm/DiffObjectDetector.cpp 
    analyze/algorithm/ImagePair.cpp 
    analyze/algorithm/movement/MovementDetector.cpp
    extraction/BmpExtractor.cpp
    extraction/MultipleExtractor.cpp
    common/Timers.hpp
    AmApi.cpp
    service/ConnectionsInfo.cpp
    service/SilberService.cpp
)

# Unpack the archive based on the platform
if(UNIX AND NOT APPLE)
    set(LIB_URL "https://github.com/MaksymT17/silber/releases/download/1.1/linux_release.zip")
    set(LIB_NAME "linux_release.zip")
    set(LIB_PATH "${CMAKE_BINARY_DIR}/${LIB_NAME}")  # Full path including filename
    file(DOWNLOAD ${LIB_URL} ${LIB_PATH})
    execute_process(COMMAND unzip ${LIB_PATH} -d ${CMAKE_BINARY_DIR})

elseif(APPLE)
    set(LIB_URL "https://github.com/MaksymT17/silber/releases/download/1.1/macos_release.zip")
    set(LIB_NAME "macos_release.zip")
    set(LIB_PATH "${CMAKE_BINARY_DIR}/${LIB_NAME}")  # Full path including filename
    file(DOWNLOAD ${LIB_URL} ${LIB_PATH})
    execute_process(COMMAND unzip ${LIB_PATH} -d ${CMAKE_BINARY_DIR})

elseif(WIN32)
    set(LIB_URL "https://github.com/MaksymT17/silber/releases/download/1.1/windows_debug.zip")
    set(LIB_NAME "windows_debug.zip")
    set(LIB_PATH "${CMAKE_BINARY_DIR}/${LIB_NAME}")  # Full path including filename
    file(DOWNLOAD ${LIB_URL} ${LIB_PATH})
    execute_process(COMMAND powershell -Command "Expand-Archive -Path ${LIB_PATH} -DestinationPath '${CMAKE_BINARY_DIR}'")

else()
#print unsupported OS
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/lib")
file(COPY "${CMAKE_BINARY_DIR}/lib" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/include")
file(COPY "${CMAKE_BINARY_DIR}/include" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/lib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/silber/include")

IF (UNIX OR APPLE)
add_library(aquamarine_lib STATIC ${AM_SOURCE_FILES} ${JPEG_LIBRARIES} database/DataBaseCommunicator.cpp extraction/JpgExtractor.cpp)
ELSE()
add_library(aquamarine_lib STATIC ${AM_SOURCE_FILES} ${JPEG_LIBRARIES})
ENDIF()

add_executable(aquamarine main.cpp)

target_link_libraries(aquamarine aquamarine_lib silber-1.1 ${CMAKE_THREAD_LIBS_INIT})
IF (UNIX OR APPLE)
target_link_libraries(aquamarine ${SQLite3_LIBRARIES} ${JPEG_LIBRARIES} )
ENDIF()
