CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # feel free to try lower
PROJECT(topologist)

SET(common_files
    src/hash_tables.c

    src/error.c
    src/query.c
    src/build.c
    src/topology.c
    src/infer/common.c
    src/infer/utils.c
    src/infer/roles.c

    src/infer/bus.c
    src/infer/bidi.c
    src/infer/extern.c

    src/mpack.c
    )

ADD_EXECUTABLE(topologist
    ${common_files}
    src/main.c
    config.c)
ADD_EXECUTABLE(topology
    ${common_files}
    src/info.c
    src/munpack.c
    config.c)

ADD_SUBDIRECTORY(quire)
GET_TARGET_PROPERTY(QUIRE_GEN quire-gen LOCATION)
ADD_CUSTOM_COMMAND(
    OUTPUT config.h config.c
    COMMAND ${QUIRE_GEN}
        --source ${CMAKE_CURRENT_SOURCE_DIR}/src/config.yaml
        --c-header config.h
        --c-source config.c
    DEPENDS src/config.yaml quire-gen
    )
SET_SOURCE_FILES_PROPERTIES(
    config.h config.c
    PROPERTIES GENERATED 1
    )
TARGET_LINK_LIBRARIES(topologist quire)
INCLUDE_DIRECTORIES(BEFORE SYSTEM quire/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET_TARGET_PROPERTIES(topology PROPERTIES
    COMPILE_FLAGS "-DCOMMAND_LINE")
TARGET_LINK_LIBRARIES(topology quire)

IF(ASTYLE)
    ADD_CUSTOM_COMMAND(
        TARGET topologist
        PRE_BUILD
        COMMAND ${ASTYLE} -Y config.c
        DEPENDS config.c
        )
    ADD_CUSTOM_COMMAND(
        TARGET topologist
        PRE_BUILD
        COMMAND ${ASTYLE} -Y config.h
        DEPENDS config.h
        )
ENDIF()

install (TARGETS topologist topology
    RUNTIME DESTINATION bin)
install (FILES examples/default.yaml
    RENAME topologist.yaml
    DESTINATION /etc)

GET_TARGET_PROPERTY(TOPOLOGY topology LOCATION)

ENABLE_TESTING()
FIND_FILE(DIFF diff)
ADD_SUBDIRECTORY(tests/simple)
