import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1

import StrikeTodo 1.0


Rectangle {
    id: itemRoot

    property alias pListView: listView
    property StrikeListModel listModel
    property string backColor: "#f5e50a"
    property int fontSize: 14

    
    //调试显示
    color: backColor
    //opacity: 0.75
    
    Component {
        id: delegateItem
        
        MouseArea {
            id: mouseArea
            width: itemRoot.width
            
            height: itemRect.height

            anchors {
                left: parent.left
                right: parent.right
            }
            
            //hoverEnabled: true
            
            //拖拽设置
            drag.smoothed: false
            drag.target: itemRect
            drag.axis: Drag.YAxis
            
            //右键菜单
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            
            onClicked: {
                console.debug("onClicked")
                
                //右键菜单
                if (mouse.button === Qt.RightButton){
                    contextMenu.popup()
                }
                else {

                    //方法1: 设置当前
                    listView.currentIndex = index;

                    //禁止右键菜单
                    contextMenu.enabled = false;
                    mouseArea.acceptedButtons = Qt.LeftButton;

                    //console.log(("MouseArea Click listview currentIndex: "                                 + listView.currentIndex + " index: " + index))
                    //console.log(("MouseArea Click ListView isCurrentItem: "                                 + ListView.isCurrentItem))

                    // 在onFocusChanged 中设置了, 此处不需要了
                    //textinput.focus = true;

                }


            }
            
            
            onFocusChanged: {
                if (focus) {
                    /*
                    console.debug("=====got focus of mouseArea, start")
                    console.debug(("got focus of mouseArea, listview currentIndex: "
                                   + listView.currentIndex + " index: " + index))
                    console.debug("got focus of mouseArea, isCurrentItem: " +  mouseArea.ListView.isCurrentItem)
                    console.debug("got focus of mouseArea, drag is active: " + drag.active)
                    console.debug("got focus of mouseArea, textInput visible: " + textinput.visible)
                    */
                    
                    textinput.focus = true
                    
                    //console.debug("=====got focus of mouseArea, end!")
                }
                else {
                    /*
                    console.debug(("lost focus of mouseArea, listview currentIndex: "
                                   + listView.currentIndex + " index: " + index))
                    
                    console.debug("lost focus of mouseArea, isCurrentItem: " +  mouseArea.ListView.isCurrentItem)
                    */
                }
            }
            
            //FIXME: 目前某行处于编辑状态, 然后其他行拖动和此行交换, 则会crash, 原因待查 2020.4.21
            //目前解决的思路: 一旦开始拖拽, 则退出编辑状态
            drag.onActiveChanged: {
                if (mouseArea.drag.active) {
                    //开始拖动时: 设置当前Item为空
                    listView.currentIndex = -1
                }
            }

            //字体颜色选择
            ColorDialog {
                id: colorDialog
                modality: Qt.ApplicationModal

                title: "请选择字体的颜色"
                      onAccepted: {
                          console.log("You chose: " + colorDialog.color)
                          textShow.color = color;
                          model.textColor = color;
                      }
                      onRejected: {
                          console.log("Canceled")
                }
                //Component.onCompleted: visible = true

            }

            //添加新任务, 删除当前, 标记为完成/进行中/初始状态, 设置文字颜色
            CoodMenu {
                  id: contextMenu
                  itemWidth: 180

                  Action { text: "删除当前任务"
                      onTriggered: {
                          console.debug("remove row " + index);
                          listModel.removeStrike(index);
                      }
                  }
                  CoodMenu {
                        title: "标记状态为"

                        Action { text: "未开始"
                            onTriggered: {
                                model.status = 0;
                            }
                        }
                        Action { text: "进行中"
                            onTriggered: {
                                model.status = 1;
                            }
                        }
                        Action { text: "已完成"
                            onTriggered: {
                                model.status = 2;
                            }
                        }
                        Action { text: "已归档"
                            onTriggered: {
                                //Todo 归档就是打个标记: 但是如何隐藏哪

                            }
                        }
                  }

                  MenuSeparator { }

                  CoodMenu {
                        title: "字体设置"

                        Action { text: "黑体"
                            onTriggered: {
                                textShow.font.bold = !textShow.font.bold ;
                                model.fontStyle =
                                        (textShow.font.bold?"bold,":"" )
                                        + ( textShow.font.italic?"italic.":"")
                                        + ( textShow.font.underline?"underline,":"");
                            }
                        }
                        Action { text: "斜体";
                            onTriggered: {
                                textShow.font.italic = !textShow.font.italic;
                                model.fontStyle =
                                        (textShow.font.bold?"bold,":"" )
                                        + ( textShow.font.italic?"italic.":"")
                                        + ( textShow.font.underline?"underline,":"");
                            }
                        }
                        Action { text: "下划线"
                            onTriggered: {
                                textShow.font.underline = !textShow.font.underline;
                                model.fontStyle =
                                        (textShow.font.bold?"bold,":"" )
                                        + ( textShow.font.italic?"italic.":"")
                                        + ( textShow.font.underline?"underline,":"");
                            }
                        }

                        Action { text: "字体颜色..."
                            onTriggered: {
                                colorDialog.open();
                            }
                        }
                  }

            }


            //实际显示内容
            Rectangle {
                id: itemRect

                //Todo: 如何根据文字大小自动适应高度
                //height: 40
                height: textShow.height + 5

                width: mouseArea.width

                color: backColor
                //opacity: 0.75

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                
                //拖拽设置
                Drag.active: mouseArea.drag.active
                Drag.source: mouseArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2
                
                
                //拖拽的状态变化
                states: State {
                    when: itemRect.Drag.active
                    
                    ParentChange {
                        target: itemRect
                        parent: itemRoot
                    }
                    AnchorChanges {
                        target: itemRect
                        anchors {
                            horizontalCenter: undefined
                            verticalCenter: undefined
                        }
                    }
                }
                
                
                CheckBox {
                    id: chkbox
                    tristate: true
                    width: 50
                    //height: parent.height
                    anchors.bottom: parent.bottom
                    //底部留点空间
                    bottomPadding: 3
                    checkState: model.status ===2? Qt.Checked: ( model.status ===1?Qt.PartiallyChecked: Qt.Unchecked );
                    
                    //checked: model.status === 2
                    onClicked: {
                        model.status === 2? model.status = 0: ( model.status ===1? model.status=2: model.status = 1);
                    }
                }
                
                Rectangle {
                    id: textSection
                    //height: parent.height
                    height: textShow.height + 5
                    
                    width: parent.width - chkbox.width
                    anchors.left: chkbox.right
                    color: backColor
                    //opacity: 0.0
                    
                    Text {
                        id: textShow
                        anchors.bottom: parent.bottom

                        text: model.desc
                        
                        //控制可见: 不是当前
                        visible: !mouseArea.ListView.isCurrentItem
                        color: model.textColor
                        font.bold: model.fontStyle.includes("bold")
                        //字体大小
                        font.pixelSize: fontSize
                        
                        //底部留点空间
                        bottomPadding: 3
                        
                    } //end textShow
                    
                    TextInput {
                        id: textinput
                        
                        anchors.bottom: parent.bottom
                        
                        color: "blue"
                        font.pixelSize: fontSize
                        
                        //底部留点空间
                        bottomPadding: 3
                        
                        text: model.desc
                        
                        //控制是否编辑状态
                        visible: mouseArea.ListView.isCurrentItem
                        enabled: mouseArea.ListView.isCurrentItem
                        
                        //focus: true
                        selectByMouse: true
                        
                        //Debug: 不变不会进入的, 例如已经focus, 再次设置不会触发此事件
                        onFocusChanged: {
                            if (focus) {
                                console.debug("got focus " + "textInput")
                            }
                            else {
                                console.debug("lost focus " + "textInput")
                            }
                        }
                        
                        onEditingFinished: {
                            //console.debug("=== start onEditingFinished ")

                            model.desc = textinput.text
                            model.updated = new Date(); //也可以在ListModel中设置, 更合理
                            
                            //方法1: 设置index
                            if (listView.currentIndex == index) {
                                listView.currentIndex = -1;

                                //恢复右键菜单
                                contextMenu.enabled = true;
                                mouseArea.acceptedButtons = Qt.LeftButton | Qt.RightButton;
                            }
                            
                            /*
                            console.log(("TextInput listview currentIndex: "
                                         + listView.currentIndex + " index: " + index))
                            console.log(("TextInput ListView isCurrentItem: "
                                         + mouseArea.ListView.isCurrentItem))
                            
                            console.debug("=== end onEditingFinished ")
                            */
                        }
                    } //end TextInput
                    
                } //end textSection Rectangle
                
                Frame {
                    width: itemRect.width
                    height: 1
                    anchors.bottom: itemRect.bottom
                    //anchors.topMargin: 1
                    
                }
                
            } //end itemRect Rectangle
            
            
            DropArea {
                anchors {
                    fill: parent
                    margins: 10
                }
                
                
                onEntered: {
                    console.debug("===== start DropArea onEntered")
                    console.debug("drag.source.DelegateModel.itemsIndex: " + drag.source.DelegateModel.itemsIndex)
                    console.debug("mouseArea.DelegateModel.itemsIndex: " + mouseArea.DelegateModel.itemsIndex )
                    var srcIndex = drag.source.DelegateModel.itemsIndex;
                    var destIndex = mouseArea.DelegateModel.itemsIndex;
                    
                    //移动Delegate
                    visualModel.items.move(srcIndex, destIndex, 1)
                    
                    //移动Model: 不移动的话model和delegate就不同步了

                    visualModel.model.moveRow(srcIndex, destIndex)

                    
                    console.debug("===== end DropArea onEntered")
                }
            }
            //end DropArea
            
            
        } //end MouseArea
        
    } //end Component
    
    DelegateModel {
        id: visualModel
        
        model: listModel
        delegate: delegateItem
    }
    
    ListView {
        id: listView
        
        width: parent.width
        height: parent.height

        //opacity: 0.9
        
        keyNavigationEnabled: true
        
        //clip: true
        model: visualModel
        //delegate: delegateItem
        
        //默认不要是第一个, 否则第一个是可编辑状态(针对方法1)
        Component.onCompleted: {
            currentIndex = -1
        }
        
        //默认焦点
        focus: true
        
        spacing: 0
    }
}
