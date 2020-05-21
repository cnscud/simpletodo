#include "mockhelper.h"
#include "board.h"
#include "strikelistmodel.h"
#include "boardwindowlistmodel.h"
#include "datamanager.h"

MockHelper::MockHelper()
{

}


BoardWindowListModel* MockHelper::mockData()
{
  qDebug("start mock data......");

  Board* board1 = new Board();
  board1->setBid("b1111");
  board1->setTitle("Board Todo 1111");

  Board* board2 = new Board();
  board2->setBid("b222");
  board2->setTitle("Board Todo 2222");


  Strike strike11;
  strike11.setSid("111-111");
  strike11.setDesc("Working at home");

  Strike strike12;
  strike12.setSid("1111-222");
  strike12.setDesc("Clean the room");

  QList<Strike*> strikeList1;
  strikeList1 << &strike11 << &strike12;

  StrikeListModel* strikeListModel1 = new StrikeListModel();
  strikeListModel1->setBoard(board1);


  Strike strike21;
  strike21.setSid("222-222");
  strike21.setDesc("Wash my car");

  Strike strike22;
  strike22.setSid("222-333");
  strike22.setDesc("Shopping for apple");

  QList<Strike*> strikeList2;
  strikeList2 << &strike21 << &strike22;

  StrikeListModel* strikeListModel2 = new StrikeListModel();
  strikeListModel2->setBoard(board2);


  /*
  BoardModelProxy* bmp1 = new BoardModelProxy();
  bmp1->setBoard(board1);
  bmp1->setStrikeListModel(strikeListModel1);

  BoardModelProxy* bmp2 = new BoardModelProxy();
  bmp2->setBoard(board2);
  bmp2->setStrikeListModel(strikeListModel2);

  QList<BoardModelProxy*> boardProxyList;
  boardProxyList.append(bmp1);
  boardProxyList.append(bmp2);
  */

  QList<Board*>* boards = new QList<Board*>();
  boards->append(board1);
  boards->append(board2);

  BoardWindowListModel* bwlm  = new BoardWindowListModel();
  bwlm->setBoardsAndProxy(boards);

  qDebug("end mock data......");

  return bwlm;
}
