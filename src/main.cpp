#include "mainwindow.h"
#include <QApplication>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* argv[], int nShowCmd) {
  //QStringList paths = QCoreApplication::libraryPaths();
  //paths.append(".");
  //paths.append("platforms");
  //paths.append("imageformats");
  //paths.append("printsupport");
  //QCoreApplication::setLibraryPaths(paths);

  int argc = 1;
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//
//    return a.exec();
//}
