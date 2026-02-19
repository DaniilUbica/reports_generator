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
    title: qsTr("Reports generator")
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
                map.zoomToMyLocation(5.0, true)
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
                map.zoomToMyLocation(value, false)
            }
        }

        RowLayout {
            spacing: QMConst.spacing
            Layout.fillWidth: true
            Layout.bottomMargin: QMConst.margin
            Layout.leftMargin: QMConst.margin
            Layout.rightMargin: QMConst.margin

            QMTextField {
                id: addressInput
                Layout.preferredHeight: QMConst.controlHeightSmall
                Layout.fillWidth: true
                Layout.leftMargin: QMConst.marginLarge
                Layout.rightMargin: QMConst.marginLarge

                placeholderText: qsTr("Enter address")
            }

            QMButton {
                text: qsTr("Confirm")
                Layout.preferredHeight: QMConst.controlHeightSmall

                onClicked: {
                    addressInputManager.addressEntered(addressInput.text)
                    addressInput.text = ""
                }
            }
        }
    }

    Connections {
        target: rgApplication

        // void locationRequestCompleted(const QString& address, const QPointF& location);
        function onLocationRequestCompleted(address, location) {
            map.zoomToPoint(location.x, location.y, 20.0, true)
        }
    }
}
