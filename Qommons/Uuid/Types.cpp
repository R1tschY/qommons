#include "Uuid.h"

#include <QtQml>

namespace Qommons {

static QObject* createUuidFactory(QQmlEngine* qml, QJSEngine* js) {
    return new UuidFactory(qml);
}

void registerUuidTypes()
{
    qmlRegisterSingletonType<UuidFactory>(
                "Qommons.Uuid", 0, 1, "Uuid", createUuidFactory);
}

}
