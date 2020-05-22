#include "boardwindowlistmodel.h"
#include "helputils.h"
#include "board.h"

#include <QDebug>

BoardWindowListModel::BoardWindowListModel(QObject *parent)
  : QAbstractListModel(parent) {
}

BoardWindowListModel::~BoardWindowListModel()
{
  qDeleteAll(m_boardModelProxys->begin(), m_boardModelProxys->end());
  delete m_boardModelProxys;
}

int BoardWindowListModel::rowCount(const QModelIndex &parent) const {
        if(parent.isValid())
                return 0;

        return m_boardModelProxys->size();
}

QVariant BoardWindowListModel::data(const QModelIndex &index, int role) const {
        if(!index.isValid())
                return QVariant();

        BoardModelProxy* boardProxy = m_boardModelProxys->at(index.row());
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
                case FontSizeRole:
                        return board->getFontSize();
                case FontFamilyRole:
                        return board->getFontFamily();
                //get ListModel for listview
                case StrikeListModelRole:
                        return QVariant::fromValue(boardProxy->strikeListModel());
        }


        return QVariant();
}

bool BoardWindowListModel::setData(const QModelIndex &index, const QVariant &value, int role) {


        if(data(index, role) != value) {

                BoardModelProxy* boardProxy = m_boardModelProxys->at(index.row());
                Board* board = boardProxy->board();;

                //总是更新为最新时间
                board->setUpdated(QDateTime::currentDateTime());

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
                        case FontSizeRole:
                                board->setFontSize(value.toInt());
                                break;
                        case FontFamilyRole:
                          board->setFontFamily(value.toString());
                          break;

                }


                //debug
                QHash<int, QByteArray> names = roleNames();
                QString roleName = QString::fromUtf8(names[role]);

                qDebug("[BoardWindowListModel]Data changed %s  value %s", qUtf8Printable(roleName), qUtf8Printable(value.toString()));

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
        names[FontSizeRole] = "fontSize";
        names[FontFamilyRole] = "fontFamily";

        names[StrikeListModelRole] = "strikeListModel";

        return names;
}


bool BoardWindowListModel::insertRows(int row, int count, const QModelIndex &parent) {
        beginInsertRows(parent, row, row + count - 1);

        for(int i = 0; i < count; ++i) {

                Board* board = new Board();
                board->setBid(HelpUtils::uuid());

                QString newTitle =  "New Board " + QString::number(m_boardModelProxys->size() + i + 1);
                board->setTitle(newTitle);
                board->setCreated(QDateTime::currentDateTime());
                board->setUpdated(QDateTime::currentDateTime());

                QList<Strike*>* items = new QList<Strike*>();

                Strike* strike = new Strike();
                strike->setSid(HelpUtils::uuid());
                strike->setDesc("New Task");
                strike->setStatus(Strike::NewAdd);
                strike->setCreated(QDateTime::currentDateTime());
                strike->setUpdated(QDateTime::currentDateTime());

                items->append(strike);

                board->setItems(items);

                StrikeListModel* strikeListModel = new StrikeListModel();
                strikeListModel->setBoard(board);

                BoardModelProxy* bmp = new BoardModelProxy();
                bmp->setBoard(board);
                bmp->setStrikeListModel(strikeListModel);

                //同时设置
                m_boards->insert(row, board);
                m_boardModelProxys->insert(row, bmp);
        }


        endInsertRows();
        return true;
}

bool BoardWindowListModel::removeRows(int row, int count, const QModelIndex &parent) {
        beginRemoveRows(parent, row, row + count - 1);


        for(int i = 0; i < count; ++i) {

                BoardModelProxy* boardProxy = m_boardModelProxys->at(row);
                StrikeListModel* slm = boardProxy->strikeListModel();
                //是否需要哪???
                slm->disconnect();

                //销毁对象: 是否需要手工销毁关联的子对象哪?
                delete m_boardModelProxys->at(row + i);
                delete m_boards->at(row + i);

                m_boardModelProxys->removeAt(row + i);
                m_boards->removeAt(row + i);
        }


        endRemoveRows();
        return true;
}

bool BoardWindowListModel::addBoard() {
        qDebug("add Board");
        insertRows(0, 1, QModelIndex());
        qDebug("end add Board");

        return true;
}

bool BoardWindowListModel::removeBoard(int index) {
        qDebug("remove Board....%d ", index);
        removeRows(index, 1, QModelIndex());
        qDebug("remove Board.... end");

        return true;
}


QList<BoardModelProxy *> *BoardWindowListModel::boardModelProxys() const {
        return m_boardModelProxys;
}


QList<Board *> *BoardWindowListModel::boards() const {
        return m_boards;
}

void BoardWindowListModel::setBoardsAndProxy(QList<Board *> *boards) {
        beginResetModel();

        //初始化为了ListView需要的Model
        QList<BoardModelProxy*>* boardProxyList = new QList<BoardModelProxy*>();
        for(int i = 0; i < boards->size(); ++i) {
                Board* board = boards->at(i);
                BoardModelProxy* bmproxy = new BoardModelProxy();
                bmproxy->setBoard(board);

                StrikeListModel* strikeListModel = new StrikeListModel();
                strikeListModel->setBoard(board);

                bmproxy->setStrikeListModel(strikeListModel);

                //add to list
                boardProxyList->append(bmproxy);
        }

        m_boards = boards;
        m_boardModelProxys = boardProxyList;

        endResetModel();
}




