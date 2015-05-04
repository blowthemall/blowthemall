# Source files
set(blowthemall_SRC
    main.cpp
    blowthemallcontroller.cpp
    rpcnode.cpp
    matchserver.cpp
    matchserverbroadcast.cpp
    matchserverplayer.cpp
    matchservermatchsetupstate.cpp
    matchservermatchstate.cpp
    mapmodel.cpp
    matchclient.cpp
    matchclientidentificationstate.cpp
    matchclientmatchsetupstate.cpp
    matchclientmatchstate.cpp
    gamescene.cpp
    btaitem.cpp
    "${PROJECT_SOURCE_DIR}/data/resources.qrc"
    # Trigger recompilation of Qt resources if modified
    # And also serves as hints to IDEs
    "${PROJECT_SOURCE_DIR}/data/ui/main.qml"
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
    AUTOMOC ON
    AUTORCC ON
    AUTORCC_OPTIONS --no-compress
    VERSION ${BLOWTHEMALL_VERSION}
)

qt5_use_modules(blowthemall Core QuickWidgets Widgets Network)
target_link_libraries(blowthemall ${TUFAO_LIBRARIES})

# Install info
install(TARGETS blowthemall
    DESTINATION bin
    COMPONENT runtime
)
