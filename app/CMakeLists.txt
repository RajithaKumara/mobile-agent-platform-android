cmake_minimum_required(VERSION 3.4.1)

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp)

include_directories( SYSTEM /usr/include/node ) # Path to Node.js include directory

add_library( node-lib
        SHARED
        IMPORTED )

set_target_properties(
        node-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libnode.so )

find_library(
        log-lib
        log)

add_library(
        native-activity
        SHARED
        src/main/cpp/native-activity.cpp)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

#set(CMAKE_SHARED_LINKER_FLAGS
#        "${CMAKE_SHARED_LINKER_FLAGS} -u ANodeNativeActivity_onCreate")

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native-activity PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

include_directories( SYSTEM ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h )

target_link_libraries(
        native-lib
        node-lib
#        native_app_glue
        ${log-lib})

target_link_libraries(
        native-activity
        android
        native_app_glue
        EGL
        GLESv1_CM
        ${log-lib})
