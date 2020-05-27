#include "datamanager.h"

#include <QDir>
#include <QFuture>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>
#include <QThread>
#include <QtConcurrent>

DataManager::DataManager() {

}

void DataManager::windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(roles);

        qDebug("[windowModelDataChanged] row %d to row %d", topLeft.row(), bottomRight.row());

        fireSaveData();
}

void DataManager::windowModelRowsInserted(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);
        qDebug("[windowModelRowsInserted] first %d to last %d", first, last);

        fireSaveData();
}

void DataManager::windowModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
        Q_UNUSED(parent);
        Q_UNUSED(destination);

        qDebug("[windowModelRowsMoved] start %d to end %d, to row %d", start, end, row);

        fireSaveData();
}

void DataManager::windowModelRowsRemoved(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);

        qDebug("[windowModelRowsRemoved] first %d to last %d", first, last);

        fireSaveData();
}

void DataManager::strikeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(roles);
        qDebug("[strikeModelDataChanged] datachanged row %d to row %d", topLeft.row(), bottomRight.row());

        fireSaveData();
}

void DataManager::strikeModelRowsInserted(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);
        qDebug("[strikeModelRowsInserted] first %d to last %d", first, last);

        fireSaveData();
}

void DataManager::strikeModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
        Q_UNUSED(parent);
        Q_UNUSED(destination);

        qDebug("[strikeModelRowsMoved] start %d to end %d, to row %d", start, end, row);

        fireSaveData();
}

void DataManager::strikeModelRowsRemoved(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);

        qDebug("[strikeModelRowsRemoved] first %d to last %d", first, last);

        fireSaveData();
}

void DataManager::archivedStrike(QString bid, Strike &strike) {
        qDebug("catch a archived Strike singal: %s", qPrintable(strike.getSid()));


        Board* destBoard = nullptr;
        for(int i = 0; i < m_boards->size(); ++i) {
                Board* aboard = m_boards->at(i);
                if(aboard->getBid() == bid) {
                        destBoard = aboard;
                        break;
                }
        }

        //竟然没有这个Board
        if(destBoard == nullptr) {
                qErrnoWarning("Not find the board %s for archived strikes", qPrintable(bid));

                //啥也不干....
                return;
        }

        Strike destStrike = strike;

        Board* destArchivedBoard = nullptr;

        QString archived_bid = archived_prefix + bid;

        //查找Board, 如果不存在, 则创建一个
        for(int i = 0; i < m_archivedBoards->size(); ++i) {
                Board* archivedBoard = m_archivedBoards->at(i);
                if(archivedBoard->getBid() == archived_bid) {
                        destArchivedBoard = archivedBoard;
                        break;
                }
        }

        if(destArchivedBoard == nullptr) {
                destArchivedBoard = new Board(true);
                m_archivedBoards->append(destArchivedBoard);
        }

        //设置Bid, Title
        destArchivedBoard->setBid(archived_bid);
        destArchivedBoard->setTitle(destBoard->getTitle());
        destArchivedBoard->setCreated(destBoard->getCreated());
        destArchivedBoard->setUpdated(destBoard->getUpdated());

        //加入此归档的任务
        destArchivedBoard->insertItem(&strike);

        qDebug("end catch archived Strike signal");

        //存储 Todo ? 需要吗: 因为归档时会移除当前行, 会触发fireSaveData, 所以目前此处没有调用
        //fireSaveData();

        qDebug("fire archived Strike signal --> save data");
}



QList<Board*>* DataManager::readAllBoards() {
        QJsonDocument doc = readDataFromFile();

        //parse json
        QJsonObject json = doc.object();

        if(json.contains("all") && json["all"].isArray()) {
                QJsonArray allArray = json["all"].toArray();

                QList<Board*> *boardList = new QList<Board*>();

                for(int n = 0; n < allArray.size(); ++n) {
                        QString listName = allArray[n].toString();

                        //读一个任务列表
                        Board* oneList = parseOneBoard(json, listName, false);

                        boardList->append(oneList);
                }

                m_boards = boardList; //
        }

        //没数据: 默认数据
        if(m_boards == nullptr || m_boards->size() <= 0) {
                m_boards = prepareDefaultBoard();
        }

        //todo 读取归档数据
        //m_archivedBoards  = new QList<Board*>();

        if(json.contains("archived_all") && json["archived_all"].isArray()) {
                QJsonArray archivedAllArray = json["archived_all"].toArray();

                QList<Board*> *archivedBoardList = new QList<Board*>();

                for(int n = 0; n < archivedAllArray.size(); ++n) {
                        QString listName = archivedAllArray[n].toString();

                        //读一个任务列表
                        Board* oneList = parseOneBoard(json, listName, true);

                        archivedBoardList->append(oneList);
                }

                m_archivedBoards = archivedBoardList; //
        }


        return m_boards;
}


