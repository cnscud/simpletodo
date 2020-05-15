#include "helputils.h"
#include "consts.h"

#include <QString>
#include <QUuid>


HelpUtils::HelpUtils()
{
}

//生成一个UUID.
QString HelpUtils::uuid()
{
  QUuid id = QUuid::createUuid();
  QString strId = id.toString();
  return strId;
}

