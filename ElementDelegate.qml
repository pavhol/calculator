import QtQuick
import QtQuick.Controls

RoundButton {
    property int cellWidth: GridView.view.cellWidth
    property int cellHeight: GridView.view.cellHeight
    property double startTime: 0
    property double stopTime: 0
    width: cellWidth-10
    height: cellHeight-10
    radius: 100
    background: Rectangle {
        id: back
        width: cellWidth-10
        height: cellHeight-10
        radius: 100
        color: co
    }
    onPressed: {
        back.color = "#F7E425";
        if (txt === "=") {
            secretflag = true;
            startTime = new Date().getTime();
        }
    }
    onReleased: {
        back.color = co;
        if (secretflag) {
            stopTime = new Date().getTime();
            if (stopTime - startTime < 4000) {
                secretflag = false;
            }
        }
    }
    onClicked: {
        if (txt === "=") {
            if (expression.text !== "")
            result.text = expression.text + "="+ calculator.calculate(qsTr(expression.text));
            expression.text = "";
        }
        else if (txt === "C") {
            expression.text="";
            result.text = "";
        }
        else if (secretflag && txt === "3" && expression.text === "12" && new Date().getTime()-stopTime < 5000) {
            secretflag = false;
            var component = Qt.createComponent("secret.qml")
            var window = component.createObject()
            window.show()
        }
        else {
            expression.text += txt;
        }
    }

    Text {
        anchors.centerIn: parent
        text: txt
        font.family: "Open Sans Semibold"
        font.pixelSize: cellHeight/2
        color: txtcolor
    }
}

