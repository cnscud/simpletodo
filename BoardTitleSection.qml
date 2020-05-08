import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1

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
    color: "#f5e50a"
    //opacity: 0.766

    //父窗口的ListView
    property ListView pListView

    property string title;



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
        }
    }
    
    Button {
        id: closeButton
        
        width: 25
        height: parent.height
        text: "X"


        anchors.left: parent.left
        anchors.leftMargin: 0
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
    
    //Todo 可修改
    Label {
        id: boardTitleLbl
        //从model中读取 Todo
        text: title
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

        anchors.left: closeButton.right
        anchors.leftMargin: 20
    }
    
    Button {
        id: newStrikeButton
        
        width: 25
        height: parent.height
        text: "+"
        anchors.right: parent.right
        anchors.rightMargin: 0
        
        flat: true
        //font.bold: true
        font.pointSize: 22

        onClicked: {
            //TODO 新增一个任务
            //messageDialog.text = "I will create a new Strike";
            //messageDialog.open();
            
            var c = pListView.count
            //插入在第一个
            pListView.model.model.insert(0, {
                                            "description": "Buy a new book " + (c + 1),
                                            "done": false
                                        })
            
            //设置焦点, 否则listView就没焦点了
            pListView.focus = true;
            pListView.currentIndex = 0;
            
        }
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
