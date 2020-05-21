import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.1
import QtQml.Models 2.1

Window {
    id: window

    //Designer 竟然不支持..., 设计模式时要注意
    //@FIXME 最后要打开
    flags: Qt.FramelessWindowHint

    visible: !model.hidden
    width: model.windowWidth
    height: model.windowHeight
    x: (model.windowX > Screen.width)?(Screen.width - width): model.windowX
    y: (model.windowY > Screen.height)?(Screen.height - height): model.windowY


    title: model.title

    //================================================/头部操作+拖拽区/================================================

    BoardTitleSection {
        id: titleOpRect
        pListView: itemRoot.pListView
        title: model.title
        listModel: model.strikeListModel
        backColor: model.backColor
    }


    //================================================/ListView Section/================================================
    //列表容器
    MainSection {
        id: itemRoot
        anchors.top: titleOpRect.bottom
        width: parent.width
        height: parent.height - titleOpRect.height - footOpRect.height
        listModel: model.strikeListModel
        backColor: model.backColor
        fontSize: model.fontSize
    }


    //================================================/尾部操作区/================================================

    FootOpSection {
        id: footOpRect
        pListView: itemRoot.pListView
        backColor: model.backColor
    }

}
