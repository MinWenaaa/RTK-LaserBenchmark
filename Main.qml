import QtQuick
import QtQuick.Controls

Window {
    width: 720
    height: 1080
    visible: true
    title: qsTr("Hello World")

    Column{
        TextField {
            id: ipAddressInput
            width: 540; height: 60
            placeholderText: "ip address"
        }

        TextField {
            id: portInput
            width: 540; height: 60
            placeholderText: "port"
        }

        Button {
            id: connectButton
            text:  "connect"
            background: Rectangle {
                width: 540; height: 60
                color: "gray"
            }
            onClicked: {
                var ipAddress = ipAddressInput.text
                var port = parseInt(portInput.text)
                client.connectToServer(ipAddress, port)
            }
        }

        Connections {
            target: client
            onConnectSucessed: {
                connectButton.background.color = "green"
            }
            onConnectError: {
                connectButton.background.color = "red"
            }
            onStartConnection: {
                connectButton.background.color = "blue"
            }
        }
    }
}
