cmake_minimum_required(VERSION 3.0)

#################################
#### 1. Update Project Name  ####
#################################
project(bolt)


##################################
#### 2. Update Board Settings ####
##################################
set(PORT "/dev/ttyACM0")
set(MCU "atmega328p")
set(CPU_SPEED "16000000")
set(PIN_VARIANT "standard")
set(PROGRAMMER "stk500v1")


##############################################
#### 3. Set the path to the Arduino tools ####
##############################################
set(ARDUINO_PACKAGES "$ENV{HOME}/.arduino15/packages/arduino")
set(ARDUINO_TOOLS ${ARDUINO_PACKAGES}/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2)
set(ARDUINO_AVR ${ARDUINO_PACKAGES}/hardware/avr/1.6.23)
set(ARDUINO_AVRDUDE ${ARDUINO_PACKAGES}/tools/avrdude/6.3.0-arduino14)


###################################
#### 4. Add external libraries ####
###################################
set(EXTERNAL_DEPENDENCIES
        ${ARDUINO_AVR}/cores/arduino
        ${ARDUINO_AVR}/variants/${PIN_VARIANT}
        ${ARDUINO_AVR}/libraries/SoftwareSerial/src
        )

###############################################################
#### 5. Add all sources files except the mains (only .cpp) ####
###############################################################
file(GLOB_RECURSE SOURCE_FILES lib/*.cpp)


#############################################
#### 6. Update list of main source files ####
#############################################
set(MAIN_FILES
        main.cpp
        tests.cpp
        bluetooth-send-receive-test.cpp)




## CODE BELOW ##

# Verify that the specified paths exist
if (NOT EXISTS ${ARDUINO_PACKAGES})
    message(FATAL_ERROR "${ARDUINO_PACKAGES} is not a directory. Update CMakeLists.txt.")
endif ()

if (NOT EXISTS ${ARDUINO_TOOLS})
    message(FATAL_ERROR "${ARDUINO_TOOLS} is not a directory. Update CMakeLists.txt.")
endif ()

if (NOT EXISTS ${ARDUINO_AVR})
    message(FATAL_ERROR "${ARDUINO_AVR} is not a directory. Update CMakeLists.txt.")
endif ()

if (NOT EXISTS ${ARDUINO_AVRDUDE})
    message(FATAL_ERROR "${ARDUINO_AVRDUDE} is not a directory. Update CMakeLists.txt.")
endif ()

#Include dependencies
include_directories(${EXTERNAL_DEPENDENCIES})

# Add to the sources list files in the dependencies
foreach(DIRECTORY ${EXTERNAL_DEPENDENCIES})
    file(GLOB_RECURSE CURRENT_FILES
            ${DIRECTORY}/*.cpp
            ${DIRECTORY}/*.c)

    list(APPEND SOURCE_FILES ${CURRENT_FILES})
endforeach(DIRECTORY)

# Set compiler suite specification
set(CMAKE_C_COMPILER ${ARDUINO_TOOLS}/bin/avr-gcc)
set(CMAKE_CXX_COMPILER ${ARDUINO_TOOLS}/bin/avr-g++)
set(CMAKE_OBJCOPY ${ARDUINO_TOOLS}/bin/avr-objcopy)
set(CMAKE_OBJDUMP ${ARDUINO_TOOLS}/bin/avr-objdump)
set(CMAKE_RANLIB ${ARDUINO_TOOLS}/bin/avr-ranlib)
set(CMAKE_LINKER ${ARDUINO_TOOLS}/bin/avr-ld)

# Set compiler flags
add_definitions(-mmcu=${MCU} -DF_CPU=${CPU_SPEED} -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections)

# Set linker flags
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")   # remove -rdynamic for C
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "") # remove -rdynamic for CXX
set(CMAKE_EXE_LINKER_FLAGS "-Os -Wl,--gc-sections -mmcu=${MCU}")

# Create an executable and an upload target for each main file
foreach(FILE ${MAIN_FILES})
    add_executable(${FILE} ${FILE} ${SOURCE_FILES})
    add_custom_target(upload-${FILE}
            COMMAND ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${FILE} ${PROJECT_NAME}.hex # Create hex file
            COMMAND ${ARDUINO_AVRDUDE}/bin/avrdude -C${ARDUINO_AVRDUDE}/etc/avrdude.conf -F -p${MCU} -c${PROGRAMMER} -P${PORT} -D -Uflash:w:${PROJECT_NAME}.hex:i # Flash hex file to arduino
            DEPENDS ${FILE})
endforeach(FILE)