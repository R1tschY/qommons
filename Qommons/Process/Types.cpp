#include "Process.h"

#include <QtQml>

namespace Qommons {

void registerProcessTypes()
{
    qmlRegisterType<Process>(
                "Qommons.Process", 0, 1, "Process");

    qmlRegisterType<ProcessOutputModel>(
                "Qommons.Process", 0, 1, "ProcessOutputModel");
}

}
