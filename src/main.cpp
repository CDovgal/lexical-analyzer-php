#include "mainwindow.h"
#include <QApplication>

#ifdef __APPLE__ && __MACH__

int main(int argc, char *argv[]) {

#elif _WINDOWS

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* argv[], int nShowCmd) {
  int argc = 1;

#endif
    
  //QStringList paths = QCoreApplication::libraryPaths();
  //paths.append(".");
  //paths.append("platforms");
  //paths.append("imageformats");
  //paths.append("printsupport");
  //QCoreApplication::setLibraryPaths(paths);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
