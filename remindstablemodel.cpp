#include "remindstablemodel.h"
#include <chrono>
using namespace std::chrono;

#include <QDateTime>
#include <QDebug>
#include "util.h"

RemindsTableModel::RemindsTableModel(QObject *parent)
    : QAbstractTableModel{parent},
    updateTimer(new QTimer(this))
{
    updateTimer->setInterval(1000);
    connect(updateTimer, &QTimer::timeout , this, &RemindsTableModel::update);
    updateTimer->start();
}

void RemindsTableModel::removeRemind(int index) {
    removeRow(index);
}

void RemindsTableModel::updateRemind(int index, Remind remind) {
    reminds[index] = remind;
}

void RemindsTableModel::update() {
    int rows = rowCount();

    if(!rows) {
        return;
    }

    QModelIndex start = createIndex(0, 2);
    QModelIndex end   = createIndex(rows, 2);

    emit dataChanged(start, end, {Qt::DisplayRole});

    for(int i = 0; i < rows; i++) {
        if(reminds[i].getExpirationTime() <= QDateTime::currentDateTime()) {
            emit remindExpired(i, reminds[i]);
        }
    }
}

int RemindsTableModel::rowCount(const QModelIndex& parent) const {
    return reminds.size();
}

int RemindsTableModel::columnCount(const QModelIndex& parent) const {
    return 3;
}

Qt::ItemFlags RemindsTableModel::flags(const QModelIndex& index) const {
    return QAbstractTableModel::flags(index);
}

QVariant RemindsTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        Remind r = reminds.at(index.row());
        switch(index.column()) {
        case 0:
            return r.getName();
        case 1:
            return r.getDescription();
        case 2:
            {
            quint64 ms = QDateTime::currentDateTime().msecsTo(r.getExpirationTime());
            std::chrono::milliseconds milliseconds(ms);

            auto d = duration_cast<hours>(milliseconds).count() / 24;
            auto h = duration_cast<hours>(milliseconds).count() % 24;
            auto m = duration_cast<minutes>(milliseconds).count() % 60;
            auto s = duration_cast<seconds>(milliseconds).count() % 60;

            QStringList res;
            res << QString::number(d) + "d"
                << QString::number(h) + "h"
                << QString::number(m) + "m"
                << QString::number(s) + "s";

            return res.join(' ');
            }
        }
    }
    return QVariant();
}

QVariant RemindsTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString(tr("Name"));
        case 1:
            return QString(tr("Description"));
        case 2:
            return QString(tr("Time Left"));
        }
    }
    return QVariant();
}

bool RemindsTableModel::insertRows(int row, int count, const QModelIndex& parent) {
    emit beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++) {
        reminds.insert(row, Remind());
    }
    emit endInsertRows();
    return true;
}

bool RemindsTableModel::removeRows(int row, int count, const QModelIndex& parent) {
    int rc = rowCount();

    if(row > rc) {
        return false;
    } else if (row + count > rc) {
        count = row + count - rc;
    }

    emit beginRemoveRows(parent, row, row + count - 1);

    for(int i = 0; i < count; i++) {
        reminds.removeAt(row);
    }

    emit endRemoveRows();

    return true;
}
