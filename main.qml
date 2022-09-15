import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import org.raymii.ctrl 1.0

Window {
    width: 640
    height: 700
    visible: true
    title: qsTr("QThread Playground")

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Button {
            width: 400
            height: 60
            text: "Start long-running work"
            anchors.left: parent.left
            onClicked: Controller.makeThread()
        }

        Button {
            width: 400
            height: 60
            text: "Click me to see if UI thread is blocked"
            anchors.left: parent.left
            onClicked: uiTester.toggle = !uiTester.toggle
        }

        ScrollView {
            width: 600
            height: 300
            anchors.left: parent.left

            TextArea {
                id: logText
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text: Controller.result
            }
        }

        Rectangle {
            id: uiTester
            property bool toggle: false
            width: 400
            height: 60
            anchors.left: parent.left
            border.color: toggle ? "cyan" : "red"
            color: toggle ? "navy" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: uiTester.toggle ? "white" : "black"
                text: "Running: " + Controller.runningThreadCount
            }
        }

        Button {
            width: 400
            height: 60
            text: "Clear"
            anchors.left: parent.left
            onClicked: {
                Controller.result = ""
                console.log("")
            }
        }
    }
}
