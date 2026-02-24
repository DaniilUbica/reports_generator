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
    void onLocationFromAddressReceived(const std::string& address, const std::optional<location::Point>& location) override;

    void startTrackingAddress(const std::string& address) override;

    Q_PROPERTY(QString trackingAddress READ trackingAddress WRITE setTrackingAddress NOTIFY trackingAddressChanged)
    QString trackingAddress() const { return QString::fromStdString(m_trackingAddress); };
    void setTrackingAddress(const QString& address);
    Q_SIGNAL void trackingAddressChanged(const QString& address);

    Q_SIGNAL void locationRequestCompleted(const QString& address, const QPointF& location);

private:
    QPointer<AddressInputManager> m_addressInputManager;
};

}
