#include <iostream>
#include <string>

#include "mainWindow.hpp"

#ifdef _WIN32
#include <windows.h>
#include <QApplication>

#elif __gnu_linux__

// #include <qt5/QtWidgets/QApplication>
#include <qt4/QtGui//QApplication>

#endif

// int main(int argc, char **argv) {
//   
//   std::string input = argv[0];
//   if (argc < 2){ // if the parameter doesnt exist, the user forgot to enter it
//     std::cerr << "ERROR: Specify a file (format: 'executable(this program) DataFile')\n";
//     return 0;
//   }  
//     crapParser P(argv[1]);
//     P.copyFileToMem(argv[1]);
//     P.parseIntoSections();
//     
//     
//     return 0;
// }



int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    mainWindow mWindow;

#if _WIN32
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
if (osvi.dwMajorVersion >= 6  ) // if vista or later
    mWindow.resize( 400, 100 );
else
    mWindow.resize( 400, 100 );

#elif __gnu_linux__
    mWindow.resize( 400, 100 );
#endif
    mWindow.show();

    return a.exec();
}
