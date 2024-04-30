#ifndef REMIND_H
#define REMIND_H

#include <QDateTime>
#include <QJsonObject>

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
    quint64 getDuration() const;
    bool isRepeatable() const;
    void setRepeatable(bool newRepeatable);


    static Remind      fromJson(const QJsonObject& object);
    static QJsonObject toJson(const Remind& r);

    void renew();


protected:
    QString name;
    QString description;
    QDateTime expires;
    quint64 duration;
    bool repeatable;
};

#endif // REMIND_H
