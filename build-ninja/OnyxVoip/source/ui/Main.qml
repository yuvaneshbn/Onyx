import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 780
    visible: true
    title: "Onyx-voip"
    color: "#081018"

    property string targetUri: "sip:127.0.0.1"
    property string targetIp: "127.0.0.1"
    property string filePath: ""

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#081018" }
            GradientStop { position: 0.6; color: "#0f1725" }
            GradientStop { position: 1.0; color: "#1a2232" }
        }
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 24
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 18

            Frame {
                Layout.fillWidth: true
                padding: 20
                background: Rectangle {
                    radius: 18
                    color: "#121a28"
                    border.color: "#263247"
                }

                ColumnLayout {
                    width: parent.width
                    spacing: 10

                    Label {
                        text: "Onyx-voip"
                        font.pixelSize: 34
                        font.bold: true
                        color: "#f4f7fb"
                    }

                    Label {
                        text: "Decentralized LAN voice, discovery, and file transfer"
                        color: "#9fb0c8"
                        font.pixelSize: 14
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 24

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            Label { text: "Engine status"; color: "#8fa3bf"; font.pixelSize: 12 }
                            Label { text: sipController.regStatus; color: "#ffffff"; font.pixelSize: 18 }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            Label { text: "Discovery"; color: "#8fa3bf"; font.pixelSize: 12 }
                            Label { text: "LAN beacon active on UDP multicast"; color: "#ffffff"; font.pixelSize: 18 }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            Label { text: "Transfer"; color: "#8fa3bf"; font.pixelSize: 12 }
                            Label {
                                text: fileTransfer.isTransferring ? "Transferring" : "Idle"
                                color: fileTransfer.isTransferring ? "#8ee28f" : "#ffffff"
                                font.pixelSize: 18
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 18

                Frame {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    padding: 18
                    background: Rectangle {
                        radius: 18
                        color: "#111a27"
                        border.color: "#263247"
                    }

                    ColumnLayout {
                        width: parent.width
                        spacing: 12

                        Label {
                            text: "Call control"
                            color: "#f4f7fb"
                            font.pixelSize: 20
                            font.bold: true
                        }

                        TextField {
                            id: uriField
                            Layout.fillWidth: true
                            text: window.targetUri
                            placeholderText: "sip:peer@192.168.1.25"
                            onTextChanged: window.targetUri = text
                        }

                        Button {
                            text: "Place Call"
                            Layout.fillWidth: true
                            onClicked: sipController.initiateCall(uriField.text)
                        }

                        Button {
                            text: "Hang Up All"
                            Layout.fillWidth: true
                            onClicked: sipController.hangUpCall(-1)
                        }

                        TextArea {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 84
                            readOnly: true
                            wrapMode: TextArea.Wrap
                            text: "Use this panel to initiate calls against a SIP URI when PJSIP is enabled. Without pjproject .lib files, the shell runs in LAN-only mode."
                        }
                    }
                }

                Frame {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    padding: 18
                    background: Rectangle {
                        radius: 18
                        color: "#111a27"
                        border.color: "#263247"
                    }

                    ColumnLayout {
                        width: parent.width
                        spacing: 12

                        Label {
                            text: "Peer directory"
                            color: "#f4f7fb"
                            font.pixelSize: 20
                            font.bold: true
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 260
                            clip: true
                            model: contactModel

                            delegate: Rectangle {
                                width: ListView.view.width
                                height: 48
                                radius: 12
                                color: index % 2 === 0 ? "#162131" : "#101926"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    spacing: 12

                                    Rectangle {
                                        width: 10
                                        height: 10
                                        radius: 5
                                        color: isOnline ? "#5fd18c" : "#76839a"
                                    }

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 2
                                        Label { text: name; color: "#ffffff"; font.pixelSize: 14 }
                                        Label { text: sipUri; color: "#98a8bf"; font.pixelSize: 12 }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Frame {
                Layout.fillWidth: true
                padding: 18
                background: Rectangle {
                    radius: 18
                    color: "#111a27"
                    border.color: "#263247"
                }

                ColumnLayout {
                    width: parent.width
                    spacing: 10

                    Label {
                        text: "Direct file transfer"
                        color: "#f4f7fb"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        TextField {
                            id: ipField
                            Layout.fillWidth: true
                            text: window.targetIp
                            placeholderText: "Target IP"
                            onTextChanged: window.targetIp = text
                        }

                        TextField {
                            id: pathField
                            Layout.fillWidth: true
                            text: window.filePath
                            placeholderText: "File path to send"
                            onTextChanged: window.filePath = text
                        }

                        Button {
                            text: "Send"
                            enabled: ipField.text.length > 0 && pathField.text.length > 0 && !fileTransfer.isTransferring
                            onClicked: fileTransfer.sendFile(ipField.text, pathField.text)
                        }
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: fileTransfer.progress
                    }
                }
            }
        }
    }
}
