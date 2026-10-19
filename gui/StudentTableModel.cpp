#include "StudentTableModel.hpp"

StudentTableModel::StudentTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int StudentTableModel::rowCount(const QModelIndex &) const { return m_rows.size(); }
int StudentTableModel::columnCount(const QModelIndex &) const { return 4; }

QVariant StudentTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();
    const auto &r = m_rows[index.row()];
    switch (index.column()) {
        case 0: return r.firstName;
        case 1: return r.lastName;
        case 2: return r.birthYear;
        case 3: return r.group;
    }
    return QVariant();
}

QVariant StudentTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return "Имя";
            case 1: return "Фамилия";
            case 2: return "Год рождения";
            case 3: return "Группа";
        }
    }
    return QVariant();
}

void StudentTableModel::setStudents(const QVector<StudentRow> &rows) {
    beginResetModel();
    m_rows = rows;
    endResetModel();
}

void StudentTableModel::clear() {
    beginResetModel();
    m_rows.clear();
    endResetModel();
}