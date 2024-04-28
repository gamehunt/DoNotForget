#ifndef UTIL_H
#define UTIL_H

#include <chrono>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

#define DURATION(days, hours, minutes, seconds) \
    ((quint64)seconds) * 1000                + \
    ((quint64)minutes) * 60 * 1000           + \
    ((quint64)hours)   * 60 * 60 * 1000      + \
    ((quint64)days) * 24 * 60 * 60 * 1000

#define SECONDS(ms) duration_cast<std::chrono::seconds>(std::chrono::milliseconds(ms)).count()
#define HOURS(ms)   duration_cast<std::chrono::hours>(std::chrono::milliseconds(ms)).count()

static void beep() {
    static QMediaPlayer* player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/beep.mp3"));
    player->setVolume(100);
    player->play();
}

#endif // UTIL_H
