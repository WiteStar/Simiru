#include "stdafx.h"
#include "Simiru.h"
#include <QtWidgets/QApplication>

#include "ElaApplication.h"

int main(int argc, char* argv[])
{
#ifdef NDBUG
	// redirect log
	QFile logFile("Simiru.log");
	logFile.open(QFile::Append | QFile::Text);
	QTextStream logger(&logFile);
	qInstallMessageHandler([&](QtMsgType type, const QMessageLogContext& context, const QString& msg) { logger << msg << "\n"; });
#endif

	QApplication app(argc, argv);
	eApp->init();

	Simiru window;
	window.show();
	return app.exec();
}