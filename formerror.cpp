#include "formerror.h"
#include "ui_formerror.h"

FormError::FormError(QWidget *parent) :
                                        QDialog(parent),
                                        ui(std::make_shared<Ui::FormError>())
{
  ui->setupUi(this);
}

void FormError::on_pushButton_clicked()
{
  close();
}

