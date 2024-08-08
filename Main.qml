import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import calculator

Window {
    id: rootFrame
    width: 320
    height: 480
    visible: true
    title: qsTr("Calculator")
    color: "#024873"
    property bool secretflag: false
    minimumHeight: 240
    minimumWidth: 200

    Rectangle {
        id: exprstr
        color: "#04BFAD"
        height: 30
        width: 320
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Calculator {
        id: calculator
    }

    Rectangle {
        id: resultstr
        height: 120
        width: parent.width
        anchors {
            left: parent.left
            rightMargin: 10
        }
        color: "#04BFAD"
        radius: 10

        Column {
            width: parent.width
            height: parent.height

            Flickable {
                id: expressionFlickable
                width: parent.width
                height: 60
                contentWidth: expression.width
                interactive: true
                Label {
                    id: expression
                    text: ""
                    topPadding: 16
                    leftPadding: 15
                    horizontalAlignment: Text.AlignRight
                    color: "#FFFFFF"
                    wrapMode: Text.NoWrap
                    font.pixelSize: 24
                    font.family: "Open Sans Semibold"
                }
            }

            Flickable {
                id: resultFlickable
                width: parent.width
                height: 60
                contentWidth: result.width
                interactive: true

                Label {
                    leftPadding: 15
                    id: result
                    text: ""
                    color: "#FFFFFF"
                    wrapMode: Text.NoWrap
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 32
                    font.family: "Open Sans Semibold"
                }
            }
        }
    }

    GridView {
        id: keyboard
        anchors {
            top: resultstr.bottom
            left: parent.left
            right: parent.right
        }
        interactive: false
        topMargin: 15
        leftMargin: 6
        width: parent.width
        height: parent.height - exprstr.height - resultstr.height
        cellWidth: parent.width / 4 - 1.5
        cellHeight: (parent.height - exprstr.height - resultstr.height) / 5
        model: ElementsModel { }
        delegate: ElementDelegate { }
    }
}
