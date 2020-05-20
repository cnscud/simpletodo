#include "boardmodelproxy.h"

BoardModelProxy::BoardModelProxy()
{

}

Board *BoardModelProxy::board() const
{
  return m_board;
}

void BoardModelProxy::setBoard(Board *board)
{
  m_board = board;
}

StrikeListModel *BoardModelProxy::strikeListModel() const
{
  return m_strikeListModel;
}

void BoardModelProxy::setStrikeListModel(StrikeListModel *strikeListModel)
{
  m_strikeListModel = strikeListModel;
}


