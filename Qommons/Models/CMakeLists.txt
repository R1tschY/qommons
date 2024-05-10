qommons_qml_plugin(
    NAME qommons-models
    URI "Qommons.Models"
    QML_IMPORT_PATH "${QOMMONS_IMPORT_PATH}"
    QML_INSTALL_DIR "${QOMMONS_QML_INSTALL_PATH}"

    SOURCES
    qmldir
    plugin.h
    plugin.cpp
    sortfilterproxymodel.h
    sortfilterproxymodel.cpp

    TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests"
    TEST_SOURCES
    tests/tests.cpp
    tests/tst_modelstest.qml
)
