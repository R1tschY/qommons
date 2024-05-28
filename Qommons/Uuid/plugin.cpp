#include "plugin.h"

#include <QtQml>

#include "uuid.h"

namespace Qommons {

static QObject* createUuidFactory(QQmlEngine* qml, QJSEngine* js) {
    return new UuidFactory(qml);
}

void UuidModulePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Qommons.Uuid"));

    qmlRegisterSingletonType<UuidFactory>(
                "Qommons.Uuid", 0, 1, "Uuid", createUuidFactory);
}

}

#include "moc_plugin.cpp"