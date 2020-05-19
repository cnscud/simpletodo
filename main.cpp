#include "board.h"
#include "strikelistmodel.h"
#include "boardwindowlistmodel.h"
#include "datamanager.h"

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  //所有窗口关闭程序也不退出 Todo 最后打开 而且应该提示用户
  //QApplication::setQuitOnLastWindowClosed(false);


  DataManager dataMan;

  //start data ============================================
  //手工模拟数据
  Board* board1 = new Board();
  board1->setBid("b1111");
  board1->setTitle("Board Todo 1111");

  Board* board2 = new Board();
  board2->setBid("b222");
  board2->setTitle("Board Todo 2222");


  Strike strike11;
  strike11.setSid("111-111");
  strike11.setDesc("Working at home");

  Strike strike12;
  strike12.setSid("1111-222");
  strike12.setDesc("Clean the room");

  QList<Strike*> strikeList1;
  strikeList1 << &strike11 << &strike12;

  StrikeListModel* strikeListModel1 = new StrikeListModel();
  strikeListModel1->setStrikes(strikeList1);
  strikeListModel1->setBoard(board1);


  Strike strike21;
  strike21.setSid("222-222");
  strike21.setDesc("Wash my car");

  Strike strike22;
  strike22.setSid("222-333");
  strike22.setDesc("Shopping for apple");

  QList<Strike*> strikeList2;
  strikeList2 << &strike21 << &strike22;

  StrikeListModel* strikeListModel2 = new StrikeListModel();
  strikeListModel2->setStrikes(strikeList2);
  strikeListModel2->setBoard(board2);



  BoardModelProxy bmp1;
  bmp1.setBoard(board1);
  bmp1.setStrikeListModel(strikeListModel1);

  BoardModelProxy bmp2;
  bmp2.setBoard(board2);
  bmp2.setStrikeListModel(strikeListModel2);

  QList<BoardModelProxy*> boardProxyList;
  boardProxyList << &bmp1 << &bmp2;

  BoardWindowListModel bwlm;
  bwlm.setBoardModelProxys(boardProxyList);

  //end data  ============================================

  QQmlApplicationEngine engine;

  qmlRegisterType<StrikeListModel>("StrikeTodo", 1, 0, "StrikeListModel");


  //set model
  engine.rootContext()->setContextProperty("windowModel", &bwlm);



  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);


  //监听窗口数据变化, 实时保存
  QObject::connect(&bwlm, &BoardWindowListModel::dataChanged, &dataMan, &DataManager::windowModelDataChanged);
  QObject::connect(&bwlm, &BoardWindowListModel::rowsInserted, &dataMan, &DataManager::windowModelRowsInserted);
  QObject::connect(&bwlm, &BoardWindowListModel::rowsMoved, &dataMan, &DataManager::windowModelRowsMoved);
  QObject::connect(&bwlm, &BoardWindowListModel::rowsRemoved, &dataMan, &DataManager::windowModelRowsRemoved);

  //每个白板的任务列表都要监测: 方便保存数据
  for(int i=0; i< boardProxyList.size();++i){
    BoardModelProxy* proxy = boardProxyList.at(i);
    StrikeListModel* slm = proxy->strikeListModel();

    //绑定事件: 但是不知道是哪个StrikeList! 自定义信号可以解决, 目前应该不需要
    QObject::connect(slm, &StrikeListModel::dataChanged, &dataMan, &DataManager::strikeModelDataChanged);
    QObject::connect(slm, &StrikeListModel::rowsInserted, &dataMan, &DataManager::strikeModelRowsInserted);
    QObject::connect(slm, &StrikeListModel::rowsMoved, &dataMan, &DataManager::strikeModelRowsMoved);
    QObject::connect(slm, &StrikeListModel::rowsRemoved, &dataMan, &DataManager::strikeModelRowsRemoved);
  }



  engine.load(url);

  return app.exec();
}
