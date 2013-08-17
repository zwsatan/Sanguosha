#include "sgsui_output.h"
#include "sgsui_MainWindow.h"
#include "sgsui_GUIStaticData.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <cstdio>

namespace sgsui {

QFile				g_debugLog("debug_log.html");
QFile				g_historyLog("history_log.html");
QTextStream			g_debugStream(&g_debugLog);
QTextStream			g_historyStream(&g_historyLog);
//QTextStream debugStream(stdout);

void printDebug(const QString& textInput)
{
	if (GUIStaticData::isDebugLogOn())
	{
		g_debugLog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		g_debugStream << QDateTime::currentDateTime().toString() << " ";
		g_debugStream << textInput << "<br />" << endl;
		g_debugLog.close();
		MainWindow::appendDebug(textInput);
	}
}

void printHistory(const QString& textInput)
{
	if (GUIStaticData::isDebugLogOn())
	{
		g_historyLog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		g_historyStream << QDateTime::currentDateTime().toString() << " " << textInput << "<br />" << endl;
		g_historyLog.close();
	}
	MainWindow::appendHistory(textInput);
}

void printInitiateMessage()
{
	if (GUIStaticData::isDebugLogOn())
	{
		g_debugLog.open(QIODevice::WriteOnly | QIODevice::Text);
		g_debugStream
				<< "<html><head>"
				<< "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
				<< "<title>SanguoshaEX - Debug Log</title></head>";
		g_debugStream << "<body>New Game Started (" << QDateTime::currentDateTime().toString() << ")<br />" << endl;
		g_debugLog.close();

		g_historyLog.open(QIODevice::WriteOnly | QIODevice::Text);
		g_historyStream
				<< "<html><head>"
				<< "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
				<< "<title>SanguoshaEX - History Log</title></head>";
		g_historyStream << "<body>New Game Started (" << QDateTime::currentDateTime().toString() << ")<br />" << endl;
		g_historyLog.close();
	}
}

void printEndingMessage()
{
	if (GUIStaticData::isDebugLogOn())
	{
		g_debugLog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		g_debugStream << "Game is Over (" << QDateTime::currentDateTime().toString() << ")<br /></body></html>" << endl;
		g_debugLog.close();

		g_historyLog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		g_historyStream << "Game is Over (" << QDateTime::currentDateTime().toString() << ")<br /></body></html>" << endl;
		g_historyLog.close();
	}
}

} /* namespace sgsui */
