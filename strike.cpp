#include "strike.h"
#include <QtDebug>

Strike::Strike()
{

}

Strike::Strike(const Strike &s2)
{
  this->sid = s2.sid;
  this->desc = s2.desc;
  this->status = s2.status;
  this->textColor = s2.textColor;
  this->fontStyle = s2.fontStyle;
  this->created = s2.created;
  this->updated = s2.updated;
}


Strike::~Strike()
{
  qDebug() << "destructor strike  " << sid << "," << desc << ", " <<this;
}


void Strike::cloneStrike(Strike *s2)
{
  sid = s2->getSid();
  desc = s2->getDesc();
  status = s2->getStatus();
  textColor = s2->getTextColor();
  fontStyle = s2->getFontStyle();
  created = s2->getCreated();
  updated = s2->getUpdated();
}

QString Strike::getSid() const
{
    return sid;
}

void Strike::setSid(const QString &value)
{
    sid = value;
}

QString Strike::getTextColor() const
{
    return textColor;
}

void Strike::setTextColor(const QString &value)
{
    textColor = value;
}

QString Strike::getFontStyle() const
{
    return fontStyle;
}

void Strike::setFontStyle(const QString &value)
{
    fontStyle = value;
}

QDateTime Strike::getCreated() const
{
    return created;
}

void Strike::setCreated(const QDateTime &value)
{
    created = value;
}

QDateTime Strike::getUpdated() const
{
    return updated;
}

void Strike::setUpdated(const QDateTime &value)
{
    updated = value;
}

QString Strike::getDesc() const
{
    return desc;
}

void Strike::setDesc(const QString &value)
{
    desc = value;
}

int Strike::getStatus() const
{
  return status;
}

void Strike::setStatus(int value)
{
  status = value;
}

