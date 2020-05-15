#ifndef STRIKEMODEL_H
#define STRIKEMODEL_H

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

  // 在QML里直接调用来生成一个新行
  Q_INVOKABLE bool newStrike();



  QList<Strike*> strikes() const;
  void setStrikes(const QList<Strike*> &strikes);

private:
  QList<Strike*> m_strikes;

};

#endif // STRIKEMODEL_H