# Distributed under the MIT License. See accompanying LICENSE.md

#[=======================================================================[.rst:
FindStb
-------

Finds the Stb headers.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Stb::Stb``
  Stb

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Stb_FOUND``
  True if the system has the Stb.
``Stb_INCLUDE_DIRS``
  Include directory needed to use Stb.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Stb_INCLUDE_DIR``
  The directory containing ``stb.h``.

#]=======================================================================]

include(FindPackageHandleStandardArgs)

# Stb is a header only library, so all we need to do is find the directory in
# which it resides FIXME: Currently only looks for the headers I actually use in
# the project, should look for all the headers
find_path(
    Stb_INCLUDE_DIR
    NAMES "stb.h" "stb_image.h"
    HINTS "/usr/include" "/usr/include/stb")

find_package_handle_standard_args(
    Stb
    FOUND_VAR Stb_FOUND
    REQUIRED_VARS Stb_INCLUDE_DIR)

if(Stb_FOUND)
    message(STATUS "Stb found in: ${Stb_INCLUDE_DIR}")

    # Classic Find*.cmake way
    set(Stb_INCLUDE_DIRS ${Stb_INCLUDE_DIR})

    # Newer *Config.cmake way
    add_library(Stb::Stb INTERFACE IMPORTED)
    target_include_directories(Stb::Stb INTERFACE ${Stb_INCLUDE_DIR})

    mark_as_advanced(Stb_INCLUDE_DIR)
endif()
