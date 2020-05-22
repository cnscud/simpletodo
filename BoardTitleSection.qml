import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQml.Models 2.1
import QtQuick.Dialogs 1.2

import StrikeTodo 1.0


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
        
        //支持右键菜单: 编辑模式下不需要
        acceptedButtons: titleEditMode? Qt.LeftButton : ( Qt.LeftButton | Qt.RightButton)

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

        onClicked: {
            //右键菜单
            if (mouse.button === Qt.RightButton){
                titleContextMenu.popup()
            }
        }

    }

    CoodMenu {
          id: titleContextMenu
          itemWidth: 180
          //编辑状态则不需要右键菜单
          enabled: !titleEditMode

          Action { text: "删除此白板"
              onTriggered: {
                  //属性绑定, 不关闭会crash...
                  titleContextMenu.close();
                  //询问对话框
                  deleteConfirmDialog.open();
              }
          }

          Action { text: "背景色..."
              onTriggered: {
                  backColorDialog.open();
              }
          }

          Action { text: "字体..."
              onTriggered: {
                  fontDialog.open();
              }
          }

          CoodMenu {
              title: "字体大小"
              Action { text: "最大"
                  checked: model.fontSize ===30
                  onTriggered: {
                      model.fontSize = 30;
                  }
              }
              Action { text: "较大"
                  checked: model.fontSize ===24
                  onTriggered: {
                      model.fontSize = 24;
                  }
              }
              Action { text: "适中"
                  checked: model.fontSize ===18
                  onTriggered: {
                      model.fontSize = 18;
                  }
              }
              Action { text: "较小"
                  checked: model.fontSize ===14
                  onTriggered: {
                      model.fontSize = 14;
                  }
              }
              Action { text: "最小"
                  checked: model.fontSize ===9
                  onTriggered: {
                      model.fontSize = 9;
                  }
              }

          }

    }


    FontDialog {
        id: fontDialog
        title: "请选择一个字体"
        font: Qt.font({ family: "Arial", pointSize: model.fontSize, weight: Font.Normal })
        modality : Qt.WindowModal
        onAccepted: {
            console.log("You chose: " + fontDialog.font)
            model.fontFamily = fontDialog.font.family;
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    //背景色选择
    ColorDialog {
        id: backColorDialog
        modality: Qt.ApplicationModal

        title: "请选择背景色"
              onAccepted: {
                  model.backColor = color;
              }
              onRejected: {
                  console.log("Canceled")
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
            //设置Board的状态为隐藏: 应该提示用户
            closeConfirmDialog.open();
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
            
            //var c = pListView.count

            //这样调用: 是最佳方法吗?
            listModel.addStrike();

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
        id: closeConfirmDialog
        title: "关掉白板"
        icon: StandardIcon.Question
        text: "你要关闭这个白板吗? 你可以在菜单里重新打开"
        //informativeText: "hello"
        standardButtons: StandardButton.Ok |  StandardButton.Cancel
        onAccepted: {
            //设置隐藏
            model.hidden = true;
            window.close()
        }
    }
    
    MessageDialog {
        id: deleteConfirmDialog
        //title: "删除白板"
        icon: StandardIcon.Question
        text: "你要删除这个白板吗? 此白板的数据将全部被删除, 并且不可恢复!"
        standardButtons: StandardButton.Ok |  StandardButton.Cancel
        onAccepted: {
            //移除当前白板
            windowModel.removeBoard(index);
        }
    }

}
