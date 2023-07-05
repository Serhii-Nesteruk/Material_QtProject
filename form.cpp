#include "form.h"
#include "ui_form.h"

#include <stdexcept>
#include <QString>

Form::Form(QWidget *parent) :
    QDialog(parent),
    ui(std::make_shared<Ui::Form>()) {
    ui->setupUi(this);
}

float Form::getBasePrice() const {
    return ui->textEdit->toPlainText().toFloat();
}

float Form::getQuality() const {
    return ui->textEdit_2->toPlainText().toFloat();
}

QString Form::getMaterialName() const {
    return ui->textEdit_3->toPlainText();
}

bool Form::isWantToAdd() const {
    return isWantToAdd_;
}

void Form::on_push_clicked() {
    isWantToAdd_ = true;
    close();
}
