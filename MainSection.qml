import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1

Rectangle {
    id: itemRoot

    property alias pListView: listView

    
    //调试显示
    //color: "blue"
    //border.width: 2
    
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
            
            
            
            onClicked: {
                console.debug("onClicked")
                
                //方法1: 设置当前
                listView.currentIndex = index
                
                console.log(("MouseArea Click listview currentIndex: "
                             + listView.currentIndex + " index: " + index))
                console.log(("MouseArea Click ListView isCurrentItem: "
                             + ListView.isCurrentItem))
                
                // 在onFocusChanged 中设置了, 此处不需要了
                //textinput.focus = true;
            }
            
            
            onFocusChanged: {
                if (focus) {
                    console.debug("=====got focus of mouseArea, start")
                    console.debug(("got focus of mouseArea, listview currentIndex: "
                                   + listView.currentIndex + " index: " + index))
                    console.debug("got focus of mouseArea, isCurrentItem: " +  mouseArea.ListView.isCurrentItem)
                    console.debug("got focus of mouseArea, drag is active: " + drag.active)
                    console.debug("got focus of mouseArea, textInput visible: " + textinput.visible)
                    
                    textinput.focus = true
                    
                    console.debug("=====got focus of mouseArea, end!")
                }
                else {
                    console.debug(("lost focus of mouseArea, listview currentIndex: "
                                   + listView.currentIndex + " index: " + index))
                    
                    console.debug("lost focus of mouseArea, isCurrentItem: " +  mouseArea.ListView.isCurrentItem)
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
            
            //实际显示内容
            Rectangle {
                id: itemRect
                height: 40
                width: mouseArea.width
                
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
                    width: 50
                    //height: parent.height
                    anchors.bottom: parent.bottom
                    //底部留点空间
                    bottomPadding: 3
                    
                    checked: model.done
                    onClicked: model.done = checked
                }
                
                Rectangle {
                    id: textSection
                    height: parent.height
                    
                    width: parent.width - chkbox.width
                    anchors.left: chkbox.right
                    
                    Text {
                        id: textShow
                        
                        text: model.description
                        anchors.bottom: parent.bottom
                        
                        //控制可见: 不是当前
                        visible: !mouseArea.ListView.isCurrentItem
                        
                        //底部留点空间
                        bottomPadding: 3
                        
                    } //end textShow
                    
                    TextInput {
                        id: textinput
                        
                        anchors.bottom: parent.bottom
                        
                        color: "blue"
                        
                        //底部留点空间
                        bottomPadding: 3
                        
                        text: model.description
                        
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
                            console.debug("=== start onEditingFinished ")
                            model.description = textinput.text
                            
                            //方法1: 设置index
                            if (listView.currentIndex == index) {
                                listView.currentIndex = -1
                            }
                            
                            console.log(("TextInput listview currentIndex: "
                                         + listView.currentIndex + " index: " + index))
                            console.log(("TextInput ListView isCurrentItem: "
                                         + mouseArea.ListView.isCurrentItem))
                            
                            console.debug("=== end onEditingFinished ")
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
                    
                    //移动Delegate
                    visualModel.items.move(
                                drag.source.DelegateModel.itemsIndex,
                                mouseArea.DelegateModel.itemsIndex, 1)
                    
                    //移动Model: 不移动的话model和delegate就不同步了
                    visualModel.model.move(
                                drag.source.DelegateModel.itemsIndex,
                                mouseArea.DelegateModel.itemsIndex, 1)
                    
                    console.debug("===== end DropArea onEntered")
                }
            }
            //end DropArea
            
            
        } //end MouseArea
        
    } //end Component
    
    DelegateModel {
        id: visualModel
        
        model: MyModel {}
        delegate: delegateItem
    }
    
    ListView {
        id: listView
        
        width: parent.width
        height: parent.height
        
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
