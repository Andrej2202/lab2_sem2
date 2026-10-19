#include "MainWindow.hpp"
#include "StudentTableModel.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Student Index System (Qt)");
    resize(900, 600);
    
    try {
        system_ = new IndexSystem("students.bin");
        qDebug() << "[DEBUG] IndexSystem создан";
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Критическая ошибка",
            QString("Не удалось инициализировать систему:\n%1").arg(e.what()));
    }
    setupUI();
}

MainWindow::~MainWindow() {
    delete system_;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setAlignment(Qt::AlignTop);
    
    QGroupBox* groupAdd = new QGroupBox("Добавить студента");
    QVBoxLayout* addLayout = new QVBoxLayout(groupAdd);
    
    editFirstName_ = new QLineEdit(); editFirstName_->setPlaceholderText("Имя");
    editLastName_ = new QLineEdit(); editLastName_->setPlaceholderText("Фамилия");
    editYear_ = new QLineEdit(); editYear_->setPlaceholderText("Год рождения");
    editGroup_ = new QLineEdit(); editGroup_->setPlaceholderText("Группа");
    
    QPushButton* btnAdd = new QPushButton("Добавить");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    
    addLayout->addWidget(editFirstName_);
    addLayout->addWidget(editLastName_);
    addLayout->addWidget(editYear_);
    addLayout->addWidget(editGroup_);
    addLayout->addWidget(btnAdd);
    controlLayout->addWidget(groupAdd);
    
    QGroupBox* groupSearch = new QGroupBox("Поиск");
    QVBoxLayout* searchLayout = new QVBoxLayout(groupSearch);
    
    editSearchYear_ = new QLineEdit(); editSearchYear_->setPlaceholderText("Точный год");
    QPushButton* btnSearchYear = new QPushButton("Найти по году");
    connect(btnSearchYear, &QPushButton::clicked, this, &MainWindow::onSearchByYear);
    
    QHBoxLayout* rangeLayout = new QHBoxLayout();
    editMinYear_ = new QLineEdit(); editMinYear_->setPlaceholderText("От");
    editMaxYear_ = new QLineEdit(); editMaxYear_->setPlaceholderText("До");
    QPushButton* btnSearchRange = new QPushButton("Найти в диапазоне");
    connect(btnSearchRange, &QPushButton::clicked, this, &MainWindow::onSearchByRange);
    rangeLayout->addWidget(editMinYear_);
    rangeLayout->addWidget(editMaxYear_);
    rangeLayout->addWidget(btnSearchRange);
    
    searchLayout->addWidget(editSearchYear_);
    searchLayout->addWidget(btnSearchYear);
    searchLayout->addLayout(rangeLayout);
    controlLayout->addWidget(groupSearch);
    
    QGroupBox* groupActions = new QGroupBox("Действия");
    QVBoxLayout* actionsLayout = new QVBoxLayout(groupActions);
    
    QPushButton* btnLoadCSV = new QPushButton("Загрузить из CSV");
    connect(btnLoadCSV, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    
    QPushButton* btnShowAll = new QPushButton("Показать всех");
    connect(btnShowAll, &QPushButton::clicked, this, &MainWindow::onShowAll);
    
    QPushButton* btnClear = new QPushButton("Очистить базу");
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearDatabase);
    
    QPushButton* btnBenchmark = new QPushButton("Запустить бенчмарк");
    connect(btnBenchmark, &QPushButton::clicked, this, &MainWindow::onRunBenchmark);
    
    actionsLayout->addWidget(btnLoadCSV);
    actionsLayout->addWidget(btnShowAll);
    actionsLayout->addWidget(btnClear);
    actionsLayout->addWidget(btnBenchmark);
    controlLayout->addWidget(groupActions);
    
    controlLayout->addStretch();
    
    statusLabel_ = new QLabel("Готово");
    controlLayout->addWidget(statusLabel_);
    
    mainLayout->addLayout(controlLayout, 1);
    
    model_ = new StudentTableModel(this);
    tableView_ = new QTableView();
    tableView_->setModel(model_);
    tableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(tableView_, 3);
}

void MainWindow::clearTable() {
    if (model_) model_->clear();
    if (statusLabel_) statusLabel_->setText("Готово");
}

QVector<StudentRow> convertToRows(Sequence<Student>* seq) {
    QVector<StudentRow> rows;
    if (!seq) return rows;
    
    size_t len = seq->GetLength();
    rows.reserve(static_cast<int>(len));
    
    for (size_t i = 0; i < len; i++) {
        Student s = seq->Get(i);
        StudentRow row;
        row.firstName = QString::fromUtf8(s.firstName.c_str());
        row.lastName = QString::fromUtf8(s.lastName.c_str());
        row.birthYear = s.birthYear;
        row.group = QString::fromUtf8(s.group.c_str());
        rows.append(row);
    }
    delete seq;
    return rows;
}


void MainWindow::onLoadCSV() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите CSV файл", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;

    if (!system_) {
        QMessageBox::critical(this, "Ошибка", "Система не инициализирована.");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + fileName);
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    bool headerSkipped = false;
    int count = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        if (!headerSkipped) {
            if (line.contains("Год") || line.contains("Year") || 
                line.contains("Имя") || line.contains("Name")) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true;
        }

        QStringList parts = line.split(",");
        if (parts.size() >= 4) {
            try {
                QString firstName = parts[0].trimmed();
                QString lastName = parts[1].trimmed();
                QString yearStr = parts[2].trimmed();
                QString group = parts[3].trimmed();

                firstName.remove("\""); 
                lastName.remove("\""); 
                group.remove("\"");

                int year = yearStr.toInt();
                if (year < 1900 || year > 2100) continue;

                Student student(
                    std::string(firstName.toUtf8().constData()),
                    std::string(lastName.toUtf8().constData()),
                    year,
                    std::string(group.toUtf8().constData())
                );
                
                system_->AddStudent(student);
                count++;
            } catch (...) {}
        }
    }
    file.close();
    
    statusLabel_->setText(QString("Загружено %1 записей").arg(count));
    onShowAll();
}

void MainWindow::onAddStudent() {
    if (!system_) return;
    
    try {
        std::string fName(editFirstName_->text().toUtf8().constData());
        std::string lName(editLastName_->text().toUtf8().constData());
        int year = editYear_->text().toInt();
        std::string group(editGroup_->text().toUtf8().constData());

        if (fName.empty() || lName.empty() || year == 0 || group.empty()) {
            QMessageBox::warning(this, "Предупреждение", "Заполните все поля!");
            return;
        }

        system_->AddStudent(Student(fName, lName, year, group));
        statusLabel_->setText("Студент добавлен");

        editFirstName_->clear();
        editLastName_->clear();
        editYear_->clear();
        editGroup_->clear();

        onShowAll();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::onSearchByYear() {
    if (!system_) return;
    
    int year = editSearchYear_->text().toInt();
    if (year == 0) {
        QMessageBox::warning(this, "Внимание", "Введите корректный год!");
        return;
    }

    try {
        Sequence<Student>* results = system_->SearchByYear(year);
        QVector<StudentRow> rows = convertToRows(results);
        model_->setStudents(rows);
        statusLabel_->setText(QString("Найдено записей: %1").arg(rows.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::onSearchByRange() {
    if (!system_) return;
    
    int minYear = editMinYear_->text().toInt();
    int maxYear = editMaxYear_->text().toInt();
    
    if (minYear == 0 || maxYear == 0 || minYear > maxYear) {
        QMessageBox::warning(this, "Внимание", "Введите корректный диапазон!");
        return;
    }

    try {
        Sequence<Student>* results = system_->SearchByYearRange(minYear, maxYear);
        QVector<StudentRow> rows = convertToRows(results);
        model_->setStudents(rows);
        statusLabel_->setText(QString("Найдено записей: %1").arg(rows.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::onShowAll() {
    if (!system_) return;
    
    try {
        Sequence<Student>* results = system_->GetAllStudents();
        QVector<StudentRow> rows = convertToRows(results);
        model_->setStudents(rows);
        statusLabel_->setText(QString("Всего записей: %1").arg(rows.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

void MainWindow::onClearDatabase() {
    if (!system_) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение",
        "Удалить все записи из базы?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        try {
            system_->ClearDatabase();
            clearTable();
            statusLabel_->setText("База очищена");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", e.what());
        }
    }
}

void MainWindow::onRunBenchmark() {
    statusLabel_->setText("Запуск бенчмарка...");
    QApplication::processEvents();
    
    try {
        QDir().mkpath("results");
        
        
        IndexSystem tempSystem("benchmark_temp_system.bin");
        
        QVector<BenchmarkRow> rows;
        
        
        size_t sizes[] = {1000, 10000};
        
        for (size_t N : sizes) {
            auto res1 = tempSystem.RunBenchmark(N, true, true);
            BenchmarkRow row1;
            row1.dataType = QString::fromStdString(res1.dataType);
            row1.dataSize = res1.dataSize;
            row1.treeBuildTime = res1.treeBuildTime_ms;
            row1.sortedBuildTime = res1.sortedBuildTime_ms;
            row1.treeSearchTime = res1.treeSearchTime_ms;
            row1.sortedSearchTime = res1.sortedSearchTime_ms;
            rows.append(row1);
            
            
            auto res2 = tempSystem.RunBenchmark(N, true, false);
            BenchmarkRow row2;
            row2.dataType = QString::fromStdString(res2.dataType);
            row2.dataSize = res2.dataSize;
            row2.treeBuildTime = res2.treeBuildTime_ms;
            row2.sortedBuildTime = res2.sortedBuildTime_ms;
            row2.treeSearchTime = res2.treeSearchTime_ms;
            row2.sortedSearchTime = res2.sortedSearchTime_ms;
            rows.append(row2);
            
            
            auto res3 = tempSystem.RunBenchmark(N, false, false);
            BenchmarkRow row3;
            row3.dataType = QString::fromStdString(res3.dataType);
            row3.dataSize = res3.dataSize;
            row3.treeBuildTime = res3.treeBuildTime_ms;
            row3.sortedBuildTime = res3.sortedBuildTime_ms;
            row3.treeSearchTime = res3.treeSearchTime_ms;
            row3.sortedSearchTime = res3.sortedSearchTime_ms;
            rows.append(row3);
        }
        
        
        tempSystem.RunFullBenchmark("results/results.csv");
        
        
        BenchmarkDialog dialog(this);
        dialog.setResults(rows);
        dialog.exec();
        
        statusLabel_->setText("Бенчмарк завершён");
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
        statusLabel_->setText("Ошибка бенчмарка");
    }
}