#ifndef REMINDSTABLEMODEL_H
#define REMINDSTABLEMODEL_H

#include "remind.h"
#include <QAbstractTableModel>
#include <QTimer>

class RemindsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RemindsTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void update();

public slots:
    void removeRemind(int index);
    void updateRemind(int index, Remind remind);

signals:
    void remindExpired(int index, Remind remind);

private:
    QTimer* updateTimer;
    QList<Remind> reminds;
};

#endif // REMINDSTABLEMODEL_H
