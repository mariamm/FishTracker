#include <QApplication>
#include "MainWindow.h"
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile f(":qdarkstyle/darkstyle.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }

    MainWindow window;
    window.show();
    return app.exec();
}
