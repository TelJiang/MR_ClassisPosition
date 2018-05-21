#ifndef HOSTCOMPUTERMAINWINDOW_H
#define HOSTCOMPUTERMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextBrowser>
#include <QGridLayout>
#include  <QHBoxLayout>
#include <QSizePolicy>
#include <QSpinBox>
#include <QLineEdit>
#include <QTimer>
#include <QScrollBar>
#include <QCloseEvent>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>
#include <QRegExp>
#include <QTimerEvent>
#include "serialportthread.h"
#include "camerathread.h"
#include "configurationwidget.hpp"
#include "mymath.hpp"
class HostComputerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	HostComputerMainWindow(QWidget *parent = 0);
	~HostComputerMainWindow();

	//软件外观设计
	void designUI();
	void createActions();
	void createMenus();
	void createToolBars();
	void createSerialPortConfWidget();
	void createOrdinarySerialPortTransferTab();
	void createProtocolSerialPortTransferTab();
	void createSerialPortReceiveAreaWidget();
	void createTipWidget();
	void createSerialPortTxRxInfoWidget();
	void createWholeLayout();
	void loadQSSFile(const QString style);

	void signalsAndSlots();

	void init();

	void writeTips(QString tip);
	void writeTips(QStringList tips);

	void showWarningInfo(QString info);

	void closeEvent(QCloseEvent *event);

	void delayMsec(int msecs);

	void timerEvent(QTimerEvent *event);
private slots:

	void displayDataUpdate();
	void transportUpdate();
	void getConfInfo(QStringList availableSerialPortName, QStringList availableCameraName,
		QStringList customSerialPortName, QStringList customCameraName, QList<int> transferRole);
	void showError(QString info);

	void on_confPeripheral_action_triggered();
	void on_searchSerialPort_action_triggered();
	void on_searchCamera_action_triggered();
	void on_openCamera_action_triggered();
	void on_startHostComputer_action_triggered();
	void on_oneButtonStart_action_triggered();

	void on_isHex_CheckBox_clicked();
	void on_isTimestamp_CheckBox_clicked();
	void on_isWrap_CheckBox_clicked();
	void on_ordinaryDelayedTx_clicked();
	void on_ordinarySendByHex_clicked();
	void on_protocolDelayedTx_clicked();
	void on_protocolSendByHex_clicked();

	void on_saveReceiveData_PushButton_clicked();
	void on_clearReceiveData_PushButton_clicked();
	void on_openSerialPort_PushButton_clicked();
	void on_ordinarySerialPortSend_PushButton_clicked();
	void on_protocolSerialPortSend_PushButton_clicked();

	void on_serialPortDescription_ComboBox_currentIndexChanged(int index);
	void on_ordinaryCurrentSerialPort_ComboBox_currentIndexChanged(int index);
	void on_protocolCurrentSerialPort_ComboBox_currentIndexChanged(int index);

	void on_displayReceiveArea_TabWidget_currentChanged(int index);

	void on_ordinarySendArea_PlainEditText_textChanged();
