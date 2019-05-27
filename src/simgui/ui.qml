import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 640
    height: 640

    Repeater {
        model: field.qCells

        Rectangle {
            property var cell: modelData
            property var cellSize: 80
            property var pixelSize: 10
            color: "gray"
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

            /* Led diode */
            Shape {
                visible: cell.ledState
                ShapePath {
                    fillColor: "blue"
                    strokeWidth: -1

                    PathAngleArc {
                        startAngle: 0
                        sweepAngle: 360
                        centerX: 0
                        centerY: 0
                        radiusX: 5
                        radiusY: 5
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
