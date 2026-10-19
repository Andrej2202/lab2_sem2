#include "BenchmarkTableModel.hpp"

BenchmarkTableModel::BenchmarkTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int BenchmarkTableModel::rowCount(const QModelIndex &) const { return m_rows.size(); }
int BenchmarkTableModel::columnCount(const QModelIndex &) const { return 6; }

QVariant BenchmarkTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();
    const auto &r = m_rows[index.row()];
    switch (index.column()) {
        case 0: return r.dataType;
        case 1: return r.dataSize;
        case 2: return QString::number(r.treeBuildTime, 'f', 2);
        case 3: return QString::number(r.sortedBuildTime, 'f', 2);
        case 4: return QString::number(r.treeSearchTime, 'f', 2);
        case 5: return QString::number(r.sortedSearchTime, 'f', 2);
    }
    return QVariant();
}

QVariant BenchmarkTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return "Тип данных";
            case 1: return "N";
            case 2: return "Построение дерева (мс)";
            case 3: return "Построение массива (мс)";
            case 4: return "Поиск в дереве (мс)";
            case 5: return "Поиск в массиве (мс)";
        }
    }
    return QVariant();
}

void BenchmarkTableModel::setRows(const QVector<BenchmarkRow> &rows) {
    beginResetModel();
    m_rows = rows;
    endResetModel();
}