#include "sgsui_InitiateBox.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	srand(static_cast<unsigned>(time(0)));

	sgsui::InitiateBox * initiateBox = new sgsui::InitiateBox();
	initiateBox->show();

	return app.exec();
}
