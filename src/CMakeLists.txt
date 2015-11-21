# Source files
set(blowthemall_SRC
    main.cpp
    gameloop.cpp
)

add_definitions(
    "-DBLOWTHEMALL_VERSION=\"${BLOWTHEMALL_VERSION}\""
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(blowthemall ${blowthemall_SRC})

set_target_properties(
    blowthemall
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    VERSION ${BLOWTHEMALL_VERSION}
)

target_include_directories(blowthemall
  PRIVATE ${SDL2_INCLUDE_DIR}
)

target_link_libraries(blowthemall ${SDL2_LIBRARY})

# Install info
install(TARGETS blowthemall
    DESTINATION bin
    COMPONENT runtime
)
