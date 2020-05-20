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
    StrikeListModelRole, //特殊, 是ListModel类型的, 不是普通QList
  };


  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QHash<int, QByteArray> roleNames() const override;



  // Add data:
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

  // Remove data:
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;



  QList<BoardModelProxy*> boardModelProxys() const;
  void setBoardModelProxys(const QList<BoardModelProxy*> &boardModelProxys);


private:
  //Board Proxy Model
  QList<BoardModelProxy*> m_boardModelProxys;


};

#endif // BOARDMODEL_H
