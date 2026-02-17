import QtQuick 2.15
import QtQuick.Window 2.15

import reports.generator.types 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Map")

    MapView {
        id: map
        anchors.fill: parent
        latitude: 55.7558
        longitude: 37.6176

        MouseArea {
            anchors.fill: parent

            onClicked: {
                map.clicked()
            }
        }
    }
}
