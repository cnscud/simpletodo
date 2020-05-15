import QtQuick 2.12
import QtQuick.Controls 2.12

/**
  自定义样式的菜单. (Cood = Cool+Good, 用作自定义样式的控件)

  Todo: 选择框和 文字之间的间距问题
*/
Menu {

        topPadding: 1
        bottomPadding: 1
        font {
            pointSize: 14
        }

        //width: itemWidth

        property int itemHeight : 30
        property int itemWidth: 150
        property int arrowHeight: 10

        delegate: MenuItem {
            id: menuItem
            implicitWidth: itemWidth
            implicitHeight: itemHeight

            width: itemWidth

            arrow: Canvas {
                id: arrowCanvas
                x: parent.width - width
                implicitWidth: 40
                implicitHeight: itemHeight
                visible: menuItem.subMenu
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
                    ctx.moveTo((width - arrowHeight)/2, (height - arrowHeight) /2)
                    ctx.lineTo( width/2 + arrowHeight/2 , height/2)
                    ctx.lineTo( (width - arrowHeight)/2, height/2 + arrowHeight/2)
                    ctx.closePath()
                    ctx.fill()

                }
            }

            //选中时重画箭头
            onHighlightedChanged: {
                if(menuItem.subMenu) {
                    arrowCanvas.requestPaint()
                }
            }



            indicator: Item {

                implicitWidth: 24
                implicitHeight: itemHeight
                anchors.leftMargin: 2
                anchors.rightMargin: 0
                width: 24

                Rectangle {
                    width: 20
                    height: 20
                    anchors.centerIn: parent                    
                    visible: menuItem.checkable
                    border.color: "#21be2b"
                    radius: 3
                    Rectangle {
                        width: 14
                        height: 14
                        anchors.centerIn: parent
                        visible: menuItem.checked
                        color: "#21be2b"
                        radius: 2
                    }
                }
            }

            contentItem: Text {
                leftPadding: menuItem.indicator.width
                rightPadding: menuItem.arrow.width
                text: menuItem.text
                font: menuItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuItem.highlighted ? "#ffffff" : "#21be2b"

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                //注意!!! 太窄文字就缩略
                //elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: itemWidth
                implicitHeight: itemHeight
                opacity: enabled ? 1 : 0.3
                color: menuItem.highlighted ? "#21be2b" : "transparent"
            }
        }


        background: Rectangle {
            implicitWidth: itemWidth
            implicitHeight: itemHeight
            color: "#ffffff"
            border.color: "#21be2b"
            radius: 2
        }



}
