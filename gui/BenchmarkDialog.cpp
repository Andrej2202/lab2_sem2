#include "BenchmarkDialog.hpp"
#include <QHeaderView>

BenchmarkDialog::BenchmarkDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Результаты бенчмарка");
    resize(800, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    model_ = new BenchmarkTableModel(this);
    tableView_ = new QTableView();
    tableView_->setModel(model_);
    tableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addWidget(tableView_);
}

void BenchmarkDialog::setResults(const QVector<BenchmarkRow> &rows) {
    model_->setRows(rows);
}