enable_testing()

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
  PRIVATE ${SDL2_INCLUDE_DIR} ${Boost_INCLUDE_DIR}
)

target_link_libraries(blowthemall ${SDL2_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
  ${Boost_COROUTINE_LIBRARY} ${Boost_CONTEXT_LIBRARY})

# Install info
install(TARGETS blowthemall
    DESTINATION bin
    COMPONENT runtime
)

macro(add_test_target target source)
  add_executable("blowthemall-test-${target}" ${source})

  set_target_properties(
    "blowthemall-test-${target}"
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    )

  target_include_directories("blowthemall-test-${target}"
    PRIVATE ${SDL2_INCLUDE_DIR} ${Boost_INCLUDE_DIR}
    )

  target_link_libraries("blowthemall-test-${target}"
    ${SDL2_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_COROUTINE_LIBRARY}
    ${Boost_CONTEXT_LIBRARY})

  target_compile_definitions("blowthemall-test-${target}"
    PRIVATE BOOST_TEST_DYN_LINK
    )

  add_test(NAME "blowthemall-test-${target}" COMMAND $<TARGET_FILE:blowthemall-test-${target}>)
endmacro()

add_test_target(gamemap "gamemap.cpp;gamemap-test.cpp")
