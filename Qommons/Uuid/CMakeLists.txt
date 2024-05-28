qommons_qml_module(
    PLUGIN qommons-uuid-qt5
    URI "Qommons.Uuid"
    QML_IMPORT_PATH "${QOMMONS_IMPORT_PATH}"
    QML_INSTALL_DIR "${QOMMONS_QML_INSTALL_PATH}"

    SOURCES
    qmldir
    plugin.h
    plugin.cpp
    uuid.h
    uuid.cpp
)
