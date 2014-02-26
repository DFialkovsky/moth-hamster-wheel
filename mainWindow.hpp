#ifndef _mainWindow_hpp_
#define _mainWindow_hpp_

#ifdef __gnu_linux__


#if QT_VERSION <= 0x049 // if its less or equal to Qt 4.99.x // which will prolly never exist

#include <qt4/QtGui/QMainWindow>
#include <qt4/QtGui/QApplication>
#include <qt4/QtGui/QFileDialog>
#include <qt4/QtGui/QMessageBox>
#include <qt4/QtGui/QWidget>
#include <qt4/QtGui/QMenuBar>
#include <qt4/QtGui/QMenu>
#include <qt4/QtGui/QAction>
#include <qt4/QtGui/QKeySequence>
#include <qt4/QtGui/QToolButton>
#include <qt4/QtGui/QToolBar>
#include <qt4/QtGui/QStatusBar>
#include <qt4/QtGui/QCheckBox>
#include <qt4/QtGui/QDesktopServices>
#include <qt4/QtGui/QErrorMessage>

#include <qt4/QtCore/QString>
#include <qt4/QtCore/QStringList>
#include <qt4/QtCore/QUrl>

#elif QT_VERSION >= 0x0500

#include <qt5/QtWidgets/QMainWindow>
#include <qt5/QtWidgets/QApplication>
#include <qt5/QtWidgets/QFileDialog>
#include <qt5/QtWidgets/QMessageBox>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QMenuBar>
#include <qt5/QtWidgets/QMenu>
#include <qt5/QtWidgets/QAction>
#include <qt5/QtGui/QKeySequence>
#include <qt5/QtWidgets/QToolButton>
#include <qt5/QtWidgets/QToolBar>
#include <qt5/QtWidgets/QStatusBar>
#include <qt5/QtWidgets/QCheckBox>
#include <qt5/QtGui/QDesktopServices>
#include <qt5/QtWidgets/QErrorMessage>

#include <qt5/QtCore/QString>
#include <qt5/QtCore/QStringList>
#include <qt5/QtCore/QUrl>

#endif // Qt Version for linux

#elif _WIN32 

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QToolButton>
#include <QToolBar>
#include <QStatusBar>

#include <QCheckBox>
#include <QDesktopServices>
#include <QErrorMessage>

#include <QString>
#include <QStringList>
#include <QUrl>


#endif // OS

// #ifndef nullptr
// #define nullptr 0
// #endif

#include "newParser.hpp"
#include <utility> // std::pair
static const QString HAMSTER_VERSION = "0.1.2";

class mainWindow : public QMainWindow {
  
    Q_OBJECT
    
public:
    mainWindow(QWidget * = nullptr);
    ~mainWindow();
    
public Q_SLOTS:
    
    void 		openFile();
    void 		newSession();
    void 		quit();
    void 		aboutWindow();
    void 		aboutQtWindow();
    
private:
    //void printTable(qint32 tableNumber);
    void 		createToolBar();
    void 		createActions();
    void 		generateMenu();
    
    bool 		ifErrorsEvaluate(std::pair<int, unsigned int> msg);
private: //datastructures
    //QStringList 	outputFileList_;
    
private: // gui vars
    
    QMenu 		*fileMenu_;
    QAction 		*openAct_;
    QAction		*newAct_;
    QAction 		*quitAct_;
    
    QMenu		*helpMenu_;
    QAction		*aboutAct_;
    QAction 		*aboutQtAct_;
    
    QToolButton		*openFileBtn_;
    QCheckBox 		*openFolderWhenSaveCheckBox_;
    
    QStatusBar		*sbar_;
};




#endif //#ifndef _mainWindow_hpp_
