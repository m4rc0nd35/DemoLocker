import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtSingleton 1.12

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Tabs")

    Component.onCompleted: {
        Sinstance.callme.connect(onCallme);
        console.log(Sinstance.getInfo());
    }

    Sinstance {
        id: singleton
    }

    function onCallme(){
        console.log("SLOT: onCallme")
    }

    Rectangle {
        anchors.fill: parent

        GridLayout {
            id: grid
            anchors.centerIn: parent
            columns: 2
            columnSpacing: 30

            Repeater {
                model: 2

                Rectangle {
                    width: 500
                    height: 900
                    color: "#AFCCCC"
                    border.color: "black"
                    border.width: .5
                    radius: 10

                    GridLayout {
                        anchors.centerIn: parent
                        columns: 2
                        rows: 8
                        columnSpacing: 15

                        Repeater {
                            model: 16

                            Rectangle {
                                height: 100
                                width: 220
                                color: "#6140E8"
                                border.color: "black"
                                border.width: .5
                                radius: 3
                                Text {
                                    id: name
                                    anchors.centerIn: parent
                                    text: qsTr("Box " + index)
                                    color: "white"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
