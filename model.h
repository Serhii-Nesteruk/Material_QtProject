#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "material.h"

#include <QVector>
#include <QAbstractTableModel>

#include <condition_variable>
#include <mutex>

class Model : public QAbstractTableModel
{
  Q_OBJECT

public:
  inline static const int baseColumnCount = 4;

  explicit Model(QObject *parent = nullptr);

  void save();
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  void addRow(const Material &newMaterial);
  void addHeader();
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  const QString &getSaveFileName() const;
  void setSaveFileName(const QString &newSaveFileName);
  void deserialize();

  QString getLoadingStatus() const;
  std::condition_variable &getIsDeserialize();
  std::mutex &mutex();
  bool wasLoaded() const;
private:
  bool loaded = false;
  std::condition_variable isDeserialize;
  std::mutex mutex_;
  QString saveFileName = "info.txt";
  QVector<Material> arrayData{};
  QVector<QString> arrayHeadersNames{};
};

#endif // MODEL_H
