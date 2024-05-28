qommons_qml_module(
    PLUGIN qommons-process-qt5
    URI "Qommons.Process"
    QML_IMPORT_PATH "${QOMMONS_IMPORT_PATH}"
    QML_INSTALL_DIR "${QOMMONS_QML_INSTALL_PATH}"

    SOURCES
    qmldir
    plugin.h
    plugin.cpp
    process.h
    process.cpp
)
