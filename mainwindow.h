#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "form.h"
#include "formforremove.h"
#include "model.h"

#include <memory>
#include <QMainWindow>
#include <QTableWidgetItem>
#include <future>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void on_addNewMaterial_clicked();

    void on_removeAMaterial_clicked();

    void on_header_clicked(int index);
private:
    void showTableView();
    void loading();
    void createFormError();
    void removeMatterial(FormForRemove& form);
    void addMaterial(Form& form);
private:
    Model model;
    std::shared_ptr<Ui::MainWindow> ui;
};
#endif // MAINWINDOW_H
