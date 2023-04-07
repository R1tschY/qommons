#include "SortFilterProxyModel.h"

#include <QtQml>

namespace Qommons {

void registerModelsTypes()
{
    qmlRegisterType<SortFilterProxyModel>("Qommons.Models", 0, 1, "SortFilterProxyModel");

    qmlRegisterType<EqualsFilter>("Qommons.Models", 0, 1, "EqualsFilter");
    qmlRegisterType<AllOfFilter>("Qommons.Models", 0, 1, "AllOfFilter");
    qmlRegisterType<AnyOfFilter>("Qommons.Models", 0, 1, "AnyOfFilter");
    qmlRegisterType<NoneOfFilter>("Qommons.Models", 0, 1, "NoneOfFilter");
    qmlRegisterType<OneOfFilter>("Qommons.Models", 0, 1, "OneOfFilter");
}

}
