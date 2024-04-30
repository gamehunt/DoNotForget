#include "remind.h"

#define TIME_FORMAT "dd.MM.yyyy hh:mm:ss"

Remind::Remind() {
    name = "<blank>";
    description = "";
    expires = QDateTime(QDate(31, 12, 9999), QTime(23, 59));
}

Remind::Remind(QString n, QString desc, QDateTime exp){
    name = n;
    description = desc;
    duration = QDateTime::currentDateTime().msecsTo(exp);
    expires = exp;
    repeatable = false;
}

Remind::Remind(QString n, QString desc, quint64 dur){
    name = n;
    description = desc;
    duration = dur;
    expires = QDateTime::currentDateTime().addMSecs(dur);
    repeatable = false;
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

quint64 Remind::getDuration() const
{
    return duration;
}

bool Remind::isRepeatable() const
{
    return repeatable;
}

void Remind::setRepeatable(bool newRepeatable)
{
    repeatable = newRepeatable;
}

Remind Remind::fromJson(const QJsonObject& object){
    return Remind(object["name"].toString(),
                  object["description"].toString(),
                  QDateTime::fromString(object["expires"].toString(), TIME_FORMAT));
}

QJsonObject Remind::toJson(const Remind& r){
    QJsonObject object;
    object["name"] = r.name;
    object["description"] = r.description;
    object["expires"] = r.expires.toString(TIME_FORMAT);
    return object;
}

void Remind::renew() {
    expires = QDateTime::currentDateTime().addMSecs(duration);
}

