#pragma once

#include <QObject>

class AddressInputManager : public QObject {
    Q_OBJECT
public:
    AddressInputManager(QObject* parent = nullptr);

    Q_SIGNAL void addressEntered(const QString& address);
};
