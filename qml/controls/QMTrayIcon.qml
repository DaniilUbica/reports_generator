import QtQuick
import QtQuick.Window
import Qt.labs.platform

import "../style"

SystemTrayIcon {
    id: root
    visible: mainWindow ? !mainWindow.visible : true
    icon.source: "qrc:/location.png"

    property Window mainWindow: null

    menu: Menu {
        MenuItem {
            text: qsTr("Open window")
            onTriggered: {
                mainWindow.show()
                mainWindow.raise()
                mainWindow.requestActivate()
            }
        }

        MenuItem {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
    }
}
