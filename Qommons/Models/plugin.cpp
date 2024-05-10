#include "plugin.h"

#include <QtQml>

#include "sortfilterproxymodel.h"

namespace Qommons {

void ModelsModulePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Qommons.Models"));

    qmlRegisterType<SortFilterProxyModel>("Qommons.Models", 0, 1, "SortFilterProxyModel");

    qRegisterMetaType<ModelFilter*>();
    qmlRegisterUncreatableType<ModelFilter>("Qommons.Models", 0, 1, "ModelFilter", "abstract type");
    qmlRegisterType<EqualsFilter>("Qommons.Models", 0, 1, "EqualsFilter");
    qmlRegisterType<AllOfFilter>("Qommons.Models", 0, 1, "AllOfFilter");
    qmlRegisterType<AnyOfFilter>("Qommons.Models", 0, 1, "AnyOfFilter");
    qmlRegisterType<NoneOfFilter>("Qommons.Models", 0, 1, "NoneOfFilter");
    qmlRegisterType<OneOfFilter>("Qommons.Models", 0, 1, "OneOfFilter");
}

}
