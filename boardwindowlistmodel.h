#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include "board.h"
#include "boardmodelproxy.h"
#include "strikelistmodel.h"

#include <QAbstractListModel>

class BoardWindowListModel : public QAbstractListModel
{
  Q_OBJECT

 public:
  explicit BoardWindowListModel(QObject *parent = nullptr);


  //不要留空行: 有时候莫名其妙出错
  enum {
    BidRole,
    TitleRole,
    HiddenRole,
    BackColorRole,
    HiddenArchivedRole,
    CreatedRole,
    UpdatedRole,
    WindowXRole,
    WindowYRole,
    WindowWidthRole,
    WindowHeightRole,
    FontSizeRole,
    FontFamilyRole,
    StrikeListModelRole, //特殊, 是ListModel类型的, 不是普通QList
  };


  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QHash<int, QByteArray> roleNames() const override;


  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


  //新增一个白板
  Q_INVOKABLE bool addBoard();
  Q_INVOKABLE bool removeBoard(int index);


  QList<BoardModelProxy *> *boardModelProxys() const;
  //void setBoardModelProxys(QList<BoardModelProxy *> *boardModelProxys);

  QList<Board *> *boards() const;
  //void setBoards(QList<Board *> *boards);

  void setBoardsAndProxy(QList<Board *> *boards);

private:
  QList<Board*> *m_boards;
  //Board Proxy Model
  QList<BoardModelProxy*> *m_boardModelProxys;


};

#endif // BOARDMODEL_H
