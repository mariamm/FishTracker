#pragma once

#include <QMainWindow>
#include <QString>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    private:
        //Ui
        void createActions();
        void createMenus();
        void updateActions();

        //File
        bool loadFile(const QString &);
        bool saveFile(const QString &fileName);

        //File Menu
        QMenu *m_fileMenu;
        QAction *m_openAction;
        QAction *m_saveAction;
        QAction *m_saveAsAction;
        QAction *m_printAction;
        QAction *m_themeAction;
        QAction *m_exitAction;

        //Edit Menu
        QMenu *m_editMenu;
        QAction *m_undoAction;
        QAction *m_redoAction;
        QAction *m_copyAction;
        QAction *m_pasteAction;

        //View Menu
        QMenu *m_viewMenu;
        QAction *m_zoomInAction;
        QAction *m_zoomOutAction;
        QAction *m_normalSizeAction;
        QAction *m_fitToWindowAction;

        //Help Menu
        QMenu *m_helpMenu;
        QAction *m_aboutAction;


    private slots:
        //File
        void open(){}
        void save(){}
        void saveAs(){}
        void print(){}
        void toggleTheme(bool toggled){}

        //Help
        void about(){}
};
