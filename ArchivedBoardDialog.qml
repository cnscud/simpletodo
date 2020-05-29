import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5
import QtQuick 2.2


Rectangle {
    id: root
    width: 500
    height: 400
    border.width: 1
    border.color: "black"



    Rectangle {
        id: rectangle
        height: 30
        color: "#c8c4c4"
        //width: parent.width-2
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 1


        MouseArea {
            id: mouseMoveWindowArea
            //height: 20
            anchors.fill: parent

            property point clickPos: "0,0"

            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
                //isMoveWindow = true
            }
            onReleased: {
                //isMoveWindow = false
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)

                //如果mainwindow继承自QWidget,用setPos
                window.setX(window.x + delta.x)
                window.setY(window.y + delta.y)
            }
        }


        Text {
            id: element
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            text: "查看归档: " +  board.title
            anchors.horizontalCenter: parent.horizontalCenter            
            font.pixelSize: 18
        }

        Button {
            id: closeBtn

            width: 25
            height: parent.height
            text: "X"

            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            flat: true
            //font.bold: true
            font.pointSize: 22

            onClicked: {
                window.close();
            }
        }

        Frame {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }

    ListView {
        id: listView
        //width: parent.width -2
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.top: rectangle.bottom
        anchors.bottom: bottomRectangle.top

        delegate: Rectangle {
            height: 35
            width: parent.width


                TextInput {
                    id: descLbl
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: listView.width - updatedLabel.width
                    text: model.desc
                    verticalAlignment: Text.AlignVCenter
                    //elide: Text.ElideRight
                    font.pointSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.bottom: parent.bottom
                    selectByMouse: true
                    readOnly: true
                    clip: true

                }
                Text {
                    id: updatedLbl
                    height: parent.height
                    width: 120
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    text: model.created.toLocaleString(Qt.Locale, "yyyy-MM-dd hh:mm")
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.bottom: parent.bottom

                }

            Frame {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                //anchors.topMargin: 1

            }

        }

        model: strikeListModel

    }



    Rectangle {
        id: bottomRectangle
        height: 20
        //width: parent.width-2
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        color: "#c8c4c4"

        Text {
            id: totalLabel
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            text: "共有 " + listView.count + " 个归档任务"
            font.pixelSize: 14
        }

        Text {
            id: updatedLabel
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.bottom: parent.bottom
            text: "更新时间 " + board.updated
            font.pixelSize: 14
        }


        MouseArea {
            id: resize
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            width: 15
            height: parent.height
            cursorShape: Qt.SizeFDiagCursor

            property point clickPos: "1,1"

            //保持窗口左上角不动
            property int oldX
            property int oldY

            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
                oldX = window.x
                oldY = window.y
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x,
                                     mouse.y - clickPos.y)

                var minWidth = 100;
                var minHeight = 100;

                //最小
                var newWidth = (window.width + delta.x)<minWidth?minWidth:(window.width + delta.x);

                //最小
                var newHeight = (window.height + delta.y)<minHeight?minHeight:(window.height + delta.y);

                window.width = newWidth;
                window.height = newHeight;
                window.x = oldX
                window.y = oldY

            }

            Rectangle {
                id: resizeHint
                color: "grey"
                anchors.fill: resize
            }
        }

    }











}
