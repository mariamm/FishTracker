#include <QtWidgets>
#include <QDockWidget>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QStackedWidget>
#include <QLayout>
//#if defined(QT_PRINTSUPPORT_LIB)
//#include <QtPrintSupport/qtprintsupportglobal.h>
//#if QT_CONFIG(printdialog)
//#include <QPrintDialog>
//#endif
//#endif

#include "MainWindow.h"
#include "Common.h"

#include <opencv2/opencv.hpp>

MainWindow::MainWindow()
{
    setWindowTitle(tr("FishTracker v.1"));

    Player* player = new Player(this);
    
    setCentralWidget(player);

}
