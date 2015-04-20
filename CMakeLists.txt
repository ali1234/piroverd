cmake_minimum_required(VERSION 2.8)

PROJECT(piroverd)

SET(piroverd_SOURCES

  main.c
  i2c.c
  mpu6050.c
  ak8975.c
  sn3218.c
  max17043.c
  pca9685.c
  sensors.c
  net.c

)

ADD_DEFINITIONS(-std=c99)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES (DEPS REQUIRED  gio-2.0 glib-2.0 gstreamer-1.0)
INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})
LINK_DIRECTORIES(${DEPS_LIBRARY_DIRS})

ADD_EXECUTABLE(${PROJECT_NAME} ${piroverd_SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DEPS_LIBRARIES} -lgstrtspserver-1.0 ${DEPS_LDFLAGS_OTHER})
