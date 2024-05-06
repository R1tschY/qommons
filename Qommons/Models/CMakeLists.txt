add_library(
    qommons-models MODULE
    qmldir
    plugin.h
    plugin.cpp
    sortfilterproxymodel.h
    sortfilterproxymodel.cpp
)
target_link_libraries(qommons-models PRIVATE Qt5::Core Qt5::Qml)

target_compile_definitions(qommons-models
    PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>
)

#target_include_directories(qommons-models PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# test
add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(qommons-models-test test.cpp tst_modelstest.qml)
add_test(NAME qommons-models-test COMMAND qommons-models-test
    -import "${QOMMONS_IMPORT_PATH}"
    -plugins "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(qommons-models-test
    PRIVATE Qt5::QuickTest Qt5::Qml)

# install
set(INSTALL_DIR "${QOMMONS_QML_INSTALL_PATH}/Qommons/Models")
install(TARGETS qommons-models DESTINATION ${INSTALL_DIR})
install(FILES qmldir DESTINATION ${INSTALL_DIR})
