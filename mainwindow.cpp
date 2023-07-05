#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formerror.h"

#include <stdexcept>
#include <thread>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_shared<Ui::MainWindow>()) {
  ui->setupUi(this);
  showTableView();
  model.addHeader();

  loading();

  connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::on_header_clicked);
}

void MainWindow::on_addNewMaterial_clicked() {
  Form form(this);
  form.setModal(true);
  form.show();
  form.exec();

  if(form.isWantToAdd())
    addMaterial(form);
  model.save();
}

void MainWindow::on_removeAMaterial_clicked() {
  FormForRemove form(this);
  form.setModal(true);
  form.show();
  form.exec();

  if (form.isWantToRemove())
    removeMatterial(form);
  model.save();
}

void MainWindow::on_header_clicked(int index) {
  if (index >= 0 && index <= 3)
    model.sort(index, Qt::AscendingOrder);
}

void MainWindow::showTableView() {
  ui->tableView->setModel(&model);
  ui->tableView->show();
}

void MainWindow::loading() {
  std::thread([this](){
    model.deserialize();
  }).detach();

  std::thread([this](){
    std::unique_lock ul(model.mutex());
    model.getIsDeserialize().wait(ul, [this](){ return model.wasLoaded();});
      ui->statusbar->showMessage(model.getLoadingStatus());
  }).detach();
}

void MainWindow::createFormError() {
  FormError formError(this);
  formError.setModal(true);
  formError.show();
  formError.exec();
}

void MainWindow::removeMatterial(FormForRemove &form) {
  int tempRow = form.getRowForRemove()-1;
  int countRow = ui->tableView->model()->rowCount();
  if (FormForRemove::requireValidIndexRow(tempRow, countRow, 0))
    createFormError();
  else
    model.removeRows(tempRow, 1);
}

void MainWindow::addMaterial(Form& form) {
  Material material_;
  material_.setName(form.getMaterialName().toStdString());
  material_.setBasePrice(form.getBasePrice());
  if (!material_.setQuality(form.getQuality()))
    createFormError();
  else
    model.addRow(material_);
}

