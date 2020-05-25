#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "board.h"

#include <QModelIndex>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>

class DataManager : public QObject
{
 public:
  DataManager();
  const QString dataFileName = "simpletodo.json"; //文件名
  const QString archived_prefix = "archived_";


  //白板列表模型数据变化
  void  windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

  void  windowModelRowsInserted(const QModelIndex &parent, int first, int last);
  void  windowModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
  void  windowModelRowsRemoved(const QModelIndex &parent, int first, int last);


  //任务列表模型数据变化
  void strikeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
  void strikeModelRowsInserted(const QModelIndex &parent, int first, int last);
  void strikeModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
  void strikeModelRowsRemoved(const QModelIndex &parent, int first, int last);

  //归档数据
  void archivedStrike(QString bid, Strike &strike);

  //读取所有数据
  QList<Board*> *readAllBoards();


  //保存数据
  void doSaveData();
  void fireSaveData();


protected:
   QJsonObject transferBoardToJson(Board* board, bool archived);

private:
  QList<Board*> *m_boards = new QList<Board*>();
  QList<Board*> *m_archivedBoards = new QList<Board*>(); //归档的任务: 按原Board分类, 一一对应

  QString pickDataFilePathName();
  QJsonDocument readDataFromFile();
  Board* parseOneBoard(QJsonObject &json, QString &abbr, bool archived);


};

#endif // DATAMANAGER_H
