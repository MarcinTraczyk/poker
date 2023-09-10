cmake_minimum_required(VERSION 3.16.3)

project(poker_solver)
set(
   POKER_INSTALL_LIBDIR "/usr/local/lib/poker"
   CACHE STRING "Installation directory for shared objects")

add_library(poker SHARED
   poker.cpp
   math.cpp
   hand.cpp
   full_hand.cpp
   table.cpp
   table_results.cpp
   range.cpp
)
list(APPEND POKER_INSTALL_TARGETS poker)

install(TARGETS ${POKER_INSTALL_TARGETS}
   LIBRARY DESTINATION "${POKER_INSTALL_LIBDIR}"
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)