#include "Uuid.h"

#include <QUuid>

namespace Qommons {

UuidFactory::UuidFactory(QObject *parent)
    : QObject(parent)
{ }

UuidFactory::~UuidFactory() = default;

QString Qommons::UuidFactory::createUuid() const
{
    return QUuid::createUuid().toString();
}

}

#include "Uuid.moc"