private:

	QMenu *conf_menu;

	QToolBar *toolBar;

	QAction *confPeripheral_action;
	QAction *searchSerialPort_action;
	QAction *searchCamera_action;
	QAction *openCamera_action;
	QAction *startHostComputer_action;
	QAction *oneButtonStart_action;

	QLabel *serialPortChoice_Label;
	QLabel *baudRate_Label;
	QLabel *dataBits_Label;
	QLabel *stopBits_Label;
	QLabel *parityBits_Label;
	QLabel *flowControl_Label;
	QLabel *ordinaryPeriodUnit_Label;
	QLabel *ordinaryCurrentSerialPort_Label;
	QLabel *protocolPeriodUnit_Label;
	QLabel *protocolCurrentSerialPort_Label;
	QLabel *protocolSenderID_Label;
	QLabel *protocolDataID_Label;
	QLabel *protocolData_Label;
	QLabel *protocolCRC_Label;
	QLabel *readInfo_Label;
	QLabel *writeInfo_Label;

	QComboBox *serialPortDescription_ComboBox;
	QComboBox *baudRate_ComboBox;
	QComboBox *dataBits_ComboBox;
	QComboBox *stopBits_ComboBox;
	QComboBox *parityBits_ComboBox;
	QComboBox *flowControl_ComboBox;
	QComboBox *ordinaryCurrentSerialPort_ComboBox;
	QComboBox *protocolCurrentSerialPort_ComboBox;
	QComboBox *protocolSenderID_ComboBox;
	QComboBox *protocolDataID_ComboBox;

	QCheckBox *isHex_CheckBox;
	QCheckBox *isTimestamp_CheckBox;
	QCheckBox *isWrap_CheckBox;
	QCheckBox *ordinaryDelayedTx_CheckBox;
	QCheckBox *ordinarySendHex_CheckBox;
	QCheckBox *protocolDelayedTx_CheckBox;
	QCheckBox *protocolSendHex_CheckBox;

	QPushButton *saveReceiveData_PushButton;
	QPushButton *clearReceiveData_PushButton;
	QPushButton *openSerialPort_PushButton;
	QPushButton *ordinarySerialPortSend_PushButton;
	QPushButton *protocolSerialPortSend_PushButton;

	QLineEdit *protocolData_LineEdit;
	QLineEdit *protocolCRC_LineEdit;
	QLineEdit *readInfo_LineEdit;
	QLineEdit *writeInfo_LineEdit;


	QGroupBox *manageSerialPort_GroupBox;
	QGroupBox *manageTransmissonAndTip_GroupBox;
	QGroupBox *readWriteInfo_GroupBox;

	QGridLayout *manageSerialPortGroupBox_GridLayout;
	QGridLayout *centralWidget_Gridlayout;
	QGridLayout *ordinaryTransportWidget_GridLayout;
	QGridLayout *protocolTransportWidget_GridLayout;

	QGridLayout *manageTransmissonAndTipGroupBox_GridLayout;

	QHBoxLayout *readWriteInfoGroupBox_HBoxLayout;

	QTabWidget *displayReceivedData_TabWidget;
	QTabWidget *manageSerialPortTransmission_TabWidget;

	QList<QPlainTextEdit*> serialPortReceiveArea_PlainTextEdit;
	QList<QPlainTextEdit*> cameraReceiveArea_PlainTextEdit;
	QPlainTextEdit * ordinarySendArea_PlainTextEdit;
	QTextBrowser *tip_TextBrowser;

	QSpinBox *ordinarySendTimes_SpinBox;
	QSpinBox *protocolSendTimes_SpinBox;

	QWidget *centralWidget_Widget;
	QWidget *manageOrdinaryTransport_Widget;
	QWidget *manageProtocolTransport_Widget;

	QTimer displayDataUpdateTimer;
	QTimer transportUpdateTimer;

	QList<SerialPortThread*> mySerialPortThread;
	int availableSerialPortNum;
	QStringList availableSerialPortName;

	//串口状态参数的记录
	QList<int> baudRate;
	QList<int> dataBits;
	QList<int> stopBits;
	QList<int> parityBits;
	QList<int> flowControl;
	QList<bool> isHexShow;
	QList<bool> isTimestamp;
	QList<bool> isWrap;
	QList<bool> isOpenSerialPort;
	QList<bool> isOrdinaryDelayedTx;
	QList<bool> isOrdinarySendByHex;
	QList<bool> isOrdinarySend;
	QList<bool> isProtocolDelayedTx;
	QList<bool> isProtocolSendByHex;
	QList<bool> isProtocolSend;
	QList<int> ordinarySendPeriod;
	QList<int> protocolSendPeriod;
	QList<QString> ordinarySendPlainTextEdit;
	QList<int> protocolSenderID;
	QList<int> protocolDataID;
	QList<QString> protocolData;
	QList<QString> protocolCRC;
	QList<int> txNum, rxNum;

	bool isClearReceivedData;
	int lastSerialPortIndex;

	QList<CameraThread*> myCameraThread;
	int availableCameraNum;
	QStringList availableCameraName;

	ConfigurationWidget *confWidget;

	QStringList customSerialPortName, customCameraName;
	QList<int> transferRole;
	int roleLength;
	QList<int> senderIndex, receiverIndex;
	QList<int> role_serialPortIndex;
	QList<int> role_cameraIndex;

	QRegExp *hex_RegExp;

	MyMath myMath;

	QList<int> ordinaryTimerId, protocolTimerId;

};

#endif // HOSTCOMPUTERMAINWINDOW_H
