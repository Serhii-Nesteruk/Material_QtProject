#pragma once

#ifndef FORMERROR_H
#define FORMERROR_H

#include <QDialog>
#include <memory>

namespace Ui {
class FormError;
}

class FormError : public QDialog
{
  Q_OBJECT

public:
  explicit FormError(QWidget *parent = nullptr);
  ~FormError() = default;

private slots:
  void on_pushButton_clicked();

private:
  std::shared_ptr<Ui::FormError> ui;
};

#endif // FORMERROR_H
