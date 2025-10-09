ADD_LIBRARY(uilib uitree.cpp renderer.cpp)

TARGET_INCLUDE_DIRECTORIES(uilib PUBLIC include)
TARGET_LINK_LIBRARIES(uilib PRIVATE SFML::Graphics)
