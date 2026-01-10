ADD_LIBRARY(testgrid
    grid.cpp
    grid.h
)

TARGET_LINK_LIBRARIES(testgrid PRIVATE SFML::Graphics)

ADD_EXECUTABLE(gridrunner main.cpp)
TARGET_LINK_LIBRARIES(gridrunner PRIVATE SFML::Graphics testgrid uilib)
