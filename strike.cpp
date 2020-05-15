#include "strike.h"

Strike::Strike()
{

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

