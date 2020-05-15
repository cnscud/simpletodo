import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import Qt.labs.platform 1.1


Item {
    Instantiator {
        id: windowInstantiator

        model: windowModel
        /*
        model: ListModel {
            id: windowModel
            ListElement {
                title: "Initial Window"
                x: -200
            }
            ListElement {
                title: "Second Window"
                x: 300
            }
        }*/

        delegate: BoardWindow {
            id: window
        }


    }

    //增加到系统菜单栏
    SystemTrayIcon {
        visible: true
        icon.source: "media/todo.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }


}
