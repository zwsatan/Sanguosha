#include "sgsui_InitiateBox.h"
#include "sgsui_MainWindow.h"
#include "sgsui_GUIStaticData.h"
#include "sgsui_RoleViewer.h"
#include "sgsui_output.h"
#include <QKeyEvent>
#include <QCloseEvent>
#include <exception>
#include <iostream>

char datamain(int playerNumber);

namespace sgsui {

const QSize InitiateBox::ms_fixedSize(400, 300);
const QString InitiateBox::ms_playAgain(trUtf8("再来一局"));

InitiateBox::InitiateBox()
	: m_roleViewer(0)
{
	setWindowIcon(QIcon("images/icon.png"));
	setupUi(this);
	setFixedSize(ms_fixedSize);

	QApplication::setApplicationName("SanguoshaEX");
	QApplication::setOverrideCursor(QCursor(QPixmap("images/cursor/1.png")));
	GUIStaticData::loadFonts();
//	debugOption->hide();
	connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
	connect(roleButton, SIGNAL(clicked()), this, SLOT(showRoleViewer()));
}

InitiateBox::~InitiateBox()
{
	if (m_roleViewer)
		delete m_roleViewer;
}

void InitiateBox::start()
{
	if (m_roleViewer)
		m_roleViewer->hide();

	int options = 0, package = 0;

	options += musicCheckBox->isChecked();
	options += soundCheckBox->isChecked() << 1;
	options += fullscreenCheckBox->isChecked() << 2;
	options += AICheckBox->isChecked() << 3;

	package += jzpCheckBox->isChecked();
	package += fengCheckBox->isChecked() << 1;
	package += huoCheckBox->isChecked() << 2;
	package += linCheckBox->isChecked() << 3;
	hide();

	QFont systemFont = QApplication::font();
	setFont(systemFont);

	GUIStaticData::m_debugLogOn = debugLogCheckBox->isChecked();
	GUIStaticData::m_options = options;
	GUIStaticData::m_package = package;
	GUIStaticData::m_fullPlayerCount = playerNumSpinBox->value();
	GUIStaticData::m_humanPlayerIndex = 1;

	sgs::Interface::m_maxShaUse = shaCheckBox->isChecked() ? -1 : shaSpinBox->value();

	sgsui::printInitiateMessage();
	sgsui::MainWindow * mainWindow = new sgsui::MainWindow();
	printDebug("InitiateBox::start: MainWindow created");
	mainWindow->show();
	printDebug("InitiateBox::start: MainWindow shown, now wait some time");
	// wait for the windows to be fully constructed
	mainWindow->sleepSomeTime(GUIStaticData::initWaitTime);
	printDebug("InitiateBox::start: about to run datamain");

	char result = 0xFF;
	try
	{
		// run the game
		result = datamain(sgsui::GUIStaticData::m_fullPlayerCount);
	}
	catch (std::exception& expection)
	{
		std::cout << "Exception: " << expection.what();
	}

	mainWindow->gameOver(result); // show game result
	delete mainWindow;

	startButton->setText(ms_playAgain);
	show();
}

void InitiateBox::showRoleViewer()
{
	if (!m_roleViewer)
		m_roleViewer = new RoleViewer();
	m_roleViewer->show();
}

void InitiateBox::closeEvent(QCloseEvent *event)
{
	event->ignore();
	GUIStaticData::quitGame();
}

void InitiateBox::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_D && event->modifiers() == Qt::ControlModifier)
		debugOption->setHidden(debugOption->isVisible());
	else
		QDialog::keyPressEvent(event);
}

} /* namespace sgsui */
