#ifndef BOARDMODELPROXY_H
#define BOARDMODELPROXY_H

#include "board.h"
#include "strikelistmodel.h"

//代理 Board: 为了区分Board基础数据和ListModel用的

class BoardModelProxy: public QObject
{
  Q_OBJECT

 public:
  BoardModelProxy();

  Board *board() const;
  void setBoard(Board *board);

  StrikeListModel *strikeListModel() const;
  void setStrikeListModel(StrikeListModel *strikeListModel);

 Q_SIGNALS:
  void strikeListModelChanged();

private:


  Board* m_board; //被代理的Board

  StrikeListModel* m_strikeListModel; //视图中ListView需要的ListModel: 任务列表

};

#endif // BOARDMODELPROXY_H
