#include "board.h"

#include "consts.h"
#include <QUuid>

Board::Board() {

}


/*
bool Board::setItemAt(int index, const Strike &item) {
        if(index < 0 || index >= mItems.size()) {
                return false;
        }

        const Strike &oldItem = mItems.at(index);
        //目前只判断了主要信息
        if(item.getSid() == oldItem.getSid() && item.getDesc() == oldItem.getDesc()) {
                return false;
        }

        mItems[index] = item;
        return true;
}
*/

int Board::size() const {
        return mItems.size();
}

void Board::appendNewItem(QString &desc) {
        Strike strike;

        //uuid
        QUuid id = QUuid::createUuid();
        QString strId = id.toString();

        strike.setSid(strId);
        strike.setDesc(desc);
        strike.setStatus(Strike::NewAdd);
        strike.setCreated(QDateTime::currentDateTime());
        strike.setUpdated(QDateTime::currentDateTime());

        mItems.append(&strike);
}

void Board::insertNewItem(int index, QString &desc) {
        Strike strike;

        //uuid
        QUuid id = QUuid::createUuid();
        QString strId = id.toString();

        strike.setSid(strId);
        strike.setDesc(desc);
        strike.setStatus(Strike::NewAdd);
        strike.setCreated(QDateTime::currentDateTime());
        strike.setUpdated(QDateTime::currentDateTime());

        mItems.insert(index, &strike);
}


QString Board::getBid() const {
        return bid;
}

void Board::setBid(const QString &value) {
        bid = value;
}


bool Board::getHidden() const {
        return hidden;
}

void Board::setHidden(bool value) {
        hidden = value;
}

QString Board::getBackColor() const {
        return backColor;
}

void Board::setBackColor(const QString &value) {
        backColor = value;
}

bool Board::getHiddenArchived() const {
        return hiddenArchived;
}

void Board::setHiddenArchived(bool value) {
        hiddenArchived = value;
}

int Board::getWindowX() const {
        return windowX;
}

void Board::setWindowX(int value) {
        windowX = value;
}

int Board::getWindowY() const {
        return windowY;
}

void Board::setWindowY(int value) {
        windowY = value;
}

int Board::getWindowWidth() const {
        return windowWidth;
}

void Board::setWindowWidth(int value) {
        windowWidth = value;
}

int Board::getWindowHeight() const {
        return windowHeight;
}

void Board::setWindowHeight(int value) {
        windowHeight = value;
}

QString Board::getTitle() const
{
    return title;
}

void Board::setTitle(const QString &value)
{
    title = value;
}

int Board::getFontSize() const
{
  return fontSize;
}

void Board::setFontSize(int value)
{
  fontSize = value;
}

QList<Strike *> Board::getItems() const
{
  return mItems;
}

void Board::setItems(const QList<Strike *> &items)
{
  mItems = items;
}

QDateTime Board::getCreated() const {
  return created;
}

void Board::setCreated(const QDateTime &value) {
  created = value;
}

QDateTime Board::getUpdated() const {
        return updated;
}

void Board::setUpdated(const QDateTime &value) {
        updated = value;
}
