# SanguoshaEX.pro
# Created on: Feb 26, 2012
# Author: latios
TEMPLATE = app
TARGET = SanguoshaEX
QT += core gui widgets multimedia

RC_FILE += src/sgs_ui/sgsui_icon.rc

CONFIG += c++11

HEADERS += \
	src/sgs_ui/sgsui_RoleViewer.h \
	src/sgs_ui/sgsui_CardViewer.h \
	src/sgs_ui/sgsui_RoleFrame.h \
	src/sgs_ui/sgsui_AbstractEquipProcessor.h \
	src/sgs_ui/sgsui_PlayerSort.h \
	src/sgs_ui/sgsui_ZhuangbeiLabel.h \
	src/sgs_ui/sgsui_SkillLabel.h \
	src/sgs_ui/sgsui_TextBrowser.h \
	src/sgs_ui/sgsui_Outstream.h \
	src/sgs_ui/sgsui_BlockLoop.h \
	src/sgs_ui/sgsui_ResultPlayerLabel.h \
	src/sgs_ui/sgsui_HelpBox.h \
	src/sgs_ui/sgsui_GameOverBox.h \
	src/sgs_ui/sgsui_AbstractPlayerArea.h \
	src/sgs_ui/sgsui_PosAnimation.h \
	src/sgs_ui/sgsui_BlockTimer.h \
	src/sgs_ui/sgsui_FanjianBox.h \
	src/sgs_ui/sgsui_WugufengdengBox.h \
	src/sgs_ui/sgsui_global.h \
	src/sgs_ui/sgsui_AboutFrame.h \
	src/sgs_ui/sgsui_AbstractCardContainer.h \
	src/sgs_ui/sgsui_AbstractDragProcessor.h \
	src/sgs_ui/sgsui_AudioPlayer.h \
	src/sgs_ui/sgsui_BlockableButton.h \
	src/sgs_ui/sgsui_BlockingFrame.h \
	src/sgs_ui/sgsui_CardButton.h \
	src/sgs_ui/sgsui_CardContainer.h \
	src/sgs_ui/sgsui_CardFrame.h \
	src/sgs_ui/sgsui_DialogBox.h \
	src/sgs_ui/sgsui_DragableCardFrame.h \
	src/sgs_ui/sgsui_GUIStaticData.h \
	src/sgs_ui/sgsui_GameButton.h \
	src/sgs_ui/sgsui_GameCheckBox.h \
	src/sgs_ui/sgsui_GuanxingBox.h \
	src/sgs_ui/sgsui_InitiateBox.h \
	src/sgs_ui/sgsui_LineAnimationWidget.h \
	src/sgs_ui/sgsui_MainWindow.h \
	src/sgs_ui/sgsui_MessageBox.h \
	src/sgs_ui/sgsui_PixmapAnimationWidget.h \
	src/sgs_ui/sgsui_PlayerArea.h \
	src/sgs_ui/sgsui_Shoupai.h \
	src/sgs_ui/sgsui_SkillButton.h \
	src/sgs_ui/sgsui_SmartStatusBar.h \
	src/sgs_ui/sgsui_WujiangArea.h \
	src/sgs_ui/sgsui_WujiangButton.h \
	src/sgs_ui/sgsui_WujiangChooseBox.h \
	src/sgs_ui/sgsui_ZhuangbeiArea.h \
	src/sgs_ui/sgsui_ZhuangbeiPic.h \
	src/sgs_ui/sgsui_nameConvert.h \
	src/sgs_ui/sgsui_output.h \
	src/sgs_data/sgsdata_card.h \
	src/sgs_data/sgsdata_skill_func.h \
	src/sgs_data/sgsdata_card_func.h \
	src/sgs_data/sgsdata_enum.h \
	src/sgs_data/sgsdata_extern.h \
	src/sgs_data/sgsdata_gamecard.h \
	src/sgs_data/sgsdata_gamedata.h \
	src/sgs_data/sgsdata_hero.h \
	src/sgs_data/sgsdata_heroes.h \
	src/sgs_data/sgsdata_history.h \
	src/sgs_data/sgsdata_manager.h \
	src/sgs_data/sgsdata_msg.h \
	src/sgs_data/sgsdata_namespace.h \
	src/sgs_data/sgsdata_platform.h \
	src/sgs_data/sgsdata_player.h \
	src/sgs_data/sgsdata_skill.h \
	src/sgs_ai/sgsai_AIBasic.h \
	src/sgs_ai/sgsai_AISujiang.h \
	src/sgs_ai/sgsai_AIDiaochan.h \
	src/sgs_ai/sgsai_CardSort.h \
	src/sgs_ai/sgsai_AILvbu.h \
	src/sgs_ai/sgsai_AIHuatuo.h \
	src/sgs_ai/sgsai_AIZhenji.h \
	src/sgs_ai/sgsai_AIGuojia.h \
	src/sgs_ai/sgsai_AIXuchu.h \
	src/sgs_ai/sgsai_AIZhangliao.h \
	src/sgs_ai/sgsai_AIXiahoudun.h \
	src/sgs_ai/sgsai_AISimayi.h \
	src/sgs_ai/sgsai_AISunshangxiang.h \
	src/sgs_ai/sgsai_AILuxun.h \
	src/sgs_ai/sgsai_AIDaqiao.h \
	src/sgs_ai/sgsai_AIZhouyu.h \
	src/sgs_ai/sgsai_AIHuanggai.h \
	src/sgs_ai/sgsai_AILvmeng.h \
	src/sgs_ai/sgsai_AIGanning.h \
	src/sgs_ai/sgsai_AIHuangyueying.h \
	src/sgs_ai/sgsai_AIMachao.h \
	src/sgs_ai/sgsai_AIZhaoyun.h \
	src/sgs_ai/sgsai_AIZhugeliang.h \
	src/sgs_ai/sgsai_AIZhangfei.h \
	src/sgs_ai/sgsai_AIGuanyu.h \
	src/sgs_ai/sgsai_AISunquan.h \
	src/sgs_ai/sgsai_AILiubei.h \
	src/sgs_ai/sgsai_AICaocao.h \
	src/sgs_ai/sgsai_AICommon.h \
	src/chain.h \
	src/sgs_Interface.h