Board* DataManager::parseOneBoard(QJsonObject &json, QString &abbr, bool archived) {
        // read one board from json
        Board* board = new Board();

        if(!json.contains(abbr)) {
                return board;
        }

        //分析设置数据

        QJsonObject oneSection = json[abbr].toObject();

        board->setBid(oneSection["bid"].toString());

        QString title = oneSection["title"].toString();
        board->setTitle(title);

        if(!archived) {
                board->setHidden(oneSection["hidden"].toBool());

                if(oneSection.contains("backColor")) {
                        board->setBackColor(oneSection["backColor"].toString());
                }

                if(oneSection.contains("fontSize")) {
                        board->setFontSize(oneSection["fontSize"].toInt());
                }

                if(oneSection.contains("fontFamily")) {
                        board->setFontFamily(oneSection["fontFamily"].toString());
                }

                board->setHiddenArchived(oneSection["hiddenArchived"].toBool());

                board->setWindowX(oneSection["windowX"].toInt());
                board->setWindowY(oneSection["windowY"].toInt());
                board->setWindowWidth(oneSection["windowWidth"].toInt());
                board->setWindowHeight(oneSection["windowHeight"].toInt());
        }

        board->setCreated(QDateTime::fromString(oneSection["created"].toString()));
        board->setUpdated(QDateTime::fromString(oneSection["updated"].toString()));

        //parse Strike list
        if(oneSection.contains("items") && oneSection["items"].isArray()) {

                QJsonArray itemArray = oneSection["items"].toArray();

                QList<Strike*> *items = new QList<Strike*>();

                for(int n = 0; n < itemArray.size(); ++n) {
                        QJsonObject item = itemArray[n].toObject();

                        //parse to strike
                        Strike* strike = new Strike();
                        strike->setSid(item["sid"].toString());
                        strike->setDesc(item["desc"].toString());
                        strike->setStatus(item["status"].toInt());
                        if(item.contains("textColor")) {
                                strike->setTextColor(item["textColor"].toString());
                        }
                        strike->setFontStyle(item["fontStyle"].toString());
                        strike->setCreated(QDateTime::fromString(item["created"].toString()));
                        strike->setUpdated(QDateTime::fromString(item["updated"].toString()));


                        items->append(strike);
                }

                board->setItems(items);
        }


        return board;
}

QList<Board *> *DataManager::prepareDefaultBoard() {
        Board* board = new Board(true);
        board->setBid(HelpUtils::uuid());
        board->setTitle("第一个白板");

        board->setCreated(QDateTime::currentDateTime());
        board->setUpdated(QDateTime::currentDateTime());

        QString task1 = "第一个任务";
        board->appendNewItem(task1);

        Strike* strike2 = new Strike();
        strike2->setSid(HelpUtils::uuid());
        strike2->setDesc("已经完成的任务");
        strike2->setStatus(Strike::Done);
        strike2->setCreated(QDateTime::currentDateTime());
        strike2->setUpdated(QDateTime::currentDateTime());

        board->appendItem(strike2);

        Strike* strike3 = new Strike();
        strike3->setSid(HelpUtils::uuid());
        strike3->setDesc("进行中的任务");
        strike3->setStatus(Strike::Working);
        strike3->setCreated(QDateTime::currentDateTime());
        strike3->setUpdated(QDateTime::currentDateTime());

        board->appendItem(strike3);

        QList<Board *> * list = new QList<Board *>();
        list->append(board);

        return list;
}

//获取数据文件路径
QString DataManager::pickDataFilePathName() {

        //debug
        //qDebug("hello %s", qPrintable(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));

        QString pathRoot = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)  + "/";

        //创建目录
        QDir dir(pathRoot);
        if(!dir.exists(pathRoot)) {
                if(!dir.mkdir(pathRoot)) {
                        qWarning("创建目录失败 %s", qPrintable(pathRoot));
                }
        }

        return pathRoot + dataFileName;
}

//读取Json文档
QJsonDocument DataManager::readDataFromFile() {

        QString pathName = pickDataFilePathName();

        QFile loadFile(pathName);

        if(!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open data file %s", qPrintable(pathName));
                return QJsonDocument();
        }

        QByteArray saveData = loadFile.readAll();

        QJsonParseError error;

        QJsonDocument loadDoc = QJsonDocument::fromJson(saveData, &error);

        //检查错误
        if(error.error != QJsonParseError::NoError) {
                qFatal("Data File Parse Error: %s", error.errorString().toUtf8().constData());
        }

        return loadDoc;
}




