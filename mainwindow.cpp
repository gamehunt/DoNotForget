#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>
#include <QEvent>
#include <QCloseEvent>

#include <newreminddialog.h>
#include <remindexpireddialog.h>
#include <util.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->tableView->setModel(&model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    trayMenu = new QMenu(this);
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icon.ico"));
    trayIcon->setToolTip("Test");
    trayIcon->setContextMenu(trayMenu);

    setWindowIcon(QIcon(":/icons/icon.ico"));

    settings = new QSettings("DoNotForget", "config");

    QObject::connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(updateButtonStates()));
    QObject::connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(removeReminds()));
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newRemind()));

    QObject::connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &MainWindow::messageClicked);
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    QObject::connect(&model, SIGNAL(remindExpired(int, Remind)), this, SLOT(handleExpiredRemind(int, Remind)));

    initSettings();
    loadReminds();

    trayIcon->show();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {

}

void MainWindow::showMessage() {

}

void MainWindow::messageClicked() {

}

MainWindow::~MainWindow() {
    remindsStorage->close();
    delete ui;
}

void MainWindow::initSettings() {
    if(!settings->contains("remindsPath")) {
        QDir d = QDir(settings->fileName());
        d.cdUp();
        settings->setValue("remindsPath", d.path() + QDir::separator() + "reminds.json");
    }

    QString path = settings->value("remindsPath").toString();
    remindsStorage = new QFile(path);

    bool created = !remindsStorage->exists();
    if(!remindsStorage->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Error", "Failed to open reminds storage.");
        return;
    }

    if(created) {
        remindsStorage->write(QJsonDocument(QJsonObject()).toJson());
        remindsStorage->flush();
    }
}

void MainWindow::loadReminds() {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(remindsStorage->readAll(), &error);

    if(error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "Error", "Failed to parse reminds storage: " + error.errorString());
        return;
    }

    QJsonArray remindsArray = doc.array();
    for(int i = 0; i < remindsArray.size(); i++) {
        QJsonObject obj = remindsArray.at(i).toObject();
        Remind r = Remind::fromJson(obj);
        model.appendRemind(r);
    }

}

void MainWindow::saveReminds() {
    QJsonArray array;

    QList<Remind> reminds = model.getReminds();

    for(int i = 0; i < model.rowCount(); i++) {
        array.append(Remind::toJson(reminds[i]));
    }

    remindsStorage->resize(0);
    remindsStorage->write(QJsonDocument(array).toJson());
    remindsStorage->flush();
}

void MainWindow::updateButtonStates() {
    ui->actionRemove->setEnabled(ui->tableView->selectionModel()->hasSelection());
}

void MainWindow::newRemind() {
    NewRemindDialog diag;

    if(diag.exec() == QDialog::Accepted) {
        Remind result = diag.getRemindData();
        model.appendRemind(result);
        saveReminds();
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    saveReminds();
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::removeReminds() {
    QItemSelectionModel* sm = ui->tableView->selectionModel();

    if(sm->hasSelection()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Are you sure?"),
                                      QString("Remove %1 reminds?")
                                          .arg(sm->selectedRows().size()),
                                      QMessageBox::Yes|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    while(sm->hasSelection()) {
        QModelIndex index = sm->selectedIndexes().first();
        model.removeRow(index.row());
    }

    saveReminds();
}

void MainWindow::handleExpiredRemind(int index, Remind r){
    beep();

    RemindExpiredDialog diag(this, r);

    if(diag.exec() == QDialog::Accepted) {
        r.renew();
        model.updateRemind(index, r);
    } else {
        model.removeRemind(index);
    }


    saveReminds();
}
