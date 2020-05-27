#include "dataholder.h"

DataHolder::DataHolder()
{

}

QDateTime DataHolder::getLastBackupTime() const
{
  return lastBackupTime;
}

void DataHolder::setLastBackupTime(const QDateTime &value)
{
  lastBackupTime = value;
}

QDateTime DataHolder::getLastUpdateTime() const
{
  return lastUpdateTime;
}

void DataHolder::setLastUpdateTime(const QDateTime &value)
{
  lastUpdateTime = value;
}
