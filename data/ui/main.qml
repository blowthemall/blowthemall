import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true

    Image {
        anchors.fill: parent
        width: 640; height: 480
        fillMode: Image.PreserveAspectCrop
        source: "qrc:///gfx/loadscreen-final.png"
    }

    ColumnLayout {
        visible: app.state == "main"
        anchors.fill: parent

        ColumnLayout {
            anchors.centerIn: parent

            Text {
                text: "BlowThemAll"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: playButton
                text: qsTr("Play")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: app.state = "play"
            }

            Button {
                text: qsTr("Addons")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: qsTr("Options")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: qsTr("About")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: qsTr("Quit")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: Qt.quit();
            }
        }

        Text {
            objectName: "versionText"
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.right
            anchors.rightMargin: 5
            font.pixelSize: 12
        }
    }

    ColumnLayout {
        anchors.fill: parent
        visible: app.state == "play"

        ColumnLayout {
            anchors.centerIn: parent

            Button {
                text: "Serve game"
                onClicked: app.state = "servegame"
            }
            Button {
                text: "Join game"
                onClicked: app.state = "joingame"
            }
            Button {
                text: "Back"
                onClicked: app.state = "main"
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        visible: app.state == "servegame"

        ColumnLayout {
            anchors.centerIn: parent

            SpinBox {
                id: servingPortInput
                minimumValue: 1
                maximumValue: 65535
                value: 1
                prefix: "Port: "
            }

            RowLayout {
                Button {
                    text: "Back"
                    onClicked: app.state = "play"
                }
                Button {
                    text: "BlowThemAll"
                    onClicked: {
                        blowthemall.serveGame(servingPortInput.value)
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        visible: app.state == "joingame"

        ColumnLayout {
            anchors.centerIn: parent

            TextField {
                id: hostInput
                placeholderText: "Host"
            }

            SpinBox {
                id: portInput
                minimumValue: 1
                maximumValue: 65535
                value: 1
                prefix: "Port: "
            }

            RowLayout {
                Button {
                    text: "Back"
                    onClicked: app.state = "play"
                }

                Button {
                    text: "BlowThemAll"
                    onClicked: blowthemall.joinGame(hostInput.text,
                                                    portInput.value);
                }
            }
        }
    }

    Item {
        id: app
        state: "main"
    }

    Connections {
        target: blowthemall
        onMatchStarted: mainWindow.hide()
        onMatchStopped: mainWindow.show()
    }
}
