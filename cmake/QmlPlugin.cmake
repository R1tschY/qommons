function(qommons_qml_plugin)
    set(options)
    set(oneValueArgs NAME TEST_DIR QML_IMPORT_PATH QML_INSTALL_DIR URI)
    set(multiValueArgs SOURCES TEST_SOURCES)
    cmake_parse_arguments(qml_plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_library(${qml_plugin_NAME} MODULE ${qml_plugin_SOURCES} qmldir)
    target_link_libraries(${qml_plugin_NAME} PRIVATE Qt5::Core Qt5::Qml)
    target_compile_definitions(${qml_plugin_NAME}
        PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>
    )

    # test
    add_definitions(-DQUICK_TEST_SOURCE_DIR="${qml_plugin_TEST_DIR}")

    add_executable(${qml_plugin_NAME}-tests ${qml_plugin_TEST_SOURCES})
    add_test(NAME ${qml_plugin_NAME}-tests COMMAND ${qml_plugin_NAME}-tests
        -import "${QML_IMPORT_PATH}"
        -plugins "${CMAKE_CURRENT_BINARY_DIR}"
    )

    target_link_libraries(${qml_plugin_NAME}-tests
        PRIVATE Qt5::QuickTest Qt5::Qml)

    # install
    string(REPLACE "\\." "/" uri_as_path "${qml_plugin_URI}")
    set(INSTALL_DIR "${qml_plugin_QML_INSTALL_DIR}/${uri_as_path}")
    install(TARGETS ${qml_plugin_NAME} DESTINATION ${INSTALL_DIR})
    install(FILES qmldir DESTINATION ${INSTALL_DIR})
endfunction()
