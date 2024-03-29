#ifndef STRIKE_H
#define STRIKE_H

#include <QObject>
#include <QDateTime>
#include "consts.h"

class Strike {
  //Q_OBJECT

public:
  Strike();
  Strike(const Strike &s2);
  void cloneStrike(Strike* strike);

  ~Strike();

  //任务完成状态
  enum StrikeStatus
  {
    NewAdd = 0,
    Working = 1,
    Done =2
  };


  QString getSid() const;
  void setSid(const QString &value);



  QString getTextColor() const;
  void setTextColor(const QString &value);

  QString getFontStyle() const;
  void setFontStyle(const QString &value);



  QDateTime getCreated() const;
  void setCreated(const QDateTime &value);

  QDateTime getUpdated() const;
  void setUpdated(const QDateTime &value);

  QString getDesc() const;
  void setDesc(const QString &value);


  int getStatus() const;
  void setStatus(int value);

private:

  QString sid; //唯一key
  QString desc; //任务名称
  int status = NewAdd; //任务状态

  QString textColor = "black"; //文字颜色
  QString fontStyle = ""; //文字特色

  QDateTime created = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //创建时间
  QDateTime updated = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //更新时间


};


Q_DECLARE_METATYPE(Strike::StrikeStatus);

#endif // STRIKE_H
