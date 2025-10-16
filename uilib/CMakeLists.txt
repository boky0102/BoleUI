ADD_LIBRARY(uilib
    uielement.cpp
    uitree.cpp
    renderer.cpp
)

# TEST #
ADD_EXECUTABLE(TestUITree
    _test/TestUielement.cpp
    _test/TestRenderer.cpp
)

TARGET_LINK_LIBRARIES(TestUITree
    GTest::gtest_main
    SFML::Graphics
    uilib
)

TARGET_INCLUDE_DIRECTORIES(uilib PUBLIC include)
TARGET_LINK_LIBRARIES(uilib PRIVATE SFML::Graphics)

include(GoogleTest)
gtest_discover_tests(TestUITree)
