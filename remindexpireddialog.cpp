#include "remindexpireddialog.h"
#include "ui_remindexpireddialog.h"

RemindExpiredDialog::RemindExpiredDialog(QWidget *parent, Remind r)
    : QDialog(parent)
    , ui(new Ui::RemindExpiredDialog)
{
    ui->setupUi(this);
    ui->label->setText("Remind expired: "
                       + r.getName()        + "\n\n"
                       + r.getDescription() + "\n");

    QObject::connect(ui->discardButton, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(ui->renewButton, SIGNAL(clicked()), this, SLOT(accept()));
}

RemindExpiredDialog::~RemindExpiredDialog()
{
    delete ui;
}
