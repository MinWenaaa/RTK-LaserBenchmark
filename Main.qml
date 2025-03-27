import QtQuick
import QtQuick.Controls

Window {
    width: 720
    height: 1080
    visible: true
    title: qsTr("Hello World")

    Column {
        width: 640
        height: 540

        TextInput{
            width: 320; height: 60
            id: ipAdressInput; text: "IP Adress"
        }
        TextInput{
            width: 320; height: 60
            id: portInput; text: "Port"}
        Button{
            property bool sucessed: false
            width: 320; height: 60
            background: Rectangle {
                   color: parent.sucessed ? "red" : "blue"
               }
            text: "Connect"
            onClicked: {
                var ipAddress = ipAdressInput.text.trim()
                var port = parseInt(portInput.text.trim())
                sucessed = client.connectToServer(ipAddress, port)
            }
        }
    }
}
