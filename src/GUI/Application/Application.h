#pragma once

#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlContext>
#include <QPointer>

#include "Core/ConsoleApplication/ConsoleApplication.h"

class AddressInputManager;

namespace rg {

class Application final : public QGuiApplication, public ConsoleApplication {
    Q_OBJECT
public:
    Application(int& argc, char** argv, int flags = ApplicationFlags);
    ~Application();

    void setupApplication(QQmlContext* context);

    void onTargetLocationReached() override;
    void onTargetLocationAbandoned() override;

    Q_SIGNAL void locationRequestCompleted(const QString& address, const QPointF& location);

private:
    QPointer<AddressInputManager> m_addressInputManager;
};

}
