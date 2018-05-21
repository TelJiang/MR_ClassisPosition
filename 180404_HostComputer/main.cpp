#include "hostcomputermainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HostComputerMainWindow w;
	w.show();
	return a.exec();
}
