#pragma once

#include <QGuiApplication>
#include <QQmlContext>
#include <QPointer>

class LocationManagerBase;
class AddressInputManager;

namespace rg {

class Application final : public QGuiApplication {
    Q_OBJECT
public:
    Application(int& argc, char** argv, int flags = ApplicationFlags);
    ~Application();

    void setupApplication(QQmlContext* context);

    Q_SIGNAL void locationRequestCompleted(const QString& address, const QPointF& location);

private:
    std::shared_ptr<LocationManagerBase> m_locationManager;
    QPointer<AddressInputManager> m_addressInputManager;
};

}
