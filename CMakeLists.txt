cmake_minimum_required(VERSION 3.8.2)

# This application has its own Kconfig options.
set(KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig)

# CONF_FILE is prj.conf, plus (in addition to, not or)
# boards/${BOARD}.conf, if it exists. The board-specific file needs to
# be an absolute path; otherwise, the behavior of EXISTS is not
# well-defined.
set(CONF_FILE prj.conf)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}.conf)
  set(CONF_FILE ${CONF_FILE} " boards/${BOARD}.conf")
endif()

# Mandatory Zephyr boilerplate.
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

target_sources(app PRIVATE src/product_id.c)

# Application build configuration.
target_include_directories(app PRIVATE $ENV{ZEPHYR_BASE}/tests/include)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/app_work_queue.c)
target_sources(app PRIVATE src/elce2018_mqtt.c)
target_sources_ifdef(CONFIG_BT               app PRIVATE src/bluetooth.c)
