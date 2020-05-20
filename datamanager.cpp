#include "datamanager.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>

DataManager::DataManager() {

}

void DataManager::windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(roles);

        qDebug("[windowModelDataChanged] row %d to row %d", topLeft.row(), bottomRight.row());

        //Save Model Data
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





QList<Board*>* DataManager::readAllBoards() {
        QJsonDocument doc = readDataFromFile();

        //parse json
        QJsonObject json = doc.object();

        if(json.contains("all") && json["all"].isArray()) {
                QJsonArray allArray = json["all"].toArray();

                QList<Board*> *todoLists = new QList<Board*>();

                for(int n = 0; n < allArray.size(); ++n) {
                        QString listName = allArray[n].toString();

                        //读一个任务列表
                        Board* oneList = parseOneBoard(json, listName);

                        todoLists->append(oneList);
                }

                mBoards = todoLists; //
                return todoLists;
        }

        return new QList<Board*>();
}


Board* DataManager::parseOneBoard(QJsonObject &json, QString &abbr) {
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

        board->setHidden(oneSection["hidden"].toBool());

        if(oneSection.contains("backColor") ) {
          board->setBackColor(oneSection["backColor"].toString());
        }

        if(oneSection.contains("fontSize")){
          board->setFontSize(oneSection["fontSize"].toInt());
        }

        board->setHiddenArchived(oneSection["hiddenArchived"].toBool());

        board->setWindowX(oneSection["windowX"].toInt());
        board->setWindowY(oneSection["windowY"].toInt());
        board->setWindowWidth(oneSection["windowWidth"].toInt());
        board->setWindowHeight(oneSection["windowHeight"].toInt());

        board->setCreated(QDateTime::fromString(oneSection["created"].toString()));
        board->setUpdated(QDateTime::fromString(oneSection["updated"].toString()));


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
                        if(item.contains("textColor")){
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

//获取数据文件路径
QString DataManager::pickDataFilePathName() {

        //debug
        qDebug("hello %s", qPrintable(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));

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



void DataManager::fireSaveData(){
  qDebug("fired save data ============= start");

  //really save
  doSaveData();

  qDebug("fired save data ============= end  ");
}


//只负责存储数据, 不处理并发
void DataManager::doSaveData() {
  //save data
  qDebug("hello start to save data......");

  QJsonObject saveObject;

  QJsonArray allArray;
  for(int n = 0; n < mBoards->size(); ++n) {
    Board* board = mBoards->at(n);

    allArray.append(board->getBid());

    QJsonObject boardObj;
    boardObj["bid"] = board->getBid();
    boardObj["title"] = board->getTitle();
    boardObj["hidden"] = board->getHidden();
    boardObj["backColor"] = board->getBackColor();
    boardObj["fontSize"] = board->getFontSize();
    boardObj["hiddenArchived"] = board->getHiddenArchived();
    boardObj["windowX"] = board->getWindowX();
    boardObj["windowY"] = board->getWindowY();
    boardObj["windowWidth"] = board->getWindowWidth();
    boardObj["windowHeight"] = board->getWindowHeight();

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

    saveObject[board->getBid()] = boardObj;
  }

  saveObject["all"] = allArray;


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

  qDebug("hello end to save data......");

}


