cmake_minimum_required(VERSION 3.20)
project(rhasher VERSION 1.0 LANGUAGES C)

option(WITH_READLINE "Use readline when available" ON)

find_path(RHASH_INCLUDE_DIR NAMES rhash.h REQUIRED)
find_library(RHASH_LIBRARY NAMES rhash REQUIRED)

add_executable(rhasher rhasher.c)
target_compile_features(rhasher PRIVATE c_std_11)
target_compile_definitions(rhasher PRIVATE _POSIX_C_SOURCE=200809L)
target_include_directories(rhasher PRIVATE "${RHASH_INCLUDE_DIR}")
target_link_libraries(rhasher PRIVATE "${RHASH_LIBRARY}")

set(HAVE_READLINE OFF)
if(WITH_READLINE)
    find_path(READLINE_INCLUDE_DIR NAMES readline/readline.h)
    find_library(READLINE_LIBRARY NAMES readline)
    if(READLINE_INCLUDE_DIR AND READLINE_LIBRARY)
        set(HAVE_READLINE ON)
        target_compile_definitions(rhasher PRIVATE HAVE_READLINE=1)
        target_include_directories(rhasher PRIVATE "${READLINE_INCLUDE_DIR}")
        target_link_libraries(rhasher PRIVATE "${READLINE_LIBRARY}")
    endif()
endif()
message(STATUS "Readline support: ${HAVE_READLINE}")

include(CTest)
if(BUILD_TESTING)
    find_program(SH_EXECUTABLE NAMES sh REQUIRED)
    find_program(MD5SUM_EXECUTABLE NAMES md5sum REQUIRED)
    find_program(SHA1SUM_EXECUTABLE NAMES sha1sum REQUIRED)
    add_test(NAME hashes COMMAND "${SH_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/test.sh" "$<TARGET_FILE:rhasher>"
        "${MD5SUM_EXECUTABLE}" "${SHA1SUM_EXECUTABLE}")
endif()

include(GNUInstallDirs)
install(TARGETS rhasher RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
