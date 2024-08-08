import QtQuick
import QtQuick.Controls.Basic
Window {
    id: root
    width: 320
    height: 480
    visible: true
    Text {
        id: contentstr
        text: qsTr("SECRET MENU")
        font.pixelSize: 32
        font.family: "Sans Semibold"
    }
    Button {
        Text {
            text: qsTr("back")
        }
        anchors.top: contentstr.bottom
        onClicked: {
            root.hide();
        }
    }
}
