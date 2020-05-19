#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QModelIndex>



class DataManager : public QObject
{
 public:
  DataManager();


  //白板列表模型数据变化
  void windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

  void  windowModelRowsInserted(const QModelIndex &parent, int first, int last);
  void  windowModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
  void  windowModelRowsRemoved(const QModelIndex &parent, int first, int last);


  //任务列表模型数据变化
  void strikeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
  void strikeModelRowsInserted(const QModelIndex &parent, int first, int last);
  void strikeModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
  void strikeModelRowsRemoved(const QModelIndex &parent, int first, int last);

};

#endif // DATAMANAGER_H
