#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QDateTime>
#include <strike.h>

class Board  {
  //Q_OBJECT


 public:
  Board();
  Board(bool fromScratch);

  ~Board();

  //QList<Strike*> items() const;

  //暂时没用
  //bool setItemAt(int index, const Strike &item);

  int size() const;

  void appendNewItem(QString &desc);
  void insertNewItem(int index, QString &desc);


  void insertItem(Strike* strike);
  void appendItem(Strike* strike);

  //setter & getter
  QString getBid() const;
  void setBid(const QString &value);


  bool getHidden() const;
  void setHidden(bool value);

  QString getBackColor() const;
  void setBackColor(const QString &value);

  bool getHiddenArchived() const;
  void setHiddenArchived(bool value);

  QDateTime getCreated() const;
  void setCreated(const QDateTime &value);

  QDateTime getUpdated() const;
  void setUpdated(const QDateTime &value);


  int getWindowX() const;
  void setWindowX(int value);

  int getWindowY() const;
  void setWindowY(int value);

  int getWindowWidth() const;
  void setWindowWidth(int value);

  int getWindowHeight() const;
  void setWindowHeight(int value);


  QString getTitle() const;
  void setTitle(const QString &value);

  int getFontSize() const;
  void setFontSize(int value);




  QList<Strike *> *getItems() const;
  void setItems(QList<Strike *> *items);

  QString getFontFamily() const;
  void setFontFamily(const QString &value);

private:
  QString bid; //唯一key: UUID
  QString title; //白板名称
  QString backColor = "#DAE362"; //背景色
  int fontSize = 18; //字体大小
  QString fontFamily = "Arial"; //字体类型

  bool hidden = false; //是否隐藏
  bool hiddenArchived = false; //是否隐藏归档的任务

  QDateTime created = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //创建时间
  QDateTime updated = QDateTime::fromString("2000-01-01 12:00:00", "yyyy-MM-dd hh:mm:ss"); //更新时间

  int windowX= 100; //窗口X坐标, 启动时要检查窗口, 不合适则自动重置
  int windowY = 100; //窗口Y坐标
  int windowWidth = 200; //窗口宽度
  int windowHeight = 400; //窗口高度

  QList<Strike*>* mItems = nullptr; //任务列表

};

#endif // BOARD_H
