cmake_minimum_required(VERSION 3.28.3)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(esp32_pros3_voice)

# Copy custom voice commands
set(CMD_SRC  ${CMAKE_SOURCE_DIR}/data/commands_en.txt)
set(CMD_DST  ${CMAKE_SOURCE_DIR}/managed_components/espressif__esp-sr/model/multinet_model/fst/commands_en.txt)
configure_file(${CMD_SRC} ${CMD_DST} COPYONLY)
