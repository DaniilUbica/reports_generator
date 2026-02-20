#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>

#include "GUI/Application/Application.h"

int main(int argc, char *argv[]) {
    QScopedPointer<rg::Application> app(new rg::Application(argc, argv));
    QQmlApplicationEngine engine;

    app->setupApplication(engine.rootContext());
    app->setQuitOnLastWindowClosed(false);

    engine.load(QUrl("qrc:/main.qml"));

    return app->exec();
}
