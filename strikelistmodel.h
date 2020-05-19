#ifndef STRIKEMODEL_H
#define STRIKEMODEL_H

#include "board.h"
#include "strike.h"

#include <QAbstractListModel>

class StrikeListModel : public QAbstractListModel
{
  Q_OBJECT

 public:
  explicit StrikeListModel(QObject *parent = nullptr);

  enum {
    SidRole,
    DescRole,
    StatusRole,
    TextColorRole,
    FontStyleRole,
    CreatedRole,
    UpdatedRole,
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

  Q_INVOKABLE bool moveRow(int sourceRow, int destRow);

  // 在QML里直接调用来生成一个新行
  Q_INVOKABLE bool addStrike();
  Q_INVOKABLE bool removeStrike(int index);


  QList<Strike*> strikes() const;
  void setStrikes(const QList<Strike*> &strikes);



  Board *getBoard() const;
  void setBoard(Board *value);

private:
  Board* board; //父节点
  QList<Strike*> m_strikes;

};

#endif // STRIKEMODEL_H
