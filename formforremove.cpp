#include "formforremove.h"
#include "ui_formforremove.h"

FormForRemove::FormForRemove(QWidget *parent) :
    QDialog(parent),
    ui(std::make_shared<Ui::FormForRemove>()) {
    ui->setupUi(this);
}

int FormForRemove::getRowForRemove() const {
    return ui->textEdit->toPlainText().toInt();
}

bool FormForRemove::isWantToRemove() const {
  return isWantToRamove_;
}

bool FormForRemove::requireValidIndexRow(int row, int countRows, int min) {
  return row > countRows || row < min;
}

void FormForRemove::on_remove_clicked() {
    isWantToRamove_ = true;
    close();
}
