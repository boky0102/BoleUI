CMAKE_MINIMUM_REQUIRED(VERSION 3.31.6)
PROJECT("boleui" LANGUAGES CXX)

# GENERAL #
# zed lsp requirement
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_STANDARD 23)

include(FetchContent)


# PACKAGES #
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(googletest)

# UI LIB #
ADD_SUBDIRECTORY(uilib)


# MAIN #
ADD_EXECUTABLE(main main.cpp)
TARGET_INCLUDE_DIRECTORIES(main PUBLIC include)
TARGET_LINK_LIBRARIES(main PRIVATE SFML::Graphics)


# TEST #
ADD_EXECUTABLE(TestUITree test/TestUitree.cpp)
TARGET_LINK_LIBRARIES(TestUITree GTest::gtest_main uilib)

include(GoogleTest)
gtest_discover_tests(TestUITree)
