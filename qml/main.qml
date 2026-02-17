import QtQuick
import QtQuick.Window
import QtQuick.Layouts

import "controls"
import "style"

import reports.generator.types 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Map")
    color: QMColor.windowBackgroundColor

    ColumnLayout {
        anchors.fill: parent
        spacing: QMConst.spacing

        MapView {
            id: map
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        QMButton {
            text: qsTr("Zoom to my position")
            Layout.preferredHeight: QMConst.controlHeight
            Layout.fillWidth: true
            Layout.leftMargin: QMConst.margin
            Layout.rightMargin: QMConst.margin

            onClicked: {
                map.zoomToMyPosition()
            }
        }

        QMSlider {
            from: map.minimumZoomLevel
            to: map.maximumZoomLevel
            Layout.preferredHeight: QMConst.controlHeightSmall
            Layout.fillWidth: true
            Layout.leftMargin: QMConst.marginLarge
            Layout.rightMargin: QMConst.marginLarge

            onValueChanged: {
                map.zoomToMyPosition(value)
            }
        }
    }
}
