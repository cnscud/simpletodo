#include "datamanager.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

DataManager::DataManager() {

}

void DataManager::windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(roles);

        qDebug("[windowModelDataChanged] row %d to row %d", topLeft.row(), bottomRight.row());

        //Todo Save Model Data

}

void DataManager::windowModelRowsInserted(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);
        qDebug("[windowModelRowsInserted] first %d to last %d", first, last);

}

void DataManager::windowModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
        Q_UNUSED(parent);
        Q_UNUSED(destination);

        qDebug("[windowModelRowsMoved] start %d to end %d, to row %d", start, end, row);

}

void DataManager::windowModelRowsRemoved(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);

        qDebug("[windowModelRowsRemoved] first %d to last %d", first, last);

}

void DataManager::strikeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(roles);
        qDebug("[strikeModelDataChanged] datachanged row %d to row %d", topLeft.row(), bottomRight.row());

}

void DataManager::strikeModelRowsInserted(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);
        qDebug("[strikeModelRowsInserted] first %d to last %d", first, last);

}

void DataManager::strikeModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
        Q_UNUSED(parent);
        Q_UNUSED(destination);

        qDebug("[strikeModelRowsMoved] start %d to end %d, to row %d", start, end, row);

}

void DataManager::strikeModelRowsRemoved(const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);

        qDebug("[strikeModelRowsRemoved] first %d to last %d", first, last);

}





QList<Board*> DataManager::readAllBoards() {
        QJsonDocument doc = readDataFromFile();

        //parse json
        QJsonObject json = doc.object();

        if(json.contains("all") && json["all"].isArray()) {
                QJsonArray allArray = json["all"].toArray();

                QList<Board*> todoLists;

                for(int n = 0; n < allArray.size(); ++n) {
                        QString listName = allArray[n].toString();

                        //读一个任务列表
                        Board* oneList = parseOneBoard(json, listName);

                        todoLists.append(oneList);
                }

                mBoards = todoLists; //
                return todoLists;
        }

        return QList<Board*>();
}


Board* DataManager::parseOneBoard(QJsonObject &json, QString &abbr) {
        // read one board from json
        Board* board = new Board();
        //todoBoard->setAbbr(abbr);

        if(!json.contains(abbr)) {
                return board;
        }

        //分析设置数据

        QJsonObject oneSection = json[abbr].toObject();

        QString title = oneSection["title"].toString();
        board->setTitle(title);

        board->setBid(oneSection["bid"].toString());
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

                QList<Strike*> items;

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


                        items.append(strike);
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




/*
void DataManager::saveData(const QModelIndex topLeft, const QModelIndex bottomRight) {
  //save data
  qDebug("hello save data");

QJsonObject saveObject;

QJsonArray allArray;
for(int n = 0; n < mBoards.size(); ++n) {
  ToDoBoard* board = mBoards.at(n);

  allArray.append(board->getAbbr());

  QJsonObject boardObj;

  QJsonArray boardItemsArray;
  QList<ToDoItem> items = board->items();
  for(int m = 0; m < items.count(); ++m) {
    ToDoItem item = items.at(m);
    QJsonObject itemObj;
    itemObj["name"] = item.description;
    itemObj["done"] = item.done ? "1" : "0";

    boardItemsArray.append(itemObj);
  }

  boardObj["name"] = board->getName();
  boardObj["list"] = boardItemsArray;


  saveObject[board->getAbbr()] = boardObj;
}

saveObject["all"] = allArray;


QString pathName = pickDataFilePathName();
QFile saveFile(pathName);

if(!saveFile.open(QIODevice::WriteOnly)) {
  qWarning("Couldn't open save file.");
  return;
}

QJsonDocument saveDoc(saveObject);
saveFile.write(saveDoc.toJson());
}
*/

