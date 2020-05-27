#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <QDateTime>


//用来保存所有数据的容器
class DataHolder
{
 public:
  DataHolder();

  QDateTime getLastBackupTime() const;
  void setLastBackupTime(const QDateTime &value);

  QDateTime getLastUpdateTime() const;
  void setLastUpdateTime(const QDateTime &value);

private:
  QDateTime lastUpdateTime = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //更新时间
  QDateTime lastBackupTime = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //备份时间

};

#endif // DATAHOLDER_H
