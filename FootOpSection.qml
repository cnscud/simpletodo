import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1


/**
  尾部操作区域.
*/
Rectangle {
    id: footOpRect
    anchors.bottom: parent.bottom
    width: parent.width
    height: 30

    //父窗口的ListView
    property ListView pListView
    property string backColor: "#f5e50a"

    color: backColor

    Label {
        id: totalLbl
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

        text: "Total " + pListView.count + " Tasks"

    }

    /*
    Button {
        text: qsTr("New Board ")
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom

        onClicked: {
            //just for test 新增新窗口
            windowModel.addBoard();
        }
    }*/

    //resize区域
    MouseArea {
        id: resize
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        width: 15
        height: 15
        cursorShape: Qt.SizeFDiagCursor

        property point clickPos: "1,1"
        //property int oldWidth
        //property int oldHeight

        //保持窗口左上角不动
        property int oldX
        property int oldY

        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
            //oldWidth = window.width
            //oldHeight = window.height
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

            //保存: 窗口大小
            model.windowWidth = window.width;
            model.windowHeight = window.height;
        }

        onReleased: {
            //window.height = .75 * window.width
        }

        //Todo: 用图来替换

        Rectangle {
            id: resizeHint
            color: "red"
            anchors.fill: resize
        }
    }
}
