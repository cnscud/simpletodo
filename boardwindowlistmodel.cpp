#include "boardwindowlistmodel.h"
#include "helputils.h"
#include "board.h"

#include <QDebug>

BoardWindowListModel::BoardWindowListModel(QObject *parent)
        : QAbstractListModel(parent) {
}

int BoardWindowListModel::rowCount(const QModelIndex &parent) const {
        if(parent.isValid())
                return 0;

        return m_boardModelProxys.size();
}

QVariant BoardWindowListModel::data(const QModelIndex &index, int role) const {
        if(!index.isValid())
                return QVariant();

        BoardModelProxy* boardProxy = m_boardModelProxys.at(index.row());
        Board* board = boardProxy->board();


        switch(role) {
                case BidRole:
                        return board->getBid();
                case TitleRole:
                        return board->getTitle();
                case HiddenRole:
                        return board->getHidden();
                case BackColorRole:
                        return board->getBackColor();
                case HiddenArchivedRole:
                        return board->getHiddenArchived();

                case CreatedRole:
                        return board->getCreated();
                case UpdatedRole:
                        return board->getUpdated();

                case WindowXRole:
                        return board->getWindowX();
                case WindowYRole:
                        return board->getWindowY();
                case WindowWidthRole:
                        return board->getWindowWidth();
                case WindowHeightRole:
                        return board->getWindowHeight();

                //Todo get ListModel for listview
                case StrikeListModelRole:
                       return QVariant::fromValue(boardProxy->strikeListModel());
        }


        return QVariant();
}

bool BoardWindowListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

        BoardModelProxy* boardProxy = m_boardModelProxys.at(index.row());
        Board* board = boardProxy->board();;

        if(data(index, role) != value) {

                // set value
                switch(role) {
                        case BidRole:
                                board->setBid(value.toString());
                                break;
                        case TitleRole:
                                board->setTitle(value.toString());
                                break;
                        case HiddenRole:
                                board->setHidden(value.toBool());
                                break;
                        case BackColorRole:
                                board->setBackColor(value.toString());
                                break;
                        case HiddenArchivedRole:
                                board->setHiddenArchived(value.toBool());
                                break;

                        case CreatedRole:
                                board->setCreated(value.toDateTime());
                                break;
                        case UpdatedRole:
                                board->setUpdated(value.toDateTime());
                                break;

                        case WindowXRole:
                                board->setWindowX(value.toInt());
                                break;
                        case WindowYRole:
                                board->setWindowY(value.toInt());
                                break;
                        case WindowWidthRole:
                                board->setWindowWidth(value.toInt());
                                break;
                        case WindowHeightRole:
                                board->setWindowHeight(value.toInt());
                                break;

                }


                //debug
                qDebug("[BoardWindowListModel]Data changed %d  value %ls", role, qUtf16Printable(value.toString()));
                //qDebug() << value;

                emit dataChanged(index, index, QVector<int>() << role);
                return true;
        }

        return false;
}

Qt::ItemFlags BoardWindowListModel::flags(const QModelIndex &index) const {
        if(!index.isValid())
                return Qt::NoItemFlags;

        return Qt::ItemIsEditable;
}

QHash<int, QByteArray> BoardWindowListModel::roleNames() const {
        QHash<int, QByteArray> names;
        names[BidRole] = "bid";
        names[TitleRole] = "title";

        names[HiddenRole] = "hidden";
        names[BackColorRole] = "backColor";
        names[HiddenArchivedRole] = "hiddenArchived";
        names[CreatedRole] = "created";
        names[UpdatedRole] = "updated";
        names[WindowXRole] = "windowX";
        names[WindowYRole] = "windowY";
        names[WindowWidthRole] = "windowWidth";
        names[WindowHeightRole] = "windowHeight";
        names[StrikeListModelRole] = "strikeListModel";

        return names;
}


bool BoardWindowListModel::insertRows(int row, int count, const QModelIndex &parent) {
        beginInsertRows(parent, row, row + count - 1);

        for(int i = 0; i < count; ++i) {

                Board* board = new Board();
                board->setBid(HelpUtils::uuid());

                QString newTitle =  "New Board " + QString::number(row + i);
                board->setTitle(newTitle);
                board->setCreated(QDateTime::currentDateTime());
                board->setUpdated(QDateTime::currentDateTime());

                BoardModelProxy* bmp = new BoardModelProxy();
                bmp->setBoard(board);

                m_boardModelProxys.insert(row, bmp);
        }


        endInsertRows();
        return true;
}

bool BoardWindowListModel::removeRows(int row, int count, const QModelIndex &parent) {
        beginRemoveRows(parent, row, row + count - 1);


        for(int i = 0; i < count; ++i) {
                m_boardModelProxys.removeAt(row + i);
        }

        endRemoveRows();
        return true;
}

QList<BoardModelProxy*> BoardWindowListModel::boardModelProxys() const {
        return m_boardModelProxys;
}

void BoardWindowListModel::setBoardModelProxys(const QList<BoardModelProxy*> &boardModelProxys) {
        beginResetModel();

        m_boardModelProxys = boardModelProxys;

        //Todo 参见trytodolist项目的 todomodel, 是否需要绑定事件?


        endResetModel();
}



