import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import Qt.labs.platform 1.1
import QtQuick.Dialogs 1.1


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
                text: "新建白板"
                onTriggered: {
                    windowModel.addBoard();
                }
            }

            Menu {
                id: boardListMenu
                title: "白板列表"

                //白板列表: 显示/隐藏
                Instantiator {
                    id: menuInstantiator
                    model: windowModel
                    MenuItem {
                        text: model.title
                        checked: !model.hidden
                        onTriggered: {
                            model.hidden = !model.hidden
                        }
                    }

                    //创建后不会自动添加到菜单项里, 所以需要
                    onObjectAdded: boardListMenu.insertItem(index, object)
                    onObjectRemoved: boardListMenu.removeItem(object)
                }
            }

            MenuSeparator {}

            MenuItem {
                text: "关于"
                //role: MenuItem.AboutRole  //这样会移动到系统菜单栏
                onTriggered: {
                    aboutDialog.open();
                }
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("退出")
                //role: MenuItem.QuitRole
                onTriggered: Qt.quit()
            }

        }
    }

    MessageDialog {
        id: aboutDialog
        title: "关于"
        text: "一个简单的任务管理白板"
        icon: StandardIcon.Information
    }


}
