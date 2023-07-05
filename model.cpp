#include "model.h"

#include <fstream>
#include <algorithm>
#include <memory>
#include <thread>

Model::Model(QObject *parent)
    : QAbstractTableModel(parent)
{
  arrayHeadersNames.resize(baseColumnCount);
}

void Model::save()
{
  std::fstream file;
  file.open(saveFileName.toStdString(), std::ios::out);
  if (file.is_open())
    for (auto &i: arrayData){
      file << i.getName() << std::endl;
      file << i.getBasePrice() << std::endl;
      file << i.getQuality() << std::endl;
    }
  file.close();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    return arrayHeadersNames.at(section);
  return QVariant();
}

bool Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  if (role == Qt::EditRole && orientation == Qt::Horizontal)
      arrayHeadersNames[section] = value.toString();
  emit headerDataChanged(orientation, section, section);
  return true;
}

int Model::rowCount(const QModelIndex &parent) const
{
  return arrayData.size();
}

int Model::columnCount(const QModelIndex &parent) const
{
  return baseColumnCount;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole)
  {
    if (!index.isValid())
      return QVariant();
    switch(index.column())
    {
      case 0:
      return arrayData.at(index.row()).getName().c_str();
        break;
      case 1:
      return arrayData.at(index.row()).getBasePrice();
        break;
      case 2:
      return arrayData.at(index.row()).getQuality();
        break;
      case 3:
        return arrayData.at(index.row()).getFullPrice();
        break;
    }
  }
  return QVariant();
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role == Qt::EditRole)
  {
    switch(index.column())
    {
      case 0:
      arrayData[index.row()].setName(value.toString().toStdString());
        break;
      case 1:
      arrayData[index.row()].setBasePrice(value.toFloat());
        break;
      case 2:
      arrayData[index.row()].setQuality(value.toFloat());
        break;
    }
    emit dataChanged(index, index, {role});
    return true;
  }
  return false;
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

void Model::addRow(const Material &newMaterial)
{
  insertRows(0, 1);
  setData(this->index(0, 0), newMaterial.getName().c_str(), Qt::EditRole);
  setData(this->index(0, 1), newMaterial.getBasePrice(), Qt::EditRole);
  setData(this->index(0, 2), newMaterial.getQuality(), Qt::EditRole);
  setData(this->index(0, 3), newMaterial.getFullPrice(), Qt::EditRole);
}

void Model::addHeader()
{
  setHeaderData(0, Qt::Horizontal, "Name");
  setHeaderData(1, Qt::Horizontal, "Base price");
  setHeaderData(2, Qt::Horizontal, "Quality");
  setHeaderData(3, Qt::Horizontal, "Final price");
}

bool Model::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, count + row - 1);
  for (int i = 0; i < count; ++i)
    arrayData.insert(row, Material());
  endInsertRows();
  return true;
}

void Model::sort(int column, Qt::SortOrder order) {
  qDebug() << column;
  if (order == Qt::AscendingOrder) {
    switch(column) {
    case 0:
      std::sort(arrayData.begin(), arrayData.end(), [](const Material &a, const Material &b){
        return a.getName() < b.getName();
      });
      break;
    case 1:
      std::sort(arrayData.begin(), arrayData.end(), [](const Material &a, const Material &b){
        return a.getBasePrice() < b.getBasePrice();
      });
      break;
    case 2:
      std::sort(arrayData.begin(), arrayData.end(), [](const Material &a, const Material &b){
        return a.getQuality() < b.getQuality();
      });
      break;
    case 3:
      std::sort(arrayData.begin(), arrayData.end(), [&](const Material &a, const Material &b){
        return a.getFullPrice() < b.getFullPrice();
      });
      break;
    }
    emit dataChanged(this->index(0, 0), this->index(rowCount(), baseColumnCount));
  }
}

bool Model::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, count + row);
  for (int i = row; i < row + count; ++i)
    arrayData.removeAt(i);
  endRemoveRows();
  return true;
}

const QString &Model::getSaveFileName() const
{
  return saveFileName;
}

void Model::setSaveFileName(const QString &newSaveFileName)
{
  saveFileName = newSaveFileName;
}

void Model::deserialize()
{
  using namespace std::chrono_literals;

  std::fstream file;
  file.open(saveFileName.toStdString(), std::ios::in);

  while(!file.eof())
  {
    std::string data;
    Material temp;

    std::lock_guard lg(mutex_);

    std::getline(file, data);
    temp.setName(data);
    std::getline(file, data);
    temp.setBasePrice(atof(data.c_str()));
    std::getline(file, data);
    temp.setQuality(atof(data.c_str()));

    std::this_thread::sleep_for(2s);
    if (!data.empty())
      addRow(temp);
  }
  loaded = true;
  isDeserialize.notify_all();

  file.close();
}

QString Model::getLoadingStatus() const
{
  return QVariant(arrayData.size()).toString();
}

std::condition_variable &Model::getIsDeserialize()
{
  return isDeserialize;
}

std::mutex &Model::mutex()
{
  return mutex_;
}

bool Model::wasLoaded() const
{
  return loaded;
}
