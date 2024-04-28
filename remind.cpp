#include "remind.h"

Remind::Remind() {
    name = "<blank>";
    description = "";
    expires = QDateTime(QDate(31, 12, 9999), QTime(23, 59));
}

Remind::Remind(QString n, QString desc, QDateTime exp){
    name = n;
    description = desc;
    expires = exp;
}

Remind::Remind(QString n, QString desc, quint64 dur){
    name = n;
    description = desc;
    expires = QDateTime::currentDateTime().addMSecs(dur);
}

QString Remind::getName() const
{
    return name;
}

void Remind::setName(const QString &newName)
{
    name = newName;
}

QString Remind::getDescription() const
{
    return description;
}

void Remind::setDescription(const QString &newDescription)
{
    description = newDescription;
}

QDateTime Remind::getExpirationTime() const
{
    return expires;
}

void Remind::setExpirationTime(const QDateTime &newExpires)
{
    expires = newExpires;
}
