# joker2 CMake config file
#
# This file sets the following variables:
# joker2_FOUND - Always TRUE.
# joker2_INCLUDE_DIRS - Directories containing the joker2 include files.
# joker2_IDL_DIRS - Directories containing the joker2 IDL files.
# joker2_LIBRARIES - Libraries needed to use joker2.
# joker2_DEFINITIONS - Compiler flags for joker2.
# joker2_VERSION - The version of joker2 found.
# joker2_VERSION_MAJOR - The major version of joker2 found.
# joker2_VERSION_MINOR - The minor version of joker2 found.
# joker2_VERSION_REVISION - The revision version of joker2 found.
# joker2_VERSION_CANDIDATE - The candidate version of joker2 found.

message(STATUS "Found joker2-1.0.0")
set(joker2_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(joker2_INCLUDE_DIRS
#    "C:/Program Files (x86)/joker2/include/joker2-1"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(joker2_IDL_DIRS
#    "C:/Program Files (x86)/joker2/include/joker2-1/idl")
set(joker2_INCLUDE_DIRS
    "C:/Program Files (x86)/joker2/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(joker2_IDL_DIRS
    "C:/Program Files (x86)/joker2/include//idl")


if(WIN32)
    set(joker2_LIBRARIES
        "C:/Program Files (x86)/joker2/components/lib/joker2.lib"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(joker2_LIBRARIES
        "C:/Program Files (x86)/joker2/components/lib/joker2.dll"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(joker2_DEFINITIONS ${<dependency>_DEFINITIONS})

set(joker2_VERSION 1.0.0)
set(joker2_VERSION_MAJOR 1)
set(joker2_VERSION_MINOR 0)
set(joker2_VERSION_REVISION 0)
set(joker2_VERSION_CANDIDATE )

