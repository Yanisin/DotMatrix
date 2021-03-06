import QtQuick 2.0
import QtQuick.Shapes 1.12
import QtQuick 2.12

Shape {
    width: 10
    height: 10
    property color ledColor: "red"

    ShapePath {
        fillColor: ledColor
        strokeWidth: -1
        PathAngleArc {
            startAngle: 0
            sweepAngle: 360
            centerX: 3
            centerY: 3
            radiusX: 3
            radiusY: 3
        }
    }
}
