cmake_minimum_required(VERSION 2.8)

PROJECT(piroverd)

SET(piroverd_SOURCES

  main.cpp
  i2cdevice.cpp
  mpu6050.cpp
  ak8975.cpp
  sn3218.cpp
  max17043.cpp
  pca9685.cpp
  sensors.cpp
  outputs.cpp

)


INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES (DEPS REQUIRED  gio-2.0 glib-2.0 gstreamer-1.0)
INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})
LINK_DIRECTORIES(${DEPS_LIBRARY_DIRS})

ADD_EXECUTABLE(${PROJECT_NAME} ${piroverd_SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DEPS_LIBRARIES} -lgstrtspserver-1.0 -lm ${DEPS_LDFLAGS_OTHER})
