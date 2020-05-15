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

    Label {
        id: totalLbl
        //text:  "Total "+ listView.model.model.rowCount() +" Strikes"
        text: "Total " + pListView.count + " Strikes"
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    Button {
        text: qsTr("New Board --- Todo ")
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom

        onClicked: {
            //FIXME: just for test
            windowModel.append({
                                   "title": "Window #" + (windowModel.count + 1)
                               })
        }
    }

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

        onPressed: {
            resize.clickPos = Qt.point(mouse.x, mouse.y)
        }

        onPositionChanged: {
            var delta = Qt.point(mouse.x - resize.clickPos.x,
                                 mouse.y - resize.clickPos.y)
            window.width += delta.x
            window.height += delta.y
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
