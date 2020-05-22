#include "board.h"
#include "strikelistmodel.h"
#include "boardwindowlistmodel.h"
#include "datamanager.h"
#include "mockhelper.h"

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication app(argc, argv);

  //所有窗口关闭程序也不退出 Todo 最后打开 而且应该对话框提示用户
  //QApplication::setQuitOnLastWindowClosed(false);


  DataManager dataMan;

  //start data ============================================

  //手工模拟数据
  //MockHelper* mock = new MockHelper();
  //BoardWindowListModel* bwlm = mock->mockData();

  //从文件读取
  QList<Board*> *boards = dataMan.readAllBoards();


  BoardWindowListModel* bwlm = new BoardWindowListModel();
  bwlm->setBoardsAndProxy(boards);


  //end data  ============================================

  QQmlApplicationEngine engine;

  qmlRegisterType<StrikeListModel>("StrikeTodo", 1, 0, "StrikeListModel");


  //set model
  engine.rootContext()->setContextProperty("windowModel", bwlm);


  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);


  //监听窗口数据变化, 实时保存
  QObject::connect(bwlm, &BoardWindowListModel::dataChanged, &dataMan, &DataManager::windowModelDataChanged);
  QObject::connect(bwlm, &BoardWindowListModel::rowsInserted, &dataMan, &DataManager::windowModelRowsInserted);
  QObject::connect(bwlm, &BoardWindowListModel::rowsMoved, &dataMan, &DataManager::windowModelRowsMoved);
  QObject::connect(bwlm, &BoardWindowListModel::rowsRemoved, &dataMan, &DataManager::windowModelRowsRemoved);


  QList<BoardModelProxy*>* theBoardProxyList = bwlm->boardModelProxys();

  //每个白板的任务列表都要监测: 方便保存数据
  for(int i=0; i< theBoardProxyList->size();++i){
    BoardModelProxy* proxy = theBoardProxyList->at(i);
    StrikeListModel* slm = proxy->strikeListModel();

    //绑定事件: 但是不知道是哪个StrikeList! 自定义信号可以解决, 目前应该不需要
    QObject::connect(slm, &StrikeListModel::dataChanged, &dataMan, &DataManager::strikeModelDataChanged);
    QObject::connect(slm, &StrikeListModel::rowsInserted, &dataMan, &DataManager::strikeModelRowsInserted);
    QObject::connect(slm, &StrikeListModel::rowsMoved, &dataMan, &DataManager::strikeModelRowsMoved);
    QObject::connect(slm, &StrikeListModel::rowsRemoved, &dataMan, &DataManager::strikeModelRowsRemoved);

    //归档
    QObject::connect(slm, &StrikeListModel::strikeArchived, &dataMan, &DataManager::archivedStrike);

  }




  engine.load(url);

  return app.exec();
}


