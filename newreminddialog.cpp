#include "newreminddialog.h"
#include "ui_newreminddialog.h"

#include "util.h"
using namespace std::chrono;

NewRemindDialog::NewRemindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewRemindDialog)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    inDurationUpdate = false;
    inDateTimeUpdate = false;

    QObject::connect(ui->dateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(updateDuration()));
    QObject::connect(ui->syncPushButton, SIGNAL(clicked()), this, SLOT(sync()));

    QObject::connect(ui->durationDays, SIGNAL(valueChanged(int)), this, SLOT(updateDateTime()));
    QObject::connect(ui->durationHours, SIGNAL(valueChanged(int)), this, SLOT(updateDateTime()));
    QObject::connect(ui->durationMinutes, SIGNAL(valueChanged(int)), this, SLOT(updateDateTime()));
    QObject::connect(ui->durationSeconds, SIGNAL(valueChanged(int)), this, SLOT(updateDateTime()));
}

NewRemindDialog::~NewRemindDialog()
{
    delete ui;
}

void NewRemindDialog::sync() {
    inDateTimeUpdate = true;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addMSecs(
        DURATION(
            ui->durationDays->value(),
            ui->durationHours->value(),
            ui->durationMinutes->value(),
            ui->durationSeconds->value()
        )
    ));
    inDateTimeUpdate = false;
}

void NewRemindDialog::updateDuration() {
    if(inDateTimeUpdate) {
        return;
    }

    inDurationUpdate = true;

    quint64 ms = QDateTime::currentDateTime().msecsTo(ui->dateTimeEdit->dateTime());
    std::chrono::milliseconds milliseconds(ms);

    auto d = duration_cast<hours>(milliseconds).count() / 24;
    auto h = duration_cast<hours>(milliseconds).count() % 24;
    auto m = duration_cast<minutes>(milliseconds).count() % 60;
    auto s = duration_cast<seconds>(milliseconds).count() % 60;

    ui->durationDays->setValue(d);
    ui->durationHours->setValue(h);
    ui->durationMinutes->setValue(m);
    ui->durationSeconds->setValue(s);

    inDurationUpdate = false;
}

void NewRemindDialog::updateDateTime() {
    if(inDurationUpdate) {
        return;
    }

    inDateTimeUpdate = true;

    QDateTime dateTime = QDateTime::currentDateTime();

    ui->dateTimeEdit->setDateTime(dateTime.addMSecs(
        DURATION(
            ui->durationDays->value(),
            ui->durationHours->value(),
            ui->durationMinutes->value(),
            ui->durationSeconds->value()
        )
    ));

    inDateTimeUpdate = false;
}

Remind NewRemindDialog::getRemindData() {
    Remind r = Remind(ui->nameLineEdit->text(),
                      ui->descriptionLineEdit->toPlainText(),
                      DURATION(
                          ui->durationDays->value(),
                          ui->durationHours->value(),
                          ui->durationMinutes->value(),
                          ui->durationSeconds->value()
                        )
                      );
    r.setRepeatable(ui->checkBox->isChecked());
    return r;
}
