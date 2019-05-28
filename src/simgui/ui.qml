import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 640
    height: 640
    background: Rectangle {
        color: "white"
    }

    header: Row {
        Button {
            text: "(Re-)Start All"
            onClicked: field.restartAll()
        }
        Button {
            text: "Kill"
            onClicked: field.killAll()
        }
    }

    Repeater {
        model: field.qCells

        Rectangle {
            property var cell: modelData
            property var cellSize: 80
            property var pixelSize: 10
            color: "#cfcfcf"
            width: cellSize
            height: cellSize
            x: cell.x * cellSize
            y: cell.y * cellSize

            /* Pixels */
            Repeater {
                model: cell.display
                Rectangle {
                    property var pixel: modelData
                    color: Qt.hsva(0, 0, pixel.value/256, 1)
                    width: pixelSize
                    height: pixelSize
                    x: pixel.x * pixelSize
                    y: pixel.y * pixelSize
                }
            }

            Row {
                /* Connection indicator */
                Led {
                    ledColor: {
                        if (cell.connected) {
                            return "green";
                        } else if (cell.targetRunning) {
                            return "orange";
                        } else {
                            return "red";
                        }
                    }
                }
                /* Led diode */
                Led {
                    ledColor: {
                        return cell.ledState ? "lightblue" : "black";
                    }
                }
            }

            /* Mouse Over effect */
            Rectangle {
                visible: mouseArea.containsMouse
                color: "green"
                opacity: 0.5
                anchors.fill: parent
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
        }
    }
}
