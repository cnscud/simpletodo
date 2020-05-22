#include "strikelistmodel.h"
#include "helputils.h"

StrikeListModel::StrikeListModel(QObject *parent)
  : QAbstractListModel(parent) {
}

int StrikeListModel::rowCount(const QModelIndex &parent) const {
        if(parent.isValid())
                return 0;

        return board->getItems()->size();
}

QVariant StrikeListModel::data(const QModelIndex &index, int role) const {
        if(!index.isValid())
                return QVariant();

        Strike* strike = board->getItems()->at(index.row());

        switch(role) {
                case SidRole:
                        return strike->getSid();
                case DescRole:
                        return strike->getDesc();
                case StatusRole:
                        return strike->getStatus();
                case  TextColorRole:
                        return strike->getTextColor();
                case FontStyleRole:
                        return strike->getFontStyle();
                case CreatedRole:
                        return strike->getCreated();
                case UpdatedRole:
                        return strike->getUpdated();
        }

        return QVariant();
}

bool StrikeListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

        if(data(index, role) != value) {
          Strike* strike = board->getItems()->at(index.row());

                //总是更新最新更新时间
                strike->setUpdated(QDateTime::currentDateTime());

                switch(role) {
                        case SidRole:
                                strike->setSid(value.toString());
                                break;
                        case DescRole:
                                strike->setDesc(value.toString());
                                break;
                        case StatusRole:
                                strike->setStatus(value.toInt());
                                break;
                        case  TextColorRole:
                                strike->setTextColor(value.toString());
                                break;
                        case FontStyleRole:
                                strike->setFontStyle(value.toString());
                                break;
                        case CreatedRole:
                                strike->setCreated(value.toDateTime());
                                break;
                        case UpdatedRole:
                                strike->setUpdated(value.toDateTime());
                                break;
                }

                //debug
                QHash<int, QByteArray> names = roleNames();
                QString roleName = QString::fromUtf8(names[role]);

                qDebug("[StrikeListModel]Data changed %s  value %s", qUtf8Printable(roleName), qUtf8Printable(value.toString()));

                emit dataChanged(index, index, QVector<int>() << role);
                return true;
        }
        return false;
}

Qt::ItemFlags StrikeListModel::flags(const QModelIndex &index) const {
        if(!index.isValid())
                return Qt::NoItemFlags;

        return Qt::ItemIsEditable ;
}

QHash<int, QByteArray> StrikeListModel::roleNames() const {
        QHash<int, QByteArray> names;
        names[SidRole] = "sid";
        names[DescRole] = "desc";

        names[StatusRole] = "status";
        names[TextColorRole] = "textColor";
        names[FontStyleRole] = "fontStyle";
        names[CreatedRole] = "created";
        names[UpdatedRole] = "updated";

        return names;
}

bool StrikeListModel::insertRows(int row, int count, const QModelIndex &parent) {
        beginInsertRows(parent, row, row + count - 1);

        for(int i = 0; i < count; ++i) {
                Strike* strike = new Strike();
                strike->setSid(HelpUtils::uuid());
                strike->setDesc("New Task" + QString::number(board->getItems()->size() + 1));
                strike->setStatus(Strike::NewAdd);
                strike->setCreated(QDateTime::currentDateTime());
                strike->setUpdated(QDateTime::currentDateTime());

                board->getItems()->insert(row, strike);
        }

        endInsertRows();
        return true;
}

bool StrikeListModel::removeRows(int row, int count, const QModelIndex &parent) {
        beginRemoveRows(parent, row, row + count - 1);

        for(int i = 0; i < count; ++i) {
          //销毁对象
          delete board->getItems()->at(row +i);
          board->getItems()->removeAt(row + i);
        }

        endRemoveRows();
        return true;
}

bool StrikeListModel::moveRow(int sourceRow, int destRow) {
        if(sourceRow == destRow) {
                return false;
        }

        int oldDestRow = destRow;
        qDebug("======moveRow --- sourceRow: %d destRow: %d", sourceRow, oldDestRow);


        //向下移动有问题 moveRow 需要加工
        //see https://forum.qt.io/topic/95879/endmoverows-in-model-crashes-my-app
        //see https://www.qtcentre.org/threads/43640-beginMoveRows-working-down-to-up-but-not-up-to-down-Any-insignt-would-be-great
        if(sourceRow < destRow){
          destRow++;
        }
        if(destRow > rowCount()){
          destRow = rowCount();
        }


        if(!beginMoveRows(QModelIndex(), sourceRow, sourceRow, QModelIndex(), destRow))
                return false;

        //数据移动
        board->getItems()->move(sourceRow, oldDestRow);
        qDebug("======Done moveRow --- sourceRow: %d destRow: %d ->lastDestRow: %d", sourceRow, oldDestRow, destRow);

        endMoveRows();

        return true;
}

bool StrikeListModel::addStrike() {
        insertRows(0, 1, QModelIndex());

        //监听了rowsInserted事件, 所以不单独setData了
        /*
        QModelIndex qi = index(0, 0);
        setData(qi, HelpUtils::uuid(), SidRole);
        setData(qi, "New Task" + QString::number(m_strikes.size() + 1), DescRole);
        setData(qi, Strike::NewAdd, StatusRole);
        setData(qi, QDateTime::currentDateTime(), CreatedRole);
        setData(qi, QDateTime::currentDateTime(), UpdatedRole);
        */

        return true;
}

bool StrikeListModel::removeStrike(int index) {

        removeRows(index, 1, QModelIndex());
        return true;
}

bool StrikeListModel::archivedStrike(int index)
{
  Strike* strike = board->getItems()->at(index);
  Strike destStrike(*strike);

  //通知
  emit strikeArchived(board->getBid(), destStrike);

  //移除
  removeRows(index, 1, QModelIndex());

  return true;
}


Board *StrikeListModel::getBoard() const
{
  return board;
}

void StrikeListModel::setBoard(Board *value)
{
  beginResetModel();

  board = value;

  endResetModel();

}
