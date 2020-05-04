#include <QtWidgets>
#include <QDockWidget>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QStackedWidget>
#include <QLayout>

#include "MainWindow.h"
#include "Common.h"

#include <opencv2/opencv.hpp>

MainWindow::MainWindow()
{
    setWindowTitle(tr("FishTracker v.1"));
    QWidget *ui_area = new QWidget;
    setCentralWidget(ui_area);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    Player* player = new Player;
    
    mainLayout->addWidget(player);
    ui_area->setLayout(mainLayout);
}
