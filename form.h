#pragma once

#ifndef FORM_H
#define FORM_H

#include <memory>
#include <vector>
#include <QDialog>

namespace Ui {
class Form;
}

class Form : public QDialog {
    Q_OBJECT
public:
    explicit Form(QWidget *parent = nullptr);
    ~Form() = default;

    float getBasePrice() const;
    float getQuality() const;
    QString getMaterialName() const;
    bool isWantToAdd() const;
private slots:
    void on_push_clicked();

private:
    std::shared_ptr<Ui::Form> ui;
    bool isWantToAdd_ = false;
};

#endif // FORM_H
