#include "plugin.h"

#include <QtQml>

#include "process.h"

namespace Qommons {

void ProcessModulePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Qommons.Process"));

    qmlRegisterType<Process>(
                "Qommons.Process", 0, 1, "Process");

    qmlRegisterType<ProcessOutputModel>(
                "Qommons.Process", 0, 1, "ProcessOutputModel");
}

}

#include "moc_plugin.cpp"
