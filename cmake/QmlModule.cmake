function(qommons_qml_module)
    set(options)
    set(oneValueArgs PLUGIN TEST_DIR QML_IMPORT_PATH QML_INSTALL_DIR URI)
    set(multiValueArgs SOURCES TEST_SOURCES)
    cmake_parse_arguments(qml_plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (qml_plugin_PLUGIN)
        add_library(${qml_plugin_PLUGIN} MODULE ${qml_plugin_SOURCES} qmldir)
        target_link_libraries(${qml_plugin_PLUGIN} PRIVATE Qt5::Core Qt5::Qml)
        target_compile_definitions(${qml_plugin_PLUGIN}
            PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>
        )
    endif()

    # test
    if ((qml_plugin_TEST_SOURCES) AND (qml_plugin_PLUGIN))
        add_definitions(-DQUICK_TEST_SOURCE_DIR="${qml_plugin_TEST_DIR}")

        add_executable(${qml_plugin_PLUGIN}-tests ${qml_plugin_TEST_SOURCES})
        add_test(NAME ${qml_plugin_PLUGIN}-tests COMMAND ${qml_plugin_PLUGIN}-tests
            -import "${QML_IMPORT_PATH}"
            -plugins "${CMAKE_CURRENT_BINARY_DIR}"
        )

        target_link_libraries(${qml_plugin_PLUGIN}-tests
            PRIVATE Qt5::QuickTest Qt5::Qml)
    endif()

    # install
    string(REPLACE "\\." "/" uri_as_path "${qml_plugin_URI}")
    set(INSTALL_DIR "${qml_plugin_QML_INSTALL_DIR}/${uri_as_path}")
    if (qml_plugin_PLUGIN)
        install(TARGETS ${qml_plugin_PLUGIN} DESTINATION ${INSTALL_DIR})
    endif()
    install(FILES qmldir DESTINATION ${INSTALL_DIR})
endfunction()
