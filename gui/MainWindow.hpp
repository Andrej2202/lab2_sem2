#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>

#include "IndexSystem.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoadCSV();
    void onAddStudent();
    void onSearchByYear();
    void onSearchByRange();
    void onShowAll();
    void onRunBenchmark();

private:
    void setupUI();
    void populateTable(Sequence<Student>* students);
    void clearTable();

    // Инициализируем указатели nullptr, чтобы избежать мусора при ошибках
    IndexSystem* system_ = nullptr;
    QTableWidget* tableWidget_ = nullptr;
    QLineEdit* editFirstName_ = nullptr;
    QLineEdit* editLastName_ = nullptr;
    QLineEdit* editYear_ = nullptr;
    QLineEdit* editGroup_ = nullptr;
    QLineEdit* editSearchYear_ = nullptr;
    QLineEdit* editMinYear_ = nullptr;
    QLineEdit* editMaxYear_ = nullptr;
    QLabel* statusLabel_ = nullptr;
};