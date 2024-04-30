#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "remindstablemodel.h"

#include <QMainWindow>
#include <QFile>
#include <QSettings>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initSettings();
    void loadReminds();
    void saveReminds();

    void closeEvent(QCloseEvent *event) override;

public slots:
    void updateButtonStates();

    void newRemind();
    void removeReminds();
    void handleExpiredRemind(int index, Remind r);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();

private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;

    QAction* quitAction;
    QAction* restoreAction;

    QFile* remindsStorage;
    QSettings* settings;
    RemindsTableModel model;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
