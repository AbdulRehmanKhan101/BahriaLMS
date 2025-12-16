
#include <QApplication>
#include <QFile>
#include "mainwindow.h"

static QString loadTextFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return {};
    return QString::fromUtf8(f.readAll());
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // Apply theme (global)
    a.setStyleSheet(loadTextFile(":/theme/bahria.qss"));

    MainWindow w;
    w.show();
    return a.exec();
}
