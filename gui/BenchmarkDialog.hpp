#pragma once
#include <QDialog>
#include <QTableView>
#include <QVBoxLayout>
#include "BenchmarkTableModel.hpp"

class BenchmarkDialog : public QDialog {
    Q_OBJECT
public:
    explicit BenchmarkDialog(QWidget *parent = nullptr);
    
    void setResults(const QVector<BenchmarkRow> &rows);

private:
    QTableView *tableView_;
    BenchmarkTableModel *model_;
};