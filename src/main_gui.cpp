#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Устанавливаем стиль приложения (опционально, делает его современнее)
    app.setStyle("Fusion");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}