#include "MainWindow.hpp"
#include <QHeaderView>
#include <QDateTime>
#include <QApplication>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Student Index System (Qt)");
    resize(900, 600);
    
    // Безопасная инициализация системы
    try {
        system_ = new IndexSystem("students.bin");
        qDebug() << "[DEBUG] IndexSystem успешно создан.";
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Критическая ошибка",
            QString("Не удалось инициализировать систему:\n%1").arg(e.what()));
        std::cerr << "[DEBUG] Ошибка конструктора IndexSystem: " << e.what() << std::endl;
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
    
    // === ЛЕВАЯ ПАНЕЛЬ (Управление) ===
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setAlignment(Qt::AlignTop);
    
    // 1. Добавление студента
    QGroupBox* groupAdd = new QGroupBox("Добавить студента");
    QVBoxLayout* addLayout = new QVBoxLayout(groupAdd);
    
    editFirstName_ = new QLineEdit();
    editFirstName_->setPlaceholderText("Имя");
    editLastName_ = new QLineEdit();
    editLastName_->setPlaceholderText("Фамилия");
    editYear_ = new QLineEdit();
    editYear_->setPlaceholderText("Год рождения");
    editGroup_ = new QLineEdit();
    editGroup_->setPlaceholderText("Группа");
    
    QPushButton* btnAdd = new QPushButton("Добавить");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    
    addLayout->addWidget(editFirstName_);
    addLayout->addWidget(editLastName_);
    addLayout->addWidget(editYear_);
    addLayout->addWidget(editGroup_);
    addLayout->addWidget(btnAdd);
    controlLayout->addWidget(groupAdd);
    
    // 2. Поиск
    QGroupBox* groupSearch = new QGroupBox("Поиск");
    QVBoxLayout* searchLayout = new QVBoxLayout(groupSearch);
    
    editSearchYear_ = new QLineEdit();
    editSearchYear_->setPlaceholderText("Точный год");
    QPushButton* btnSearchYear = new QPushButton("Найти по году");
    connect(btnSearchYear, &QPushButton::clicked, this, &MainWindow::onSearchByYear);
    
    QHBoxLayout* rangeLayout = new QHBoxLayout();
    editMinYear_ = new QLineEdit();
    editMinYear_->setPlaceholderText("От");
    editMaxYear_ = new QLineEdit();
    editMaxYear_->setPlaceholderText("До");
    QPushButton* btnSearchRange = new QPushButton("Найти в диапазоне");
    connect(btnSearchRange, &QPushButton::clicked, this, &MainWindow::onSearchByRange);
    rangeLayout->addWidget(editMinYear_);
    rangeLayout->addWidget(editMaxYear_);
    rangeLayout->addWidget(btnSearchRange);
    
    searchLayout->addWidget(editSearchYear_);
    searchLayout->addWidget(btnSearchYear);
    searchLayout->addLayout(rangeLayout);
    controlLayout->addWidget(groupSearch);
    
    // 3. Общие действия
    QGroupBox* groupActions = new QGroupBox("Действия");
    QVBoxLayout* actionsLayout = new QVBoxLayout(groupActions);
    
    QPushButton* btnLoadCSV = new QPushButton("Загрузить из CSV");
    connect(btnLoadCSV, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    
    QPushButton* btnShowAll = new QPushButton("Показать всех");
    connect(btnShowAll, &QPushButton::clicked, this, &MainWindow::onShowAll);
    
    QPushButton* btnBenchmark = new QPushButton("Запустить бенчмарк");
    connect(btnBenchmark, &QPushButton::clicked, this, &MainWindow::onRunBenchmark);
    
    actionsLayout->addWidget(btnLoadCSV);
    actionsLayout->addWidget(btnShowAll);
    actionsLayout->addWidget(btnBenchmark);
    controlLayout->addWidget(groupActions);
    
    controlLayout->addStretch();
    
    // Статус бар
    statusLabel_ = new QLabel("Готово");
    controlLayout->addWidget(statusLabel_);
    
    mainLayout->addLayout(controlLayout, 1);
    
    // === ПРАВАЯ ПАНЕЛЬ (Таблица) ===
    tableWidget_ = new QTableWidget();
    tableWidget_->setColumnCount(4);
    tableWidget_->setHorizontalHeaderLabels({"Имя", "Фамилия", "Год рождения", "Группа"});
    tableWidget_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    mainLayout->addWidget(tableWidget_, 3);
}

void MainWindow::clearTable() {
    tableWidget_->setRowCount(0);
    statusLabel_->setText("Готово");
}

void MainWindow::populateTable(Sequence<Student>* students) {
    qDebug() << "[DEBUG] populateTable начался";
    
    clearTable();
    
    if (!students) {
        qDebug() << "[DEBUG] students == nullptr";
        return;
    }
    
    try {
        size_t len = students->GetLength();
        qDebug() << "[DEBUG] Длина последовательности:" << len;
        
        if (len == 0) {
            qDebug() << "[DEBUG] Последовательность пустая";
            statusLabel_->setText("Ничего не найдено");
            delete students;
            return;
        }
        
        tableWidget_->setRowCount(static_cast<int>(len));
        
        for (size_t i = 0; i < len; i++) {
            qDebug() << "[DEBUG] Обработка строки" << i;
            
            try {
                Student s = students->Get(i);
                qDebug() << "[DEBUG] Студент" << i << "получен";
                
                // Безопасная конвертация
                QString firstName = s.firstName.empty() ? QString() : QString::fromUtf8(s.firstName.c_str());
                QString lastName = s.lastName.empty() ? QString() : QString::fromUtf8(s.lastName.c_str());
                QString group = s.group.empty() ? QString() : QString::fromUtf8(s.group.c_str());
                
                qDebug() << "[DEBUG] Строки сконвертированы";
                
                tableWidget_->setItem(i, 0, new QTableWidgetItem(firstName));
                tableWidget_->setItem(i, 1, new QTableWidgetItem(lastName));
                tableWidget_->setItem(i, 2, new QTableWidgetItem(QString::number(s.birthYear)));
                tableWidget_->setItem(i, 3, new QTableWidgetItem(group));
                
                qDebug() << "[DEBUG] Строка" << i << "добавлена в таблицу";
                
            } catch (const std::exception& e) {
                qDebug() << "[DEBUG] Ошибка при обработке строки" << i << ":" << e.what();
            } catch (...) {
                qDebug() << "[DEBUG] Неизвестная ошибка при обработке строки" << i;
            }
        }
        
        statusLabel_->setText(QString("Найдено записей: %1").arg(len));
        qDebug() << "[DEBUG] populateTable завершен успешно";
        
    } catch (const std::exception& e) {
        qDebug() << "[DEBUG] Ошибка в populateTable:" << e.what();
    } catch (...) {
        qDebug() << "[DEBUG] Неизвестная ошибка в populateTable";
    }
    
    delete students;
}

// ============================================
// СЛОТЫ
// ============================================

void MainWindow::onLoadCSV() {
    qDebug() << "[DEBUG] 1. Начало onLoadCSV";
    
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите CSV файл", "", "CSV Files (*.csv)");
    qDebug() << "[DEBUG] 2. Диалог закрыт, файл:" << fileName;

    if (fileName.isEmpty()) {
        qDebug() << "[DEBUG] Файл не выбран.";
        return;
    }

    if (!system_) {
        qDebug() << "[DEBUG] ОШИБКА: system_ равен nullptr!";
        QMessageBox::critical(this, "Ошибка", "Система не инициализирована.");
        return;
    }
    qDebug() << "[DEBUG] 3. system_ валиден";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[DEBUG] Ошибка открытия файла";
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + fileName);
        return;
    }
    qDebug() << "[DEBUG] 4. Файл открыт";

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    qDebug() << "[DEBUG] 5. QTextStream создан с UTF-8";

    bool headerSkipped = false;
    int count = 0;
    int skipped = 0;
    int lineNum = 0;
    
    qDebug() << "[DEBUG] 6. Начало цикла чтения";

    try {
        while (!in.atEnd()) {
            lineNum++;
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
                    if (year < 1900 || year > 2100) {
                        skipped++;
                        continue;
                    }
                    
                    // ВАЖНО: используем toUtf8().constData() вместо toStdString()
                    Student student(
                        std::string(firstName.toUtf8().constData()),
                        std::string(lastName.toUtf8().constData()),
                        year,
                        std::string(group.toUtf8().constData())
                    );
                    
                    system_->AddStudent(student);
                    count++;
                    
                } catch (const std::exception& e) {
                    qDebug() << "[DEBUG] Ошибка парсинга строки" << lineNum << ":" << e.what();
                    skipped++;
                } catch (...) {
                    qDebug() << "[DEBUG] Неизвестная ошибка парсинга строки" << lineNum;
                    skipped++;
                }
            } else {
                skipped++;
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "[DEBUG] КРИТИЧЕСКАЯ ОШИБКА в цикле:" << e.what();
    } catch (...) {
        qDebug() << "[DEBUG] НЕИЗВЕСТНАЯ КРИТИЧЕСКАЯ ОШИБКА в цикле";
    }
    
    file.close();
    qDebug() << "[DEBUG] 7. Цикл завершен. Загружено:" << count << "Пропущено:" << skipped;
    
    statusLabel_->setText(QString("Загружено %1 записей из CSV").arg(count));
    onShowAll();
    qDebug() << "[DEBUG] 8. Готово";
}

void MainWindow::onAddStudent() {
    try {
        // Используем toUtf8().constData() вместо toStdString()
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
        
        // Очистка полей
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
    qDebug() << "[DEBUG] onSearchByYear вызван";
    
    if (!system_) {
        QMessageBox::critical(this, "Ошибка", "Система не инициализирована.");
        return;
    }
    
    try {
        int year = editSearchYear_->text().toInt();
        qDebug() << "[DEBUG] Ищем год:" << year;
        
        if (year == 0) {
            QMessageBox::warning(this, "Предупреждение", "Введите корректный год!");
            return;
        }
        
        auto results = system_->SearchByYear(year);
        qDebug() << "[DEBUG] SearchByYear вернул результаты";
        
        if (!results) {
            qDebug() << "[DEBUG] results == nullptr";
            clearTable();
            statusLabel_->setText("Ничего не найдено");
            return;
        }
        
        qDebug() << "[DEBUG] Вызов populateTable";
        populateTable(results);
        qDebug() << "[DEBUG] populateTable завершен";
        
    } catch (const std::exception& e) {
        qDebug() << "[DEBUG] Ошибка в onSearchByYear:" << e.what();
        QMessageBox::critical(this, "Ошибка поиска", 
            QString("Произошла ошибка:\n%1").arg(e.what()));
        clearTable();
    } catch (...) {
        qDebug() << "[DEBUG] Неизвестная ошибка в onSearchByYear";
        QMessageBox::critical(this, "Ошибка", "Произошла неизвестная ошибка.");
        clearTable();
    }
}

void MainWindow::onSearchByRange() {
    qDebug() << "[DEBUG] onSearchByRange вызван";
    
    if (!system_) {
        QMessageBox::critical(this, "Ошибка", "Система не инициализирована.");
        return;
    }
    
    try {
        int minYear = editMinYear_->text().toInt();
        int maxYear = editMaxYear_->text().toInt();
        qDebug() << "[DEBUG] Ищем диапазон:" << minYear << "-" << maxYear;
        
        if (minYear == 0 || maxYear == 0 || minYear > maxYear) {
            QMessageBox::warning(this, "Предупреждение", "Введите корректный диапазон!");
            return;
        }
        
        auto results = system_->SearchByYearRange(minYear, maxYear);
        qDebug() << "[DEBUG] SearchByYearRange вернул результаты";
        
        if (!results) {
            qDebug() << "[DEBUG] results == nullptr";
            clearTable();
            statusLabel_->setText("Ничего не найдено");
            return;
        }
        
        qDebug() << "[DEBUG] Вызов populateTable";
        populateTable(results);
        qDebug() << "[DEBUG] populateTable завершен";
        
    } catch (const std::exception& e) {
        qDebug() << "[DEBUG] Ошибка в onSearchByRange:" << e.what();
        QMessageBox::critical(this, "Ошибка поиска", 
            QString("Произошла ошибка:\n%1").arg(e.what()));
        clearTable();
    } catch (...) {
        qDebug() << "[DEBUG] Неизвестная ошибка в onSearchByRange";
        QMessageBox::critical(this, "Ошибка", "Произошла неизвестная ошибка.");
        clearTable();
    }
}

void MainWindow::onShowAll() {
    qDebug() << "[DEBUG] onShowAll вызван";
    
    if (!system_) {
        QMessageBox::critical(this, "Ошибка", "Система не инициализирована.");
        return;
    }
    
    try {
        auto results = system_->GetAllStudents();
        qDebug() << "[DEBUG] GetAllStudents вернул результаты";
        
        if (!results) {
            qDebug() << "[DEBUG] results == nullptr";
            clearTable();
            return;
        }
        
        qDebug() << "[DEBUG] Вызов populateTable";
        populateTable(results);
        qDebug() << "[DEBUG] populateTable завершен";
        
    } catch (const std::exception& e) {
        qDebug() << "[DEBUG] Ошибка в onShowAll:" << e.what();
        QMessageBox::critical(this, "Ошибка", 
            QString("Произошла ошибка:\n%1").arg(e.what()));
        clearTable();
    } catch (...) {
        qDebug() << "[DEBUG] Неизвестная ошибка в onShowAll";
        QMessageBox::critical(this, "Ошибка", "Произошла неизвестная ошибка.");
        clearTable();
    }
}


void MainWindow::onRunBenchmark() {
    statusLabel_->setText("Запуск бенчмарка... Пожалуйста, подождите.");
    QApplication::processEvents();
    
    try {
        QDir().mkpath("results");
        system_->RunFullBenchmark("results/results.csv");
        statusLabel_->setText("Бенчмарк завершен! Результаты в results/results.csv");
        QMessageBox::information(this, "Успех", "Бенчмарк завершен!\nРезультаты сохранены в results/results.csv");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
        statusLabel_->setText("Ошибка бенчмарка");
    }
}