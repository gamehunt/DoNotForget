#ifndef REMIND_H
#define REMIND_H

#include <QDateTime>


class Remind
{
public:
    Remind();
    Remind(QString name, QString desc, QDateTime expires);
    Remind(QString name, QString desc, quint64 duration);

    QString getName() const;
    void setName(const QString &newName);
    QString getDescription() const;
    void setDescription(const QString &newDescription);
    QDateTime getExpirationTime() const;
    void setExpirationTime(const QDateTime &newExpires);

protected:
    QString name;
    QString description;
    QDateTime expires;
};

#endif // REMIND_H
