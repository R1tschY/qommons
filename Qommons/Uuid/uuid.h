#pragma once

#include <QObject>

namespace Qommons {

class UuidFactory : public QObject {
    Q_OBJECT
public:
    UuidFactory(QObject* parent);
    ~UuidFactory() override;

    Q_INVOKABLE QString createUuid() const;
};

}