SOURCES += \
	src/sgs_ui/sgsui_RoleViewer.cpp \
	src/sgs_ui/sgsui_CardViewer.cpp \
	src/sgs_ui/sgsui_MainWindow_Initiate.cpp \
	src/sgs_ui/sgsui_MainWindow_Normalize.cpp \
	src/sgs_ui/sgsui_MainWindow_Response.cpp \
	src/sgs_ui/sgsui_MainWindow_Collect.cpp \
	src/sgs_ui/sgsui_PlayerSort.cpp \
	src/sgs_ui/sgsui_ZhuangbeiLabel.cpp \
	src/sgs_ui/sgsui_RoleFrame.cpp \
	src/sgs_ui/sgsui_MainWindow_Display.cpp \
	src/sgs_ui/sgsui_SkillLabel.cpp \
	src/sgs_ui/sgsui_TextBrowser.cpp \
	src/sgs_ui/sgsui_Outstream.cpp \
	src/sgs_ui/sgsui_BlockLoop.cpp \
	src/sgs_ui/sgsui_ResultPlayerLabel.cpp \
	src/sgs_ui/sgsui_HelpBox.cpp \
	src/sgs_ui/sgsui_GameOverBox.cpp \
	src/sgs_ui/sgsui_PosAnimation.cpp \
	src/sgs_ui/sgsui_BlockTimer.cpp \
	src/sgs_ui/sgsui_FanjianBox.cpp \
	src/sgs_ui/sgsui_WugufengdengBox.cpp \
	src/sgs_ui/sgsui_global.cpp \
	src/sgs_ui/sgsui_AboutFrame.cpp \
	src/sgs_ui/sgsui_AudioPlayer.cpp \
	src/sgs_ui/sgsui_BlockableButton.cpp \
	src/sgs_ui/sgsui_BlockingFrame.cpp \
	src/sgs_ui/sgsui_CardButton.cpp \
	src/sgs_ui/sgsui_CardContainer.cpp \
	src/sgs_ui/sgsui_CardFrame.cpp \
	src/sgs_ui/sgsui_DialogBox.cpp \
	src/sgs_ui/sgsui_DragableCardFrame.cpp \
	src/sgs_ui/sgsui_GUIStaticData.cpp \
	src/sgs_ui/sgsui_GameButton.cpp \
	src/sgs_ui/sgsui_GameCheckBox.cpp \
	src/sgs_ui/sgsui_GuanxingBox.cpp \
	src/sgs_ui/sgsui_InitiateBox.cpp \
	src/sgs_ui/sgsui_LineAnimationWidget.cpp \
	src/sgs_ui/sgsui_MainWindow.cpp \
	src/sgs_ui/sgsui_MainWindow_Debug.cpp \
	src/sgs_ui/sgsui_MessageBox.cpp \
	src/sgs_ui/sgsui_PixmapAnimationWidget.cpp \
	src/sgs_ui/sgsui_PlayerArea.cpp \
	src/sgs_ui/sgsui_Shoupai.cpp \
	src/sgs_ui/sgsui_SkillButton.cpp \
	src/sgs_ui/sgsui_SmartStatusBar.cpp \
	src/sgs_ui/sgsui_WujiangArea.cpp \
	src/sgs_ui/sgsui_WujiangButton.cpp \
	src/sgs_ui/sgsui_WujiangChooseBox.cpp \
	src/sgs_ui/sgsui_ZhuangbeiArea.cpp \
	src/sgs_ui/sgsui_ZhuangbeiPic.cpp \
	src/sgs_ui/sgsui_nameConvert.cpp \
	src/sgs_ui/sgsui_output.cpp \
	src/sgs_data/sgsdata_skill_func.cpp \
	src/sgs_data/sgsdata_skill.cpp \
	src/sgs_data/sgsdata_card.cpp \
	src/sgs_data/sgsdata_card_func.cpp \
	src/sgs_data/sgsdata_extern.cpp \
	src/sgs_data/sgsdata_gamecard.cpp \
	src/sgs_data/sgsdata_gamedata.cpp \
	src/sgs_data/sgsdata_hero.cpp \
	src/sgs_data/sgsdata_heroes.cpp \
	src/sgs_data/sgsdata_history.cpp \
	src/sgs_data/sgsdata_manager.cpp \
	src/sgs_data/sgsdata_msg.cpp \
	src/sgs_data/sgsdata_platform.cpp \
	src/sgs_data/sgsdata_player.cpp \
	src/sgs_data/sgsdata_main.cpp \
	src/sgs_ai/sgsai_AISujiang.cpp \
	src/sgs_ai/sgsai_AIBasic.cpp \
	src/sgs_ai/sgsai_CardSort.cpp \
	src/sgs_ai/sgsai_AIBasic_Algorithm.cpp \
	src/sgs_ai/sgsai_AIDiaochan.cpp \
	src/sgs_ai/sgsai_AILvbu.cpp \
	src/sgs_ai/sgsai_AIHuatuo.cpp \
	src/sgs_ai/sgsai_AIZhenji.cpp \
	src/sgs_ai/sgsai_AIGuojia.cpp \
	src/sgs_ai/sgsai_AIXuchu.cpp \
	src/sgs_ai/sgsai_AIZhangliao.cpp \
	src/sgs_ai/sgsai_AIXiahoudun.cpp \
	src/sgs_ai/sgsai_AISimayi.cpp \
	src/sgs_ai/sgsai_AISunshangxiang.cpp \
	src/sgs_ai/sgsai_AILuxun.cpp \
	src/sgs_ai/sgsai_AIDaqiao.cpp \
	src/sgs_ai/sgsai_AIZhouyu.cpp \
	src/sgs_ai/sgsai_AIHuanggai.cpp \
	src/sgs_ai/sgsai_AILvmeng.cpp \
	src/sgs_ai/sgsai_AIGanning.cpp \
	src/sgs_ai/sgsai_AIHuangyueying.cpp \
	src/sgs_ai/sgsai_AIMachao.cpp \
	src/sgs_ai/sgsai_AIZhaoyun.cpp \
	src/sgs_ai/sgsai_AIZhugeliang.cpp \
	src/sgs_ai/sgsai_AIZhangfei.cpp \
	src/sgs_ai/sgsai_AIGuanyu.cpp \
	src/sgs_ai/sgsai_AISunquan.cpp \
	src/sgs_ai/sgsai_AILiubei.cpp \
	src/sgs_ai/sgsai_AICaocao.cpp \
	src/sgs_ai/sgsai_AICommon.cpp \
	src/sgs_Interface.cpp \
	src/main.cpp

FORMS += \
	src/sgs_ui/RoleViewer.ui \
	src/sgs_ui/CardViewer.ui \
	src/sgs_ui/RoleFrame.ui \
	src/sgs_ui/HelpBox.ui \
	src/sgs_ui/PlayerArea.ui \
	src/sgs_ui/GameOverBox.ui \
	src/sgs_ui/FanjianBox.ui \
	src/sgs_ui/WugufengdengBox.ui \
	src/sgs_ui/AboutFrame.ui \
	src/sgs_ui/CardButton.ui \
	src/sgs_ui/CardContainer.ui \
	src/sgs_ui/DialogBox.ui \
	src/sgs_ui/GuanxingBox.ui \
	src/sgs_ui/InitiateBox.ui \
	src/sgs_ui/MainWindow.ui \
	src/sgs_ui/WujiangArea.ui \
	src/sgs_ui/WujiangChooseBox.ui \
	src/sgs_ui/ZhuangbeiArea.ui

INCLUDEPATH += src
INCLUDEPATH += src/sgs_ui
INCLUDEPATH += src/sgs_ai
INCLUDEPATH += src/sgs_data
