#include "archivedcontroller.h"
#include "strikelistmodel.h"

#include <QQuickView>
#include <QQmlContext>
#include <QMessageBox>
#include <QQmlPropertyMap>


ArchivedController::ArchivedController() {

}


void ArchivedController::viewArchivedStrikes(QString bid) {
        qDebug("Hello, user will view board: %s", qPrintable(bid));


        //根据DataManager找到对应的Board
        QString archived_bid = Archived_prefix + bid;
        QList<Board *> *archivedBoards = m_dataManager->archivedBoards();
        Board* destBoard = nullptr;
        for(int i = 0; i < archivedBoards->size(); ++i) {
                Board* aBoard = archivedBoards->at(i);
                if(aBoard->getBid() == archived_bid) {
                        destBoard = aBoard;
                        break;
                }
        }

        if(destBoard == nullptr) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("目前没有归档数据");
                msgBox.exec();

                return;
        }


        //打开窗口
        QQuickView *view = new QQuickView;
        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->setModality(Qt::ApplicationModal); //Modal窗口
        view->setFlag(Qt::FramelessWindowHint);

        view->rootContext()->setContextProperty("window", view);

        //board数据和strikes数据

        QQmlPropertyMap boardMap; //为了简单, 否则还要改动Board, 例如使用Property
        boardMap.insert("title", destBoard->getTitle());
        boardMap.insert("updated",  destBoard->getUpdated().toString(DateTimeFormat));

        view->rootContext()->setContextProperty("board", &boardMap);

        StrikeListModel* strikeListModel = new StrikeListModel();
        strikeListModel->setBoard(destBoard);

        view->rootContext()->setContextProperty("strikeListModel", strikeListModel);


        view->setSource(QUrl(QStringLiteral("qrc:/ArchivedBoardDialog.qml")));
        view->show();



}

void ArchivedController::setDataManager(DataManager *dataManager) {
        m_dataManager = dataManager;
}
