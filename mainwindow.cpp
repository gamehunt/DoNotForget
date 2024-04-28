#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <newreminddialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->tableView->setModel(&model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QObject::connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(updateButtonStates()));
    QObject::connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(removeReminds()));
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newRemind()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateButtonStates() {
    ui->actionRemove->setEnabled(ui->tableView->selectionModel()->hasSelection());
}

void MainWindow::newRemind() {
    NewRemindDialog diag;

    if(diag.exec() == QDialog::Accepted) {
        Remind result = diag.getRemindData();
        model.insertRows(model.rowCount(), 1);
        model.updateRemind(model.rowCount() - 1, result);
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
}
