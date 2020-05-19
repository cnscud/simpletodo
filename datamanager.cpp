#include "datamanager.h"

DataManager::DataManager()
{

}

void DataManager::windowModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  Q_UNUSED(roles);

  qDebug("[windowModelDataChanged] row %d to row %d" , topLeft.row(), bottomRight.row());

  //Todo Save Model Data

}

void DataManager::windowModelRowsInserted(const QModelIndex &parent, int first, int last)
{
  Q_UNUSED(parent);
  qDebug("[windowModelRowsInserted] first %d to last %d" , first, last);

}

void DataManager::windowModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
  Q_UNUSED(parent);
  Q_UNUSED(destination);

  qDebug("[windowModelRowsMoved] start %d to end %d, to row %d" , start, end, row);

}

void DataManager::windowModelRowsRemoved(const QModelIndex &parent, int first, int last)
{
  Q_UNUSED(parent);

  qDebug("[windowModelRowsRemoved] first %d to last %d" , first, last);

}

void DataManager::strikeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  Q_UNUSED(roles);
  qDebug("[strikeModelDataChanged] datachanged row %d to row %d" , topLeft.row(), bottomRight.row());

}

void DataManager::strikeModelRowsInserted(const QModelIndex &parent, int first, int last)
{
  Q_UNUSED(parent);
  qDebug("[strikeModelRowsInserted] first %d to last %d" , first, last);

}

void DataManager::strikeModelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
  Q_UNUSED(parent);
  Q_UNUSED(destination);

  qDebug("[strikeModelRowsMoved] start %d to end %d, to row %d" , start, end, row);

}

void DataManager::strikeModelRowsRemoved(const QModelIndex &parent, int first, int last)
{
  Q_UNUSED(parent);

  qDebug("[strikeModelRowsRemoved] first %d to last %d" , first, last);

}
