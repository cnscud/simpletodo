import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1

import StrikeTodo 1.0


//Todo: Model 用来设置关闭, 设置标题: 双向数据联动

/**
  白板的头部操作区域.
*/
Rectangle {
    id: titleOpRect
    x: 0
    y: 0
    width: parent.width
    height: 30

    //父窗口的ListView
    property ListView pListView
    property StrikeListModel listModel
    property string backColor: "#f5e50a"

    property string title : "Hello Board"
    property bool titleEditMode: false


    color: backColor
    opacity: 0.9

    MouseArea {
        id: mouseMoveWindowArea
        //height: 20
        anchors.fill: parent
        
        acceptedButtons: Qt.LeftButton
        
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

            //保存窗口位置
            model.windowX = window.x;
            model.windowY = window.y;
        }

        onDoubleClicked: {
            //进入编辑状态
            titleEditMode = true;
            //设置焦点
            titleTextinput.focus = true;
        }

    }
    
    Button {
        id: closeButton
        
        width: 25
        height: parent.height
        text: "X"

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0


        //opacity: 0.5
        flat: true
        font.bold: true
        font.pointSize: 14

        onClicked: {
            //TODO 设置Board的状态为隐藏: 应该提示用户
            //closeConfirmDialog.open();
            window.close()
        }
    }
    

    Rectangle {
        id: titleTextSection
        height: parent.height
        //color: "#f5e50a"
        visible: true
        opacity: 1

        anchors.bottom: parent.bottom

        anchors.left: closeButton.right
        anchors.leftMargin: 0
        anchors.right: newStrikeButton.left
        anchors.rightMargin: 0

        color: backColor



        Text {
            id: titleText

            text: title
            anchors.bottom: parent.bottom

            //控制可见: 是否编辑状态
            visible: !titleEditMode

            //底部留点空间
            bottomPadding: 5

        } //end titleText

        TextInput {
            id: titleTextinput

            anchors.bottom: parent.bottom

            color: "blue"
            font.bold: true

            //底部留点空间
            bottomPadding: 5

            text: title

            visible: titleEditMode

            //focus: true
            selectByMouse: true


            onEditingFinished: {

                model.title = titleTextinput.text
                model.updated = new Date();

                //恢复正常显示
                titleEditMode = false;

            }
        } //end titleTextinput

    } //end titleTextSection Rectangle

    
    Button {
        id: newStrikeButton
        
        width: 25
        height: parent.height
        text: "+"

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        flat: true
        //font.bold: true
        font.pointSize: 22

        onClicked: {
            //TODO 新增一个任务
            //messageDialog.text = "I will create a new Strike";
            //messageDialog.open();
            
            var c = pListView.count

            //这样调用: 是最佳方法吗?
            listModel.addStrike();

            //pListView.model.model.insert(0, {"description": "Buy a new book " + (c + 1),"done": false })
            
            //设置焦点, 否则listView就没焦点了
            pListView.focus = true;
            pListView.currentIndex = 0;
            
        }
    }

    Frame {
        width: titleOpRect.width
        height: 1
        anchors.bottom: titleOpRect.bottom
        Rectangle {
            height: parent.height
            width: parent.width
            color: "blue"
        }

        //anchors.topMargin: 1

    }

    
    
    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        title: "New Strike will added"
        text: "I will create a new Strike"
        
        onAccepted: {
        }
    }
    
    MessageDialog {
        id: closeConfirmDialog
        title: "关掉白板"
        icon: StandardIcon.Question
        text: "你要关闭这个白板吗? 你可以在菜单里重新打开"
        standardButtons: StandardButton.Yes |  StandardButton.No
        onYes: {
            window.close()
        }
    }
    
    
}