void DataManager::fireSaveData() {
        fireSaveCounter++;

        qDebug("fired save data ============= start %d", fireSaveCounter);


        if(needMoreSave) {
                return;
        } else {
                needMoreSave = true;
        }

        qDebug("before to call doSave......");

        //新线程运行
        QFuture<void> future = QtConcurrent::run([=]() {
          // Code in this block will run in another thread
          syncCallSaveData();
        });

        qDebug("after call doSave......");


        qDebug("fired save data ============= end %d ", fireSaveCounter);
}

//此函数在独立线程里运行
void DataManager::syncCallSaveData() {
    //加锁: 如果已经有保存在进行, 则等待解锁: 只会有一个在等待
    mutexSaveData.lock();

    //如果两个调用同时到达, 第二个等待获取锁进入之后: 二次检查
    if(!needMoreSave) {
      mutexSaveData.unlock();
      return;
    }

    savingData = true;
    needMoreSave = false;

    //!!! 修改的过程中 会产生脏数据 FIXME 是否需要clone数据?
    doSaveData();
    savingData = false;

    //解锁
    mutexSaveData.unlock();
}


QJsonObject DataManager::transferBoardToJson(Board* board, bool archived) {
        QJsonObject boardObj;
        boardObj["bid"] = board->getBid();
        boardObj["title"] = board->getTitle();

        if(!archived) {
                boardObj["hidden"] = board->getHidden();
                boardObj["backColor"] = board->getBackColor();
                boardObj["fontSize"] = board->getFontSize();
                boardObj["fontFamily"] = board->getFontFamily();
                boardObj["hiddenArchived"] = board->getHiddenArchived();
                boardObj["windowX"] = board->getWindowX();
                boardObj["windowY"] = board->getWindowY();
                boardObj["windowWidth"] = board->getWindowWidth();
                boardObj["windowHeight"] = board->getWindowHeight();
        }

        boardObj["created"] = board->getCreated().toString("yyyy-MM-dd hh:mm:ss");
        boardObj["updated"] = board->getUpdated().toString("yyyy-MM-dd hh:mm:ss");


        QJsonArray boardItemsArray;
        QList<Strike*> *items = board->getItems();

        for(int m = 0; m < items->count(); ++m) {
                Strike* item = items->at(m);
                QJsonObject itemObj;
                itemObj["sid"] = item->getSid();
                itemObj["desc"] = item->getDesc();
                itemObj["status"] = item->getStatus();
                itemObj["textColor"] = item->getTextColor();
                itemObj["fontStyle"] = item->getFontStyle();

                itemObj["created"] = item->getCreated().toString("yyyy-MM-dd hh:mm:ss");
                itemObj["updated"] = item->getUpdated().toString("yyyy-MM-dd hh:mm:ss");

                boardItemsArray.append(itemObj);
        }

        boardObj["items"] = boardItemsArray;

        return boardObj;
}

void DataManager::doSaveData() {
        doSaveCounter++;


        //save data
        qDebug("hello start to save data......%d ", doSaveCounter);

        //for test
        //QThread::msleep(1000);


        QJsonObject saveObject;

        QJsonArray allArray;
        for(int n = 0; n < m_boards->size(); ++n) {
                Board* board = m_boards->at(n);

                allArray.append(board->getBid());

                QJsonObject boardObj = transferBoardToJson(board, false);

                saveObject[board->getBid()] = boardObj;
        }

        saveObject["all"] = allArray;

        //保存已归档的数据
        QJsonArray archivedAllArray;
        for(int n = 0; n < m_archivedBoards->size(); ++n) {
                Board* board = m_archivedBoards->at(n);

                QString boardKey = board->getBid();

                archivedAllArray.append(boardKey);

                QJsonObject boardObj = transferBoardToJson(board, true);

                saveObject[boardKey] = boardObj;
        }

        saveObject["archived_all"] = archivedAllArray;


        //保存文件名
        QString pathName = pickDataFilePathName();

        QSaveFile file(pathName);
        if(!file.open(QIODevice::WriteOnly)) {
                qWarning("Couldn't open save file %s", qPrintable(pathName));
                return;
        }

        QJsonDocument saveDoc(saveObject);
        file.write(saveDoc.toJson());

        //真正保存到实际文件
        file.commit();

        qDebug("hello end to save data......%d. ", doSaveCounter);
}


