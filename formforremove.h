#pragma once

#ifndef FORMFORREMOVE_H
#define FORMFORREMOVE_H

#include <memory>
#include <QDialog>
#include <QTableView>

namespace Ui {
class FormForRemove;
}

class FormForRemove : public QDialog
{
    Q_OBJECT

public:
    explicit FormForRemove(QWidget *parent = nullptr);
    ~FormForRemove() = default;

    int getRowForRemove() const;
    bool isWantToRemove() const;
    static bool requireValidIndexRow(int row, int countRows, int min);
private slots:
    void on_remove_clicked();

private:
    std::shared_ptr<Ui::FormForRemove> ui;
    bool isWantToRamove_ = false;
};

#endif // FORMFORREMOVE_H
