#pragma once
#include <QAbstractTableModel>
#include <QVector>
#include <QString>

struct BenchmarkRow {
    QString dataType;
    int dataSize;
    double treeBuildTime;
    double sortedBuildTime;
    double treeSearchTime;
    double sortedSearchTime;
};

class BenchmarkTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BenchmarkTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRows(const QVector<BenchmarkRow> &rows);

private:
    QVector<BenchmarkRow> m_rows;
};