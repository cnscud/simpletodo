#include "strikelistmodel.h"
#include "helputils.h"

StrikeListModel::StrikeListModel(QObject *parent)
        : QAbstractListModel(parent) {
}

int StrikeListModel::rowCount(const QModelIndex &parent) const {
        // For list models only the root node (an invalid parent) should return the list's size. For all
        // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
        if(parent.isValid())
                return 0;

        return m_strikes.size();
}

QVariant StrikeListModel::data(const QModelIndex &index, int role) const {
        if(!index.isValid())
                return QVariant();

        Strike* strike = m_strikes.at(index.row());

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
                Strike* strike = m_strikes.at(index.row());

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
                qDebug("[StrikeListModel]Data changed %d value: %ls", role, qUtf16Printable(value.toString()));

                emit dataChanged(index, index, QVector<int>() << role);
                return true;
        }
        return false;
}

Qt::ItemFlags StrikeListModel::flags(const QModelIndex &index) const {
        if(!index.isValid())
                return Qt::NoItemFlags;

        //Todo 看看效果 Selectable
        return Qt::ItemIsEditable | Qt::ItemIsSelectable;
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

                m_strikes.insert(row, strike);
        }

        endInsertRows();
        return true;
}

bool StrikeListModel::removeRows(int row, int count, const QModelIndex &parent) {
        beginRemoveRows(parent, row, row + count - 1);

        for(int i = 0; i < count; ++i) {
                m_strikes.removeAt(row + i);
        }

        endRemoveRows();
        return true;
}

bool StrikeListModel::newStrike() {
        insertRows(0, 1, QModelIndex());

        QModelIndex qi = index(0, 0);
        setData(qi, HelpUtils::uuid(), SidRole);
        setData(qi, "New Task" + QString::number(m_strikes.size() + 1), DescRole);
        setData(qi, Strike::NewAdd, StatusRole);
        setData(qi, QDateTime::currentDateTime(), CreatedRole);
        setData(qi, QDateTime::currentDateTime(), UpdatedRole);

        return true;
}

QList<Strike*> StrikeListModel::strikes() const {
        return m_strikes;
}

void StrikeListModel::setStrikes(const QList<Strike*> &strikes) {
        beginResetModel();

        m_strikes = strikes;


        endResetModel();

}
