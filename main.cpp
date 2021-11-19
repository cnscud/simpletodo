#include "board.h"
#include "strikelistmodel.h"
#include "boardwindowlistmodel.h"
#include "datamanager.h"
#include "mockhelper.h"
#include "archivedcontroller.h"

#include <QApplication>
#include <QGuiApplication>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QTranslator>


int main(int argc, char *argv[]) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QApplication app(argc, argv);

        //所有窗口关闭程序也不退出 Todo 关闭最后一个窗口时应打开对话框提示用户
        QApplication::setQuitOnLastWindowClosed(false);


        //单例检测开始================================================================
        // 创建信号量
        QSystemSemaphore semaphore("simpletodo_felix2Semaphore", 1);
        // 启用信号量，禁止其他实例通过共享内存一起工作
        semaphore.acquire();

#ifndef Q_OS_WIN32
        // 在linux / unix 程序异常结束共享内存不会回收
        // 在这里需要提供释放内存的接口，就是在程序运行的时候如果有这段内存 先清除掉
        QSharedMemory nix_fix_shared_memory("simpletodo_felix");
        if(nix_fix_shared_memory.attach()) {
                nix_fix_shared_memory.detach();
        }
#endif

        // 创建一个共享内存  “SingleAppTest2”表示一段内存的标识key 可作为唯一程序的标识
        QSharedMemory sharedMemory("simpletodo_felix");
        // 测试是否已经运行
        bool isRunning = false;
        // 试图将共享内存的副本附加到现有的段中。
        if(sharedMemory.attach()) {
                // 如果成功，则确定已经存在运行实例
                isRunning = true;
        } else {
                // 否则申请一字节内存
                sharedMemory.create(1);
                // 确定不存在运行实例
                isRunning = false;
        }
        semaphore.release();

        // 如果您已经运行了应用程序的一个实例，那么我们将通知用户。
        if(isRunning) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("此程序已经在运行了, 如果你关闭了所有白板窗口, 则请使用系统菜单来打开任务白板");
                msgBox.exec();
                return 1;
        }

        //单例检测结束================================================================

        //装载中文语言
        QTranslator tran;
        if( tran.load("qt_zh_CN.qm", ":/")) { //相对路径.
          qDebug("tran load ok ");
          QApplication::installTranslator(&tran);
        }


        //=======================================================

        DataManager dataMan;
        ArchivedController archivedController;

        //start data ============================================

        //手工模拟数据
        //MockHelper* mock = new MockHelper();
        //BoardWindowListModel* bwlm = mock->mockData();

        //从文件读取
        QList<Board*> *boards = dataMan.readAllData();


        BoardWindowListModel* bwlm = new BoardWindowListModel();
        bwlm->setBoardsAndProxy(boards);

        archivedController.setDataManager(&dataMan);


        //end data  ============================================

        QQmlApplicationEngine engine;

        qmlRegisterType<StrikeListModel>("StrikeTodo", 1, 0, "StrikeListModel");


        //set model
        engine.rootContext()->setContextProperty("windowModel", bwlm);


        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject * obj, const QUrl & objUrl) {
                if(!obj && url == objUrl)
                        QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);


        //监听窗口数据变化, 实时保存
        QObject::connect(bwlm, &BoardWindowListModel::dataChanged, &dataMan, &DataManager::windowModelDataChanged);
        QObject::connect(bwlm, &BoardWindowListModel::rowsInserted, &dataMan, &DataManager::windowModelRowsInserted);
        QObject::connect(bwlm, &BoardWindowListModel::rowsMoved, &dataMan, &DataManager::windowModelRowsMoved);
        QObject::connect(bwlm, &BoardWindowListModel::rowsRemoved, &dataMan, &DataManager::windowModelRowsRemoved);


        QList<BoardModelProxy*>* theBoardProxyList = bwlm->boardModelProxys();

        //每个白板的任务列表都要监测: 方便保存数据
        for(int i = 0; i < theBoardProxyList->size(); ++i) {
                BoardModelProxy* proxy = theBoardProxyList->at(i);
                StrikeListModel* slm = proxy->strikeListModel();

                //绑定事件: 但是不知道是哪个StrikeList! 自定义信号可以解决, 目前应该不需要
                QObject::connect(slm, &StrikeListModel::dataChanged, &dataMan, &DataManager::strikeModelDataChanged);
                QObject::connect(slm, &StrikeListModel::rowsInserted, &dataMan, &DataManager::strikeModelRowsInserted);
                QObject::connect(slm, &StrikeListModel::rowsMoved, &dataMan, &DataManager::strikeModelRowsMoved);
                QObject::connect(slm, &StrikeListModel::rowsRemoved, &dataMan, &DataManager::strikeModelRowsRemoved);

                //归档: 需要是同步执行, 否则会丢失数据?
                QObject::connect(slm, &StrikeListModel::strikeArchived, &dataMan, &DataManager::archivedStrike);

        }

        //归档窗口事件
        QObject::connect(bwlm, &BoardWindowListModel::userWillViewArchivedStrikes, &archivedController, &ArchivedController::viewArchivedStrikes);


        //主现场
        engine.load(url);

        return app.exec();
}


