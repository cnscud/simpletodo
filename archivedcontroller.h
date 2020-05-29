#ifndef ARCHIVEDCONTROLLER_H
#define ARCHIVEDCONTROLLER_H

#include "datamanager.h"

#include <QObject>


//归档窗口控制辅助类
class ArchivedController: public QObject
{
  Q_OBJECT

 public:
  ArchivedController();


  void setDataManager(DataManager *dataManager);

public slots:
  void viewArchivedStrikes(QString bid);


  private:
   DataManager* m_dataManager;
};

#endif // ARCHIVEDCONTROLLER_H
