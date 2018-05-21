#include "hostcomputermainwindow.h"
#pragma execution_character_set("utf-8")
//************************************
// ������:   	HostComputerMainWindow
// ���ʿ���:   	public 
// ����ֵ:   	
// �β�: 		QWidget * parent
// ���ߣ�		���Ͽ�����
// ˵����		���캯��
// ���ڣ�		2018/4/4 18:48:52
//************************************
HostComputerMainWindow::HostComputerMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//������UI
	designUI();

	//�źźͲ۵�����
	signalsAndSlots();

	//��ʼ������
	init();

	//��������
	oneButtonStart_action->setChecked(true);
	on_oneButtonStart_action_triggered();
}

//************************************
// ������:   	~HostComputerMainWindow
// ���ʿ���:   	public 
// ����ֵ:   	
// ���ߣ�		���Ͽ�����
// ˵����		��������
// ���ڣ�		2018/4/4 18:49:08
//************************************
HostComputerMainWindow::~HostComputerMainWindow()
{

}

//************************************
// ������:   	designUI
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		������UI
// ���ڣ�		2018/4/4 18:47:01
//************************************
void HostComputerMainWindow::designUI()
{
	HostComputerMainWindow::setWindowTitle(tr("Host Computer"));
	HostComputerMainWindow::setFont(QFont("΢���ź�", 10));
	HostComputerMainWindow::resize(750, 550);
	HostComputerMainWindow::setWindowIcon(QIcon(":/Icon/test.ico"));
	createActions();
	createMenus();
	createToolBars();

	//�������Ĵ���
	centralWidget_Widget = new QWidget;
	HostComputerMainWindow::setCentralWidget(centralWidget_Widget);

	createSerialPortConfWidget();
	createOrdinarySerialPortTransferTab();
	createProtocolSerialPortTransferTab();

	manageSerialPortTransmission_TabWidget = new QTabWidget(this);
	manageSerialPortTransmission_TabWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	manageSerialPortTransmission_TabWidget->addTab(manageOrdinaryTransport_Widget, tr("ordinary transport"));
	//manageSerialPortTransmission_TabWidget->addTab(manageProtocolTransport_Widget, tr("protocol transport"));

	createSerialPortReceiveAreaWidget();
	createTipWidget();
	createSerialPortTxRxInfoWidget();
	createWholeLayout();
}

//************************************
// ������:   	createActions
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��������
// ���ڣ�		2018/4/4 18:47:19
//************************************
void HostComputerMainWindow::createActions()
{
	confPeripheral_action = new QAction(tr("peripheral"), this);
	confPeripheral_action->setStatusTip(tr("Configure Peripheral"));

	searchSerialPort_action = new QAction(QIcon(":/Icon/searchSerialPort.png"), tr("search serial port"), this);
	searchSerialPort_action->setStatusTip(tr("search serial port"));

	searchCamera_action = new QAction(QIcon(":/Icon/searchCamera.png"), tr("search camera"), this);
	searchCamera_action->setStatusTip(tr("search camera"));

	openCamera_action = new QAction(QIcon(":/Icon/openCamera.png"), tr("open camera"), this);
	openCamera_action->setStatusTip(tr("open camera"));
	openCamera_action->setCheckable(true);

	startHostComputer_action = new QAction(QIcon(":/Icon/run.png"), tr("start the host computer"), this);
	startHostComputer_action->setStatusTip(tr("start the host computer"));
	startHostComputer_action->setCheckable(true);

	oneButtonStart_action = new QAction(QIcon(":/Icon/oneButtonStart.png"), tr("one button start"), this);
	oneButtonStart_action->setStatusTip(tr("one button start"));
	oneButtonStart_action->setCheckable(true);
}

//************************************
// ������:   	createMenus
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�����˵���
// ���ڣ�		2018/4/4 18:47:42
//************************************
void HostComputerMainWindow::createMenus()
{
	conf_menu = menuBar()->addMenu(tr("configuation"));
	conf_menu->addAction(confPeripheral_action);
}

//************************************
// ������:   	createToolBars
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		����������
// ���ڣ�		2018/4/4 18:48:00
//************************************
void HostComputerMainWindow::createToolBars()
{
	toolBar = addToolBar(tr("tool"));
	toolBar->setMovable(false);

	toolBar->addAction(searchSerialPort_action);
	toolBar->addAction(searchCamera_action);
	toolBar->addAction(openCamera_action);
	toolBar->addSeparator();
	toolBar->addAction(startHostComputer_action);
	toolBar->addAction(oneButtonStart_action);
}

//************************************
// ������:   	createSerialPortConfWidget
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�������ڲ��������Ӵ���
// ���ڣ�		2018/4/4 21:11:29
//************************************
void HostComputerMainWindow::createSerialPortConfWidget()
{
	serialPortChoice_Label = new QLabel(this);
	serialPortChoice_Label->setText(tr("choose serial port :"));

	serialPortChoice_Label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	baudRate_Label = new QLabel(this);
	baudRate_Label->setText(tr("baud rate :"));

	dataBits_Label = new QLabel(this);
	dataBits_Label->setText(tr("data bits :"));

	stopBits_Label = new QLabel(this);
	stopBits_Label->setText(tr("stop bits :"));

	parityBits_Label = new QLabel(this);
	parityBits_Label->setText(tr("parity bits :"));

	flowControl_Label = new QLabel(this);
	flowControl_Label->setText(tr("flow control :"));

	serialPortDescription_ComboBox = new QComboBox(this);

	baudRate_ComboBox = new QComboBox(this);
	baudRate_ComboBox->addItem("115200");
	baudRate_ComboBox->addItem("57600");
	baudRate_ComboBox->addItem("38400");
	baudRate_ComboBox->addItem("19200");
	baudRate_ComboBox->addItem("9600");
	baudRate_ComboBox->addItem("4800");
	baudRate_ComboBox->addItem("2400");
	baudRate_ComboBox->addItem("1200");

	dataBits_ComboBox = new QComboBox(this);
	dataBits_ComboBox->addItem("8");
	dataBits_ComboBox->addItem("7");
	dataBits_ComboBox->addItem("6");
	dataBits_ComboBox->addItem("5");

	stopBits_ComboBox = new QComboBox(this);
	stopBits_ComboBox->addItem("1");
	stopBits_ComboBox->addItem("1.5");
	stopBits_ComboBox->addItem("2");

	parityBits_ComboBox = new QComboBox(this);
	parityBits_ComboBox->addItem("none");
	parityBits_ComboBox->addItem("odd");
	parityBits_ComboBox->addItem("even");

	flowControl_ComboBox = new QComboBox(this);
	flowControl_ComboBox->addItem("none");
	flowControl_ComboBox->addItem("Hardware");
	flowControl_ComboBox->addItem("Sofeware");

	isHex_CheckBox = new QCheckBox(this);
	isHex_CheckBox->setText(tr("hex show"));

	isTimestamp_CheckBox = new QCheckBox(this);
	isTimestamp_CheckBox->setText(tr("timestamp"));

	isWrap_CheckBox = new QCheckBox(this);
	isWrap_CheckBox->setText(tr("wrap"));

	saveReceiveData_PushButton = new QPushButton(this);
	saveReceiveData_PushButton->setText(tr("save Data"));

	clearReceiveData_PushButton = new QPushButton(this);
	clearReceiveData_PushButton->setText(tr("clear"));

	openSerialPort_PushButton = new QPushButton(this);
	openSerialPort_PushButton->setText(tr("open serial port"));

	manageSerialPort_GroupBox = new QGroupBox(this);
	manageSerialPortGroupBox_GridLayout = new QGridLayout(manageSerialPort_GroupBox);

	manageSerialPortGroupBox_GridLayout->addWidget(serialPortChoice_Label, 0, 0, 1, 2);
	manageSerialPortGroupBox_GridLayout->addWidget(serialPortDescription_ComboBox, 1, 0, 1, 2);
	manageSerialPortGroupBox_GridLayout->addWidget(baudRate_Label, 2, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(baudRate_ComboBox, 2, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(dataBits_Label, 3, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(dataBits_ComboBox, 3, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(stopBits_Label, 4, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(stopBits_ComboBox, 4, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(parityBits_Label, 5, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(parityBits_ComboBox, 5, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(flowControl_Label, 6, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(flowControl_ComboBox, 6, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(clearReceiveData_PushButton, 7, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(saveReceiveData_PushButton, 7, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(openSerialPort_PushButton, 8, 1);
	manageSerialPortGroupBox_GridLayout->addWidget(isHex_CheckBox, 8, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(isWrap_CheckBox, 9, 0);
	manageSerialPortGroupBox_GridLayout->addWidget(isTimestamp_CheckBox, 9, 1);
}

//************************************
// ������:   	createOrdinarySerialPortTransferTab
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		������ͨ���ڷ��ͱ�ǩ��
// ���ڣ�		2018/4/4 21:12:52
//************************************
void HostComputerMainWindow::createOrdinarySerialPortTransferTab()
{
	ordinaryPeriodUnit_Label = new QLabel(this);
	ordinaryPeriodUnit_Label->setText(tr("ms/a"));

	ordinaryCurrentSerialPort_Label = new QLabel(this);
	ordinaryCurrentSerialPort_Label->setText(tr("current serial port:"));

	ordinarySendArea_PlainTextEdit = new QPlainTextEdit(this);

	ordinarySerialPortSend_PushButton = new QPushButton(this);
	ordinarySerialPortSend_PushButton->setText(tr("send"));

	ordinarySendTimes_SpinBox = new QSpinBox(this);
	ordinarySendTimes_SpinBox->setRange(1, 50000);
	ordinarySendTimes_SpinBox->setValue(1000);

	ordinaryDelayedTx_CheckBox = new QCheckBox(this);
	ordinaryDelayedTx_CheckBox->setText(tr("delayed tx"));

	ordinarySendHex_CheckBox = new QCheckBox(this);
	ordinarySendHex_CheckBox->setText(tr("send by hex"));

	ordinaryCurrentSerialPort_ComboBox = new QComboBox(this);

	manageOrdinaryTransport_Widget = new QWidget;
	ordinaryTransportWidget_GridLayout = new QGridLayout(manageOrdinaryTransport_Widget);
	ordinaryTransportWidget_GridLayout->addWidget(ordinaryCurrentSerialPort_Label, 0, 0, 1, 1);
	ordinaryTransportWidget_GridLayout->addWidget(ordinaryCurrentSerialPort_ComboBox, 0, 1, 1, 3);
	ordinaryTransportWidget_GridLayout->addWidget(ordinarySendArea_PlainTextEdit, 1, 0, 2, 3);
	ordinaryTransportWidget_GridLayout->addWidget(ordinaryDelayedTx_CheckBox, 3, 0, 1, 1);
	ordinaryTransportWidget_GridLayout->addWidget(ordinarySerialPortSend_PushButton, 1, 3, 1, 1);
	ordinaryTransportWidget_GridLayout->addWidget(ordinarySendTimes_SpinBox, 3, 1, 1, 1);
	ordinaryTransportWidget_GridLayout->addWidget(ordinaryPeriodUnit_Label, 3, 2, 1, 1);
	ordinaryTransportWidget_GridLayout->addWidget(ordinarySendHex_CheckBox, 2, 3, 1, 1);
	ordinaryTransportWidget_GridLayout->setColumnStretch(0, 0);
	ordinaryTransportWidget_GridLayout->setColumnStretch(1, 0);
	ordinaryTransportWidget_GridLayout->setColumnStretch(2, 0);
	ordinaryTransportWidget_GridLayout->setColumnStretch(3, 0);
}

//************************************
// ������:   	createProtocolSerialPortTransferTab
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		����Э�鴮�ڷ��ͱ�ǩ��
// ���ڣ�		2018/4/4 21:20:28
//************************************
void HostComputerMainWindow::createProtocolSerialPortTransferTab()
{
	protocolPeriodUnit_Label = new QLabel(this);
	protocolPeriodUnit_Label->setText(tr("ms/a"));

	protocolCurrentSerialPort_Label = new QLabel(this);
	protocolCurrentSerialPort_Label->setText(tr("current serial port:"));

	protocolSenderID_Label = new QLabel(this);
	protocolSenderID_Label->setText(tr("sender id"));
	protocolSenderID_Label->setAlignment(Qt::AlignHCenter);

	protocolDataID_Label = new QLabel(this);
	protocolDataID_Label->setText(tr("data id"));
	protocolDataID_Label->setAlignment(Qt::AlignHCenter);

	protocolData_Label = new QLabel(this);
	protocolData_Label->setText(tr("data"));
	protocolData_Label->setAlignment(Qt::AlignHCenter);

	protocolCRC_Label = new QLabel(this);
	protocolCRC_Label->setText(tr("CRC8"));
	protocolCRC_Label->setAlignment(Qt::AlignHCenter);

	protocolCurrentSerialPort_ComboBox = new QComboBox(this);

	protocolSenderID_ComboBox = new QComboBox(this);

	protocolDataID_ComboBox = new QComboBox(this);

	protocolData_LineEdit = new QLineEdit(this);

	protocolCRC_LineEdit = new QLineEdit(this);
	protocolCRC_LineEdit->setReadOnly(true);

	protocolSerialPortSend_PushButton = new QPushButton(this);
	protocolSerialPortSend_PushButton->setText(tr("send"));

	protocolSendTimes_SpinBox = new QSpinBox(this);
	protocolSendTimes_SpinBox->setRange(1, 50000);
	protocolSendTimes_SpinBox->setValue(1000);

	protocolDelayedTx_CheckBox = new QCheckBox(this);
	protocolDelayedTx_CheckBox->setText(tr("delayed tx"));

	protocolSendHex_CheckBox = new QCheckBox(this);
	protocolSendHex_CheckBox->setText(tr("send by hex"));

	manageProtocolTransport_Widget = new QWidget;
	protocolTransportWidget_GridLayout = new QGridLayout(manageProtocolTransport_Widget);
	protocolTransportWidget_GridLayout->addWidget(protocolCurrentSerialPort_Label, 0, 0, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolCurrentSerialPort_ComboBox, 0, 1, 1, 3);
	protocolTransportWidget_GridLayout->addWidget(protocolSenderID_Label, 1, 0, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolDataID_Label, 1, 1, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolData_Label, 1, 2, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolCRC_Label, 1, 3, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolSenderID_ComboBox, 2, 0, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolDataID_ComboBox, 2, 1, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolData_LineEdit, 2, 2, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolCRC_LineEdit, 2, 3, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolDelayedTx_CheckBox, 3, 0, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolSendTimes_SpinBox, 4, 0, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolPeriodUnit_Label, 4, 1, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolSendHex_CheckBox, 3, 1, 1, 1);
	protocolTransportWidget_GridLayout->addWidget(protocolSerialPortSend_PushButton, 3, 2, 1, 1);
	protocolTransportWidget_GridLayout->setColumnStretch(0, 1);
	protocolTransportWidget_GridLayout->setColumnStretch(1, 1);
	protocolTransportWidget_GridLayout->setColumnStretch(2, 3);
	protocolTransportWidget_GridLayout->setColumnStretch(3, 1);
}

//************************************
// ������:   	createSerialPortReceiveAreaWidget
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�������ڽ�����
// ���ڣ�		2018/4/4 21:21:38
//************************************
void HostComputerMainWindow::createSerialPortReceiveAreaWidget()
{
	serialPortReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
	serialPortReceiveArea_PlainTextEdit[0]->setReadOnly(true);

	displayReceivedData_TabWidget = new QTabWidget(this);
	displayReceivedData_TabWidget->addTab(serialPortReceiveArea_PlainTextEdit[0], tr("receive area"));
}

//************************************
// ������:   	createTipWidget
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		������Ϣ��ʾ����
// ���ڣ�		2018/4/4 21:22:32
//************************************
void HostComputerMainWindow::createTipWidget()
{
	tip_TextBrowser = new QTextBrowser(this);
}

//************************************
// ������:   	createSerialPortTxRxInfoWidget
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����
// ���ڣ�		2018/4/4 21:23:10
//************************************
void HostComputerMainWindow::createSerialPortTxRxInfoWidget()
{
	readInfo_Label = new QLabel(this);
	readInfo_Label->setText(tr("Rx:"));//��λ������

	writeInfo_Label = new QLabel(this);
	writeInfo_Label->setText(tr("Tx:"));//��λ������

	readInfo_LineEdit = new QLineEdit(this);
	readInfo_LineEdit->setReadOnly(true);
	readInfo_LineEdit->setAlignment(Qt::AlignHCenter);

	writeInfo_LineEdit = new QLineEdit(this);
	writeInfo_LineEdit->setReadOnly(true);
	writeInfo_LineEdit->setAlignment(Qt::AlignHCenter);

	readWriteInfo_GroupBox = new QGroupBox(this);
	readWriteInfo_GroupBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

	readWriteInfoGroupBox_HBoxLayout = new QHBoxLayout(readWriteInfo_GroupBox);

	readWriteInfoGroupBox_HBoxLayout->addWidget(readInfo_Label);
	readWriteInfoGroupBox_HBoxLayout->addWidget(readInfo_LineEdit);
	readWriteInfoGroupBox_HBoxLayout->addWidget(writeInfo_Label);
	readWriteInfoGroupBox_HBoxLayout->addWidget(writeInfo_LineEdit);
}

//************************************
// ������:   	createWholeLayout
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�����ڵ����岼��
// ���ڣ�		2018/4/4 21:23:38
//************************************
void HostComputerMainWindow::createWholeLayout()
{
	manageTransmissonAndTip_GroupBox = new QGroupBox(this);

	manageTransmissonAndTipGroupBox_GridLayout = new QGridLayout(manageTransmissonAndTip_GroupBox);

	manageTransmissonAndTipGroupBox_GridLayout->addWidget(manageSerialPortTransmission_TabWidget, 0, 0);
	manageTransmissonAndTipGroupBox_GridLayout->addWidget(readWriteInfo_GroupBox, 1, 0);
	manageTransmissonAndTipGroupBox_GridLayout->addWidget(tip_TextBrowser, 0, 1, 2, 1);
	manageSerialPortTransmission_TabWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	manageTransmissonAndTipGroupBox_GridLayout->setColumnStretch(0, 1);
	manageTransmissonAndTipGroupBox_GridLayout->setColumnStretch(1, 2);
	manageTransmissonAndTipGroupBox_GridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

	//�������Ĵ��ڲ���
	centralWidget_Gridlayout = new QGridLayout(centralWidget_Widget);

	//�����Ĵ�����Ӳ���
	centralWidget_Gridlayout->addWidget(manageSerialPort_GroupBox, 0, 0);
	centralWidget_Gridlayout->addWidget(displayReceivedData_TabWidget, 0, 1);
	centralWidget_Gridlayout->addWidget(manageTransmissonAndTip_GroupBox, 1, 0, 1, 2);
	centralWidget_Gridlayout->setRowStretch(0, 2);
	centralWidget_Gridlayout->setRowStretch(1, 1);
}

//************************************
// ������:   	loadQSSFile
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		const QString style
// ���ߣ�		���Ͽ�����
// ˵����		����QSS�����ļ�
// ���ڣ�		2018/4/4 18:48:21
//************************************
void HostComputerMainWindow::loadQSSFile(const QString style)
{
	QFile qss(style);
	qss.open(QFile::ReadOnly);
	HostComputerMainWindow::setStyleSheet(qss.readAll());
	qss.close();
}

//************************************
// ������:   	HostComputerMainWindow::signalsAndSlots
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�źźͲ۵����ӵ�ʵ��
// ���ڣ�		2018/4/6 15:10:06
//************************************
void HostComputerMainWindow::signalsAndSlots()
{
	connect(confPeripheral_action, &QAction::triggered, this,
		&HostComputerMainWindow::on_confPeripheral_action_triggered);
	connect(searchSerialPort_action, &QAction::triggered, this,
		&HostComputerMainWindow::on_searchSerialPort_action_triggered);
	connect(searchCamera_action, &QAction::triggered, this, &HostComputerMainWindow::on_searchCamera_action_triggered);
	connect(openCamera_action, &QAction::triggered, this, &HostComputerMainWindow::on_openCamera_action_triggered);
	connect(startHostComputer_action, &QAction::triggered, this,
		&HostComputerMainWindow::on_startHostComputer_action_triggered);
	connect(oneButtonStart_action, &QAction::triggered, this,
		&HostComputerMainWindow::on_oneButtonStart_action_triggered);
	connect(isHex_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_isHex_CheckBox_clicked);
	connect(isTimestamp_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_isTimestamp_CheckBox_clicked);
	connect(isWrap_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_isWrap_CheckBox_clicked);
	connect(ordinaryDelayedTx_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_ordinaryDelayedTx_clicked);
	connect(ordinarySendHex_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_ordinarySendByHex_clicked);
	connect(protocolDelayedTx_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_protocolDelayedTx_clicked);
	connect(protocolSendHex_CheckBox, &QCheckBox::clicked, this, &HostComputerMainWindow::on_protocolSendByHex_clicked);
	connect(saveReceiveData_PushButton, &QPushButton::clicked, this,
		&HostComputerMainWindow::on_saveReceiveData_PushButton_clicked);
	connect(clearReceiveData_PushButton, &QPushButton::clicked, this,
		&HostComputerMainWindow::on_clearReceiveData_PushButton_clicked);
	connect(openSerialPort_PushButton, &QPushButton::clicked, this,
		&HostComputerMainWindow::on_openSerialPort_PushButton_clicked);
	connect(ordinarySerialPortSend_PushButton, &QPushButton::clicked, this,
		&HostComputerMainWindow::on_ordinarySerialPortSend_PushButton_clicked);
	connect(protocolSerialPortSend_PushButton, &QPushButton::clicked, this,
		&HostComputerMainWindow::on_protocolSerialPortSend_PushButton_clicked);
	connect(serialPortDescription_ComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
		&HostComputerMainWindow::on_serialPortDescription_ComboBox_currentIndexChanged);
	connect(ordinaryCurrentSerialPort_ComboBox, static_cast<void(QComboBox::*)(int)> (&QComboBox::currentIndexChanged),
		this, &HostComputerMainWindow::on_ordinaryCurrentSerialPort_ComboBox_currentIndexChanged);
	connect(protocolCurrentSerialPort_ComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &HostComputerMainWindow::on_protocolCurrentSerialPort_ComboBox_currentIndexChanged);
	connect(displayReceivedData_TabWidget, static_cast<void(QTabWidget::*)(int)>(&QTabWidget::currentChanged), this,
		&HostComputerMainWindow::on_displayReceiveArea_TabWidget_currentChanged);
	connect(&displayDataUpdateTimer, &QTimer::timeout, this, &HostComputerMainWindow::displayDataUpdate);
	connect(&transportUpdateTimer, &QTimer::timeout, this, &HostComputerMainWindow::transportUpdate);
	connect(ordinarySendArea_PlainTextEdit, &QPlainTextEdit::textChanged, this,
		&HostComputerMainWindow::on_ordinarySendArea_PlainEditText_textChanged);
}

//************************************
// ������:   	HostComputerMainWindow::init
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�����һЩ��ʼ������(������ʼ���������ʼ��������)
// ���ڣ�		2018/4/6 16:03:53
//************************************
void HostComputerMainWindow::init()
{
	hex_RegExp = new QRegExp("^[0-9a-fA-F]+$");//16���Ƶ�����

	availableSerialPortNum = 0;
	isClearReceivedData = false;
	lastSerialPortIndex = 0;
	mySerialPortThread << new SerialPortThread;
	mySerialPortThread[0]->searchSerialPort();
	availableSerialPortName = mySerialPortThread[0]->getSerialPortName();
	availableSerialPortNum = mySerialPortThread[0]->getSerialPortDescription().length();

	//���ݿ��ô�����Ŀ���������߳���Ķ���ĸ���
	//��֤һ�������Ĳ���
	if (availableSerialPortNum <= 0) {
		availableSerialPortNum = 1;
		availableSerialPortName << "receive area";
	}

	for (int i = 0; i < availableSerialPortNum; i++) {
		ordinaryTimerId << 0;
		protocolTimerId << 0;
		baudRate << 0;
		dataBits << 0;
		stopBits << 0;
		parityBits << 0;
		flowControl << 0;
		isHexShow << false;
		isTimestamp << false;
		isWrap << false;
		isOpenSerialPort << false;
		isOrdinaryDelayedTx << false;
		isOrdinarySendByHex << false;
		isOrdinarySend << false;
		isProtocolDelayedTx << false;
		isProtocolSendByHex << false;
		isProtocolSend << false;
		ordinarySendPeriod << 1000;
		protocolSendPeriod << 1000;
		ordinarySendPlainTextEdit << "";
		protocolSenderID << 0;
		protocolDataID << 0;
		protocolData << "";
		protocolCRC << "";
		txNum << 0;
		rxNum << 0;
	}

	serialPortDescription_ComboBox->addItems(mySerialPortThread[0]->getSerialPortDescription());
	ordinaryCurrentSerialPort_ComboBox->addItems(mySerialPortThread[0]->getSerialPortDescription());
	protocolCurrentSerialPort_ComboBox->addItems(mySerialPortThread[0]->getSerialPortDescription());

	//���䴮���̶߳����Լ����մ������ݵ���ʾҳ
	displayReceivedData_TabWidget->setTabText(0, availableSerialPortName.at(0));
	for (int i = 1; i < availableSerialPortNum; i++) {
		mySerialPortThread << new SerialPortThread;
		serialPortReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(serialPortReceiveArea_PlainTextEdit[i], availableSerialPortName.at(i));
	}

	availableCameraNum = 0;
	//��֤����ͷ���ܹ�����ʹ��
	myCameraThread << new CameraThread;

	writeTips(">> available camera��");
	availableCameraName = myCameraThread[0]->searchCamera();
	availableCameraNum = availableCameraName.length();
	if (availableCameraNum == 0) {
		writeTips(">> none");
	}
	else {
		writeTips(tr(">> %1").arg(availableCameraName.at(0)));
		cameraReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(cameraReceiveArea_PlainTextEdit[0], tr("����ͷ%1").arg(0));
	}
	for (int i = 1; i < availableCameraNum; i++) {
		writeTips(tr(">> %1").arg(availableCameraName.at(i)));
		myCameraThread << new CameraThread;
		cameraReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(cameraReceiveArea_PlainTextEdit[i], tr("����ͷ%1").arg(i));
	}

	//����������ʾ�Ķ�ʱ��
	displayDataUpdateTimer.start(25);
}

//************************************
// ������:   	HostComputerMainWindow::writeTips
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QString tip
// ���ߣ�		���Ͽ�����
// ˵����		����ʾ��дһ����Ϣ
// ���ڣ�		2018/4/6 16:40:51
//************************************
void HostComputerMainWindow::writeTips(QString tip)
{
	tip_TextBrowser->insertPlainText(tip + "\n");
	tip_TextBrowser->verticalScrollBar()->setValue(tip_TextBrowser->verticalScrollBar()->maximum());
}

//************************************
// ������:   	HostComputerMainWindow::writeTips
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QStringList tips
// ���ߣ�		���Ͽ�����
// ˵����		����ʾ��д������Ϣ
// ���ڣ�		2018/4/6 16:41:57
//************************************
void HostComputerMainWindow::writeTips(QStringList tips)
{
	for (int i = 0; i < tips.length(); i++) {
		tip_TextBrowser->insertPlainText(tips.at(i) + "\n");
		tip_TextBrowser->verticalScrollBar()->setValue(tip_TextBrowser->verticalScrollBar()->maximum());
	}
}

//************************************
// ������:   	HostComputerMainWindow::showWarningInfo
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QString info
// ���ߣ�		���Ͽ�����
// ˵����		��ʾһЩ��������Ϣ
// ���ڣ�		2018/4/8 21:39:37
//************************************
void HostComputerMainWindow::showWarningInfo(QString info)
{
	QMessageBox::warning(this, "warning", info, QMessageBox::Ok);
}

void HostComputerMainWindow::closeEvent(QCloseEvent * event)
{
	if (startHostComputer_action->isChecked()) {
		transportUpdateTimer.stop();
	}
	//������ڴ��ˣ��͹ر���
	for (int i = 0; i < availableSerialPortNum; i++) {
		if (isOpenSerialPort[i]) {
			mySerialPortThread[i]->closeSerialPortThread();
		}
	}
	//�������ͷ���ˣ��͹ر���
	if (openCamera_action->isChecked()) {
		openCamera_action->setChecked(false);
		on_openCamera_action_triggered();
		delayMsec(500);//��֤����ͷ�����ر�
	}
}
//************************************
// ������:   	HostComputerMainWindow::delayMsec
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		int msecs
// ���ߣ�		���Ͽ�����
// ˵����		ʵ�ֺ��뼶����ʱ
// ���ڣ�		2018/4/12 17:32:34
//************************************
void HostComputerMainWindow::delayMsec(int msecs)
{
	QTime currentTime, lastTime;
	lastTime = QTime::currentTime();
	currentTime = QTime::currentTime();
	while (lastTime.msecsTo(currentTime) < msecs) {
		currentTime = QTime::currentTime();
	}
	qDebug() << lastTime.msecsTo(currentTime);
}

//************************************
// ������:   	HostComputerMainWindow::timerEvent
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QTimerEvent * event
// ���ߣ�		���Ͽ�����
// ˵����		��д��ʱ���¼�
// ���ڣ�		2018/4/14 09:41:23
//************************************
void HostComputerMainWindow::timerEvent(QTimerEvent * event)
{
	if (ordinaryTimerId.contains(event->timerId())) {
		mySerialPortThread[ordinaryTimerId.indexOf(event->timerId())]->writeData(
			ordinarySendPlainTextEdit[ordinaryTimerId.indexOf(event->timerId())].toLocal8Bit());
	}
	else {

	}
}
//************************************
// ������:   	HostComputerMainWindow::displayDataUpdate
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		����ˢ�½��棬����������ʾ
// ���ڣ�		2018/4/9 09:02:03
//************************************
void HostComputerMainWindow::displayDataUpdate()
{
	for (int i = 0; i < availableSerialPortNum; i++) {
		mySerialPortThread[i]->isWrapOrHexShowOrTimeStamp(isWrap[i], isHexShow[i], isTimestamp[i]);
		serialPortReceiveArea_PlainTextEdit[i]->insertPlainText(mySerialPortThread[i]->showData(true));
		if (isOpenSerialPort[i]) {
			serialPortReceiveArea_PlainTextEdit[i]->verticalScrollBar()->setValue(
				serialPortReceiveArea_PlainTextEdit[i]->verticalScrollBar()->maximum());
			txNum[i] = mySerialPortThread[i]->writeBytes(false);
			rxNum[i] = mySerialPortThread[i]->readBytes(false);
		}
	}
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		readInfo_LineEdit->setText(tr("%1").arg(rxNum[serialPortDescription_ComboBox->currentIndex()]));
		writeInfo_LineEdit->setText(tr("%1").arg(txNum[serialPortDescription_ComboBox->currentIndex()]));
	}
	for (int i = 0; i < availableCameraNum; i++) {
		cameraReceiveArea_PlainTextEdit[i]->insertPlainText(myCameraThread[i]->showCameraData(true));
		if (openCamera_action->isChecked()) {
			cameraReceiveArea_PlainTextEdit[i]->verticalScrollBar()->setValue(
				cameraReceiveArea_PlainTextEdit[i]->verticalScrollBar()->maximum());
		}
	}
	update();
}

void HostComputerMainWindow::transportUpdate()
{
	for (int i = 0; i < roleLength; i++) {
		if (senderIndex.at(i) < availableSerialPortNum) {//�������Ǵ���
			if (receiverIndex.at(i) < availableSerialPortNum) {//�������Ǵ���
				mySerialPortThread[receiverIndex.at(i)]->writeData(
					mySerialPortThread[senderIndex.at(i)]->transferData(true));
			}
			else {//������������ͷ
				myCameraThread[receiverIndex.at(i) - availableSerialPortNum]->getParameter(
					mySerialPortThread[senderIndex.at(i)]->transferData(true));
			}
		}
		else {//������������ͷ
			if (receiverIndex.at(i) < availableSerialPortNum) {//�������Ǵ���
				mySerialPortThread[receiverIndex.at(i)]->writeData(
					myCameraThread[senderIndex.at(i) - availableSerialPortNum]->transferCameraData(true));
			}
			else {//������������ͷ
				myCameraThread[receiverIndex.at(i) - availableSerialPortNum]->getParameter(
					myCameraThread[senderIndex.at(i) - availableSerialPortNum]->transferCameraData(true));
			}
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::getConfInfo
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		QStringList availableSerialPortName
// �β�: 		QStringList availableCameraName
// �β�: 		QStringList customSerialPortName
// �β�: 		QStringList customCameraName
// �β�: 		QList<int> transferRole
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ���õ������Ϣ��������һЩ��Ҫ������������Դ
// ���ڣ�		2018/4/10 21:18:22
//************************************
void HostComputerMainWindow::getConfInfo(QStringList availableSerialPortName, QStringList availableCameraName,
	QStringList customSerialPortName, QStringList customCameraName, QList<int> transferRole)
{
	displayDataUpdateTimer.stop();
	//����Զ����俪ʼ�ˣ��͹ر���
	if (startHostComputer_action->isChecked()) {
		transportUpdateTimer.stop();
	}
	//������ڴ��ˣ��͹ر���
	for (int i = 0; i < this->availableSerialPortNum; i++) {
		if (isOpenSerialPort[i]) {
			mySerialPortThread[i]->closeSerialPortThread();
		}
	}

	for (int i = 0; i < this->availableCameraNum; i++) {
		myCameraThread[i]->closeCameraThread();
	}
	//�ͷ�����֮ǰ���������Դ��
	for (int i = 0; i < this->availableSerialPortNum; i++) {
		disconnect(mySerialPortThread[i], &SerialPortThread::emit_error, this, &HostComputerMainWindow::showError);
		delete mySerialPortThread[i];
		delete serialPortReceiveArea_PlainTextEdit[i];
	}
	mySerialPortThread.clear();
	serialPortReceiveArea_PlainTextEdit.clear();
	for (int i = 0; i < this->availableCameraNum; i++) {
		delete myCameraThread[i];
		delete cameraReceiveArea_PlainTextEdit[i];
	}
	displayReceivedData_TabWidget->clear();
	myCameraThread.clear();
	cameraReceiveArea_PlainTextEdit.clear();

	serialPortDescription_ComboBox->clear();
	ordinaryCurrentSerialPort_ComboBox->clear();
	protocolCurrentSerialPort_ComboBox->clear();

	if (openSerialPort_PushButton->text() == "close serial port") {
		openSerialPort_PushButton->setText("open serial port");
	}
	if (ordinarySerialPortSend_PushButton->text() == "pause") {
		ordinarySerialPortSend_PushButton->setText("send");
	}
	if (protocolSerialPortSend_PushButton->text() == "pause") {
		protocolSerialPortSend_PushButton->setText("send");
	}
	delete confWidget;
	//��������
	this->availableSerialPortName = availableSerialPortName;
	this->availableCameraName = availableCameraName;
	this->customSerialPortName = customSerialPortName;
	this->customCameraName = customCameraName;
	this->transferRole = transferRole;

	//���·�����Դ
	serialPortDescription_ComboBox->addItems(customSerialPortName);
	ordinaryCurrentSerialPort_ComboBox->addItems(customSerialPortName);
	protocolCurrentSerialPort_ComboBox->addItems(customSerialPortName);

	availableSerialPortNum = availableSerialPortName.length();
	availableCameraNum = availableCameraName.length();

	//���䴮���߳���Դ
	mySerialPortThread << new SerialPortThread;
	connect(mySerialPortThread[0], &SerialPortThread::emit_error, this, &HostComputerMainWindow::showError);
	if (availableSerialPortNum == 0) {
		availableSerialPortNum = 1;
		serialPortReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(serialPortReceiveArea_PlainTextEdit[0], "receive area");
	}
	else {
		serialPortReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(serialPortReceiveArea_PlainTextEdit[0], customSerialPortName[0]);
	}
	for (int i = 1; i < this->availableSerialPortNum; i++) {
		mySerialPortThread << new SerialPortThread;
		connect(mySerialPortThread[i], &SerialPortThread::emit_error, this, &HostComputerMainWindow::showError);
		serialPortReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(serialPortReceiveArea_PlainTextEdit[i], customSerialPortName[i]);
	}
	ordinaryTimerId.clear();
	protocolTimerId.clear();
	baudRate.clear();
	dataBits.clear();
	stopBits.clear();
	parityBits.clear();
	flowControl.clear();
	isHexShow.clear();
	isTimestamp.clear();
	isWrap.clear();
	isOpenSerialPort.clear();
	isOrdinaryDelayedTx.clear();
	isOrdinarySendByHex.clear();
	isOrdinarySend.clear();
	isProtocolDelayedTx.clear();
	isProtocolSendByHex.clear();
	isProtocolSend.clear();
	ordinarySendPeriod.clear();
	protocolSendPeriod.clear();
	ordinarySendPlainTextEdit.clear();
	protocolSenderID.clear();
	protocolDataID.clear();
	protocolData.clear();
	protocolCRC.clear();
	txNum.clear();
	rxNum.clear();
	for (int i = 0; i < this->availableSerialPortNum; i++) {
		ordinaryTimerId << 0;
		protocolTimerId << 0;
		baudRate << 0;
		dataBits << 0;
		stopBits << 0;
		parityBits << 0;
		flowControl << 0;
		isHexShow << false;
		isTimestamp << false;
		isWrap << false;
		isOpenSerialPort << false;
		isOrdinaryDelayedTx << false;
		isOrdinarySendByHex << false;
		isOrdinarySend << false;
		isProtocolDelayedTx << false;
		isProtocolSendByHex << false;
		isProtocolSend << false;
		ordinarySendPeriod << 1000;
		protocolSendPeriod << 1000;
		ordinarySendPlainTextEdit << "";
		protocolSenderID << 0;
		protocolDataID << 0;
		protocolData << "";
		protocolCRC << "";
		txNum << 0;
		rxNum << 0;
	}

	//��������ͷ�߳���Դ
	myCameraThread << new CameraThread;
	cameraReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
	writeTips(" >> available camera��");
	if (availableCameraNum == 0) {
		availableCameraNum = 1;
		writeTips(">> none");
	}
	else {
		displayReceivedData_TabWidget->addTab(cameraReceiveArea_PlainTextEdit[0], customCameraName[0]);
		writeTips(customCameraName);
	}
	for (int i = 1; i < this->availableCameraNum; i++) {
		myCameraThread << new CameraThread;
		cameraReceiveArea_PlainTextEdit << new QPlainTextEdit(this);
		displayReceivedData_TabWidget->addTab(cameraReceiveArea_PlainTextEdit[i], customCameraName[i]);
	}
	displayDataUpdateTimer.start(25);
}

void HostComputerMainWindow::showError(QString info)
{
	writeTips(info);
}

//************************************
// ������:   	HostComputerMainWindow::on_confPeripheral_action_triggered
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�������ý���,����֮ǰ���Ѿ��򿪵��豸�ر�
// ���ڣ�		2018/4/10 16:54:32
//************************************
void HostComputerMainWindow::on_confPeripheral_action_triggered()
{
	confWidget = new ConfigurationWidget;
	//Ϊ��һ������
	if (oneButtonStart_action->isChecked()) {
		confWidget->jumpVerify();
	}
	mySerialPortThread[0]->searchSerialPort();
	availableSerialPortName = mySerialPortThread[0]->getSerialPortName();
	availableCameraName = myCameraThread[0]->searchCamera();
	confWidget->getParameter(availableSerialPortName, availableCameraName);
	connect(confWidget, &ConfigurationWidget::configurationInfo, this, &HostComputerMainWindow::getConfInfo);
	confWidget->show();
	//�رն�ʱ��
	displayDataUpdateTimer.stop();
	//������ڴ��ˣ��͹ر���
	for (int i = 0; i < availableSerialPortNum; i++) {
		if (isOpenSerialPort[i]) {
			mySerialPortThread[i]->closeSerialPortThread();
		}
	}
	//�������ͷ���ˣ��͹ر���
	if (openCamera_action->isChecked()) {
		openCamera_action->setChecked(false);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_searchSerialPort_action_triggered
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		����ϵͳ�п��õĴ���,��ʾ����ʾ����
// ���ڣ�		2018/4/6 15:59:38
//************************************
void HostComputerMainWindow::on_searchSerialPort_action_triggered()
{
	mySerialPortThread[0]->searchSerialPort();
	writeTips(">> available serial port:");
	writeTips(mySerialPortThread[0]->getSerialPortDescription());
}

//************************************
// ������:   	HostComputerMainWindow::on_searchCamera_action_triggered
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��������ͷ�豸
// ���ڣ�		2018/4/10 13:20:37
//************************************
void HostComputerMainWindow::on_searchCamera_action_triggered()
{
	writeTips(">> available camera��");
	writeTips(myCameraThread[0]->searchCamera());
}

//************************************
// ������:   	HostComputerMainWindow::on_openCamera_action_triggered
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		������ͷ
// ���ڣ�		2018/4/10 13:24:18
//************************************
void HostComputerMainWindow::on_openCamera_action_triggered()
{
	qDebug() << "clicked";
	if (availableCameraName.length() > 0) {
		if (openCamera_action->isChecked()) {
			for (int i = 0; i < availableCameraNum; i++) {
				if (!myCameraThread[i]->openCameraThread(availableCameraName.at(i))) {
					writeTips(">> " + availableCameraName.at(i) + "��ʧ�ܣ�");
					openCamera_action->setChecked(false);
				}
				else {
					writeTips(">> " + availableCameraName.at(i) + "�򿪳ɹ���");
				}
			}
		}
		else {
			for (int i = 0; i < availableCameraNum; i++) {
				myCameraThread[i]->closeCameraThread();
				writeTips(">> " + availableCameraName.at(i) + "�رգ�");
			}
		}
	}
	else {
		writeTips(">> δ�����κ�����ͷ�豸��");
		openCamera_action->setChecked(false);
	}
}

void HostComputerMainWindow::on_startHostComputer_action_triggered()
{
	role_serialPortIndex.clear();
	role_cameraIndex.clear();
	senderIndex.clear();
	receiverIndex.clear();
	roleLength = transferRole.length();
	if (startHostComputer_action->isChecked()) {
		if (transferRole.isEmpty()) {
			writeTips(">> �������Ϊ�գ���λ���Զ����书���޷�������");
			startHostComputer_action->setChecked(false);
		}
		else {
			QList<int>deviceNum;//�Զ��������漰�����豸�ı��
			for (int i = 0; i < transferRole.length(); i++) {
				int tmp = transferRole.at(i) / 16;
				senderIndex.append(tmp);
				if (!deviceNum.contains(tmp)) {
					deviceNum.append(tmp);
				}
				tmp = transferRole.at(i) % 16;
				receiverIndex.append(tmp);
				if (!deviceNum.contains(tmp)) {
					deviceNum.append(tmp);
				}
			}
			bool isCan = true;
			//����������豸������ͷ�豸
			for (int i = 0; i < deviceNum.length(); i++) {
				if (deviceNum.at(i) < availableSerialPortNum) {
					role_serialPortIndex.append(deviceNum.at(i));
				}
				else {
					role_cameraIndex.append(deviceNum.at(i));
				}
			}
			for (int i = 0; i < role_serialPortIndex.length(); i++) {
				if (!isOpenSerialPort[role_serialPortIndex.at(i)]) {
					writeTips(tr(">> %1δ�򿪣�").arg(availableSerialPortName.at(i)));
					isCan = false;
				}
			}
			if (!role_cameraIndex.isEmpty()) {
				if (!openCamera_action->isChecked()) {
					writeTips(">> ����ͷ�豸δ�򿪣�");
					isCan = false;
				}
			}
			if (isCan) {
				transportUpdateTimer.start(10);
				writeTips(">> �Զ����书��������һ�й���������");
			}
			else {
				writeTips(">> ���豸û�д򿪣��Զ����书�ܲ���������");
				startHostComputer_action->setChecked(false);
			}

		}
	}
	else {
		transportUpdateTimer.stop();
	}
}
void HostComputerMainWindow::on_oneButtonStart_action_triggered()
{
	on_confPeripheral_action_triggered();
	confWidget->oneButtonStart();
	on_openSerialPort_PushButton_clicked();
	openCamera_action->setChecked(true);
	on_openCamera_action_triggered();
	startHostComputer_action->setChecked(true);
	on_startHostComputer_action_triggered();
	oneButtonStart_action->setChecked(false);

}
//************************************
// ������:   	HostComputerMainWindow::on_isHex_CheckBox_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		ѡ���Ƿ���hex��ʽ��ʾ
// ���ڣ�		2018/4/9 13:08:34
//************************************
void HostComputerMainWindow::on_isHex_CheckBox_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (isHex_CheckBox->isChecked()) {
			isHexShow[serialPortDescription_ComboBox->currentIndex()] = true;
		}
		else {
			isHexShow[serialPortDescription_ComboBox->currentIndex()] = false;
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_isTimestamp_CheckBox_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		ѡ���Ƿ���ʾʱ���
// ���ڣ�		2018/4/9 13:09:21
//************************************
void HostComputerMainWindow::on_isTimestamp_CheckBox_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (isTimestamp_CheckBox->isChecked()) {
			isTimestamp[serialPortDescription_ComboBox->currentIndex()] = true;
		}
		else {
			isTimestamp[serialPortDescription_ComboBox->currentIndex()] = false;
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_isWrap_CheckBox_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		ѡ���Ƿ��Զ�����
// ���ڣ�		2018/4/9 13:10:56
//************************************
void HostComputerMainWindow::on_isWrap_CheckBox_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (isWrap_CheckBox->isChecked()) {
			isWrap[serialPortDescription_ComboBox->currentIndex()] = true;
		}
		else {
			isWrap[serialPortDescription_ComboBox->currentIndex()] = false;
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_ordinaryDelayedTx_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ��ͨ��ʱ���͵�һЩ����
// ���ڣ�		2018/4/9 13:48:38
//************************************
void HostComputerMainWindow::on_ordinaryDelayedTx_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (ordinaryDelayedTx_CheckBox->isChecked()) {
			isOrdinaryDelayedTx[serialPortDescription_ComboBox->currentIndex()] = true;
			ordinarySendPeriod[serialPortDescription_ComboBox->currentIndex()] = ordinarySendTimes_SpinBox->value();
		}
		else {
			isOrdinaryDelayedTx[serialPortDescription_ComboBox->currentIndex()] = false;
			if (ordinarySerialPortSend_PushButton->text() == "pause") {
				killTimer(ordinaryTimerId[serialPortDescription_ComboBox->currentIndex()]);
				ordinarySerialPortSend_PushButton->setText("send");
			}
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_ordinarySendByHex_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		ȷ����ͨ�����Ƿ���Hex����ʽ����
// ���ڣ�		2018/4/9 13:42:14
//************************************
void HostComputerMainWindow::on_ordinarySendByHex_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (ordinarySendHex_CheckBox->isChecked()) {
			isOrdinarySendByHex[serialPortDescription_ComboBox->currentIndex()] = true;
			ordinarySendArea_PlainTextEdit->clear();
		}
		else {
			isOrdinarySendByHex[serialPortDescription_ComboBox->currentIndex()] = false;
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_protocolDelayedTx_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��ȡЭ�鶨ʱ���͵�һЩ����
// ���ڣ�		2018/4/9 13:47:54
//************************************
void HostComputerMainWindow::on_protocolDelayedTx_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (protocolDelayedTx_CheckBox->isChecked()) {
			isProtocolDelayedTx[serialPortDescription_ComboBox->currentIndex()] = true;
			protocolSendPeriod[serialPortDescription_ComboBox->currentIndex()] = protocolSendTimes_SpinBox->value();
			protocolTimerId[serialPortDescription_ComboBox->currentIndex()] = startTimer(protocolSendTimes_SpinBox->value());
		}
		else {
			isProtocolDelayedTx[serialPortDescription_ComboBox->currentIndex()] = false;
			killTimer(protocolTimerId[serialPortDescription_ComboBox->currentIndex()]);
		}
	}
}

void HostComputerMainWindow::on_protocolSendByHex_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {

	}
}

//************************************
// ������:   	HostComputerMainWindow::on_saveReceiveData_PushButton_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		������յ��ĵ�ǰ���ڵ�����
// ���ڣ�		2018/4/9 12:30:26
//************************************
void HostComputerMainWindow::on_saveReceiveData_PushButton_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		QString fileName = availableSerialPortName[serialPortDescription_ComboBox->currentIndex()];

		QFile file("R_" + fileName + ".txt");
		file.open(QIODevice::WriteOnly);
		file.close();
		if (!file.isOpen()) {
			if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream textStream(&file);
				textStream << serialPortReceiveArea_PlainTextEdit[serialPortDescription_ComboBox->currentIndex()]->toPlainText();
				file.close();
				writeTips("�����ļ��ɹ���");
			}
			else {
				showWarningInfo("�����ļ�ʧ�ܣ�");
			}
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_clearReceiveData_PushButton_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��յ�ǰ���ڽ����������Լ�ͳ���շ����ݵ�ͳ�ƽ��
// ���ڣ�		2018/4/9 09:30:45
//************************************
void HostComputerMainWindow::on_clearReceiveData_PushButton_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		serialPortReceiveArea_PlainTextEdit[serialPortDescription_ComboBox->currentIndex()]->clear();
		txNum[serialPortDescription_ComboBox->currentIndex()] = mySerialPortThread[
			serialPortDescription_ComboBox->currentIndex()]->writeBytes(true);
			rxNum[serialPortDescription_ComboBox->currentIndex()] = mySerialPortThread[
				serialPortDescription_ComboBox->currentIndex()]->readBytes(true);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_openSerialPort_PushButton_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�򿪵�ǰ�Ĵ���
// ���ڣ�		2018/4/8 21:17:18
//************************************
void HostComputerMainWindow::on_openSerialPort_PushButton_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (openSerialPort_PushButton->text() == "open serial port") {
			writeTips(tr(">> ���ڴ�%1").arg(availableSerialPortName.at(serialPortDescription_ComboBox->currentIndex())));
			if (mySerialPortThread[serialPortDescription_ComboBox->currentIndex()]->openSerialPortThread(
				availableSerialPortName.at(serialPortDescription_ComboBox->currentIndex()),
				baudRate_ComboBox->currentIndex(), dataBits_ComboBox->currentIndex(), stopBits_ComboBox->currentIndex(),
				parityBits_ComboBox->currentIndex(), flowControl_ComboBox->currentIndex())) {
				openSerialPort_PushButton->setText("close serial port");
				isOpenSerialPort[serialPortDescription_ComboBox->currentIndex()] = true;
				writeTips(tr(">> ����%1�򿪳ɹ���").arg(availableSerialPortName.at(serialPortDescription_ComboBox->currentIndex())));
			}
			else {
				isOpenSerialPort[serialPortDescription_ComboBox->currentIndex()] = false;
				showWarningInfo("��ǰ���ڴ�ʧ�ܣ�");
				writeTips(">> ��ǰ���ڴ�ʧ�ܣ�\n>> ������·�Ƿ�Ӵ����ã�");
			}
		}
		else {
			mySerialPortThread[serialPortDescription_ComboBox->currentIndex()]->closeSerialPortThread();
			isOpenSerialPort[serialPortDescription_ComboBox->currentIndex()] = false;
			openSerialPort_PushButton->setText("open serial port");
		}
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_ordinarySerialPortSend_PushButton_clicked
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��ͨ���䷢��
// ���ڣ�		2018/4/9 14:05:43
//************************************
void HostComputerMainWindow::on_ordinarySerialPortSend_PushButton_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {
		if (ordinaryDelayedTx_CheckBox->isChecked()) {
			if (ordinarySerialPortSend_PushButton->text() == "send") {
				ordinaryTimerId[serialPortDescription_ComboBox->currentIndex()] = startTimer(ordinarySendTimes_SpinBox->value());
				ordinarySerialPortSend_PushButton->setText("pause");
			}
			else {
				killTimer(ordinaryTimerId[serialPortDescription_ComboBox->currentIndex()]);
				ordinarySerialPortSend_PushButton->setText("send");
			}
		}
		else {
			if (isOrdinarySendByHex[serialPortDescription_ComboBox->currentIndex()]) {
				mySerialPortThread[serialPortDescription_ComboBox->currentIndex()]->writeData(
					myMath.stringToHex(ordinarySendArea_PlainTextEdit->toPlainText().toLocal8Bit()));
			}
			else {
				mySerialPortThread[serialPortDescription_ComboBox->currentIndex()]->writeData(
					ordinarySendArea_PlainTextEdit->toPlainText().toLocal8Bit());
			}
		}
	}
}

void HostComputerMainWindow::on_protocolSerialPortSend_PushButton_clicked()
{
	if (serialPortDescription_ComboBox->currentIndex() >= 0) {

	}
}

//************************************
// ������:   	HostComputerMainWindow::on_serialPortDescription_ComboBox_currentIndexChanged
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		int index
// ���ߣ�		���Ͽ�����
// ˵����		����ǰ���ڸı��ʱ��
// ���ڣ�		2018/4/9 14:19:44
//************************************
void HostComputerMainWindow::on_serialPortDescription_ComboBox_currentIndexChanged(int index)
{
	if ((index > -1) && (index < availableSerialPortNum)) {
		qDebug() << index;
		//��¼��һ�����ڵ���ز���
		baudRate[lastSerialPortIndex] = baudRate_ComboBox->currentIndex();
		dataBits[lastSerialPortIndex] = dataBits_ComboBox->currentIndex();
		stopBits[lastSerialPortIndex] = stopBits_ComboBox->currentIndex();
		parityBits[lastSerialPortIndex] = parityBits_ComboBox->currentIndex();
		flowControl[lastSerialPortIndex] = flowControl_ComboBox->currentIndex();
		isHexShow[lastSerialPortIndex] = isHex_CheckBox->isChecked();
		isTimestamp[lastSerialPortIndex] = isTimestamp_CheckBox->isChecked();
		isWrap[lastSerialPortIndex] = isWrap_CheckBox->isChecked();
		if (openSerialPort_PushButton->text() == "open serial port") {
			isOpenSerialPort[lastSerialPortIndex] = false;
		}
		else {
			isOpenSerialPort[lastSerialPortIndex] = true;
		}
		isOrdinaryDelayedTx[lastSerialPortIndex] = ordinaryDelayedTx_CheckBox->isChecked();
		isOrdinarySendByHex[lastSerialPortIndex] = ordinarySendHex_CheckBox->isChecked();
		if (ordinarySerialPortSend_PushButton->text() == "send") {
			isOrdinarySend[lastSerialPortIndex] = false;
		}
		else {
			isOrdinarySend[lastSerialPortIndex] = true;
		}
		isProtocolDelayedTx[lastSerialPortIndex] = protocolDelayedTx_CheckBox->isChecked();
		isProtocolSendByHex[lastSerialPortIndex] = protocolSendHex_CheckBox->isChecked();
		if (protocolSerialPortSend_PushButton->text() == "send") {
			isProtocolSend[lastSerialPortIndex] = false;
		}
		else {
			isProtocolSend[lastSerialPortIndex] = true;
		}
		ordinarySendPeriod[lastSerialPortIndex] = ordinarySendTimes_SpinBox->value();
		protocolSendPeriod[lastSerialPortIndex] = protocolSendTimes_SpinBox->value();
		ordinarySendPlainTextEdit[lastSerialPortIndex] = ordinarySendArea_PlainTextEdit->toPlainText();
		protocolSenderID[lastSerialPortIndex] = protocolSenderID_ComboBox->currentIndex();
		protocolDataID[lastSerialPortIndex] = protocolDataID_ComboBox->currentIndex();
		protocolData[lastSerialPortIndex] = protocolData_LineEdit->text();
		protocolCRC[lastSerialPortIndex] = protocolCRC_LineEdit->text();

		ordinaryCurrentSerialPort_ComboBox->setCurrentIndex(index);
		protocolCurrentSerialPort_ComboBox->setCurrentIndex(index);
		displayReceivedData_TabWidget->setCurrentIndex(index);

		lastSerialPortIndex = index;

		baudRate_ComboBox->setCurrentIndex(baudRate[lastSerialPortIndex]);
		dataBits_ComboBox->setCurrentIndex(dataBits[lastSerialPortIndex]);
		stopBits_ComboBox->setCurrentIndex(stopBits[lastSerialPortIndex]);
		parityBits_ComboBox->setCurrentIndex(parityBits[lastSerialPortIndex]);
		flowControl_ComboBox->setCurrentIndex(flowControl[lastSerialPortIndex]);
		isHex_CheckBox->setChecked(isHexShow[lastSerialPortIndex]);
		isTimestamp_CheckBox->setChecked(isTimestamp[lastSerialPortIndex]);
		isWrap_CheckBox->setChecked(isWrap[lastSerialPortIndex]);
		if (isOpenSerialPort[lastSerialPortIndex]) {
			openSerialPort_PushButton->setText("close serial port");
		}
		else {
			openSerialPort_PushButton->setText("open serial port");
		}
		ordinaryDelayedTx_CheckBox->setChecked(isOrdinaryDelayedTx[lastSerialPortIndex]);
		ordinarySendHex_CheckBox->setChecked(isOrdinarySendByHex[lastSerialPortIndex]);
		if (isOrdinarySend[lastSerialPortIndex]) {
			ordinarySerialPortSend_PushButton->setText("pause");
		}
		else {
			ordinarySerialPortSend_PushButton->setText("send");
		}
		protocolDelayedTx_CheckBox->setChecked(isProtocolDelayedTx[lastSerialPortIndex]);
		protocolSendHex_CheckBox->setChecked(isProtocolSendByHex[lastSerialPortIndex]);
		if (isProtocolSend[lastSerialPortIndex]) {
			protocolSerialPortSend_PushButton->setText("pause");
		}
		else {
			protocolSerialPortSend_PushButton->setText("send");
		}
		ordinarySendTimes_SpinBox->setValue(ordinarySendPeriod[lastSerialPortIndex]);
		protocolSendTimes_SpinBox->setValue(protocolSendPeriod[lastSerialPortIndex]);
		ordinarySendArea_PlainTextEdit->clear();
		ordinarySendArea_PlainTextEdit->insertPlainText(ordinarySendPlainTextEdit[lastSerialPortIndex]);
		protocolSenderID_ComboBox->setCurrentIndex(protocolSenderID[lastSerialPortIndex]);
		protocolDataID_ComboBox->setCurrentIndex(protocolDataID[lastSerialPortIndex]);
		protocolData_LineEdit->setText(protocolData[lastSerialPortIndex]);
		protocolCRC_LineEdit->setText(protocolCRC[lastSerialPortIndex]);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_ordinaryCurrentSerialPort_ComboBox_currentIndexChanged
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		int index
// ���ߣ�		���Ͽ�����
// ˵����		����ǰ���ڸı��ʱ��
// ���ڣ�		2018/4/9 14:24:10
//************************************
void HostComputerMainWindow::on_ordinaryCurrentSerialPort_ComboBox_currentIndexChanged(int index)
{
	if ((index > -1) && (index < availableSerialPortNum)) {
		serialPortDescription_ComboBox->setCurrentIndex(index);
		protocolCurrentSerialPort_ComboBox->setCurrentIndex(index);
		displayReceivedData_TabWidget->setCurrentIndex(index);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_protocolCurrentSerialPort_ComboBox_currentIndexChanged
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		int index
// ���ߣ�		���Ͽ�����
// ˵����		����ǰ���ڸı��ʱ��
// ���ڣ�		2018/4/9 14:25:59
//************************************
void HostComputerMainWindow::on_protocolCurrentSerialPort_ComboBox_currentIndexChanged(int index)
{
	if ((index > -1) && (index < availableSerialPortNum)) {
		serialPortDescription_ComboBox->setCurrentIndex(index);
		ordinaryCurrentSerialPort_ComboBox->setCurrentIndex(index);
		displayReceivedData_TabWidget->setCurrentIndex(index);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_displayReceiveArea_TabWidget_currentChanged
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		int index
// ���ߣ�		���Ͽ�����
// ˵����		����ǰ���ڸı��ʱ��
// ���ڣ�		2018/4/9 14:26:09
//************************************
void HostComputerMainWindow::on_displayReceiveArea_TabWidget_currentChanged(int index)
{
	if ((index > -1) && (index < availableSerialPortNum)) {
		serialPortDescription_ComboBox->setCurrentIndex(index);
		ordinaryCurrentSerialPort_ComboBox->setCurrentIndex(index);
		protocolCurrentSerialPort_ComboBox->setCurrentIndex(index);
	}
}

//************************************
// ������:   	HostComputerMainWindow::on_ordinarySendArea_PlainEditText_textChanged
// ���ʿ���:   	private 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		
// ���ڣ�		2018/4/9 14:26:31
//************************************
void HostComputerMainWindow::on_ordinarySendArea_PlainEditText_textChanged()
{
	if (ordinarySendHex_CheckBox->isChecked()) {
		if (!ordinarySendArea_PlainTextEdit->toPlainText().isEmpty()) {
			//ע�͵������ַ���Ҳ�ǿ���ʹ�õ�
			//QString tmp = tr("[0-9a-fA-F]{%1}").arg(ordinarySendArea_PlainTextEdit->toPlainText().length());
			//QRegExp tmp_RegExp(tmp);
			//if (!ordinarySendArea_PlainTextEdit->toPlainText().contains(tmp_RegExp)) {
			//	ordinarySendArea_PlainTextEdit->clear();
			//ordinarySendArea_PlainTextEdit->setPlaceholderText("please input 0-9 ��a-f or A-F");
			//}
			if (!ordinarySendArea_PlainTextEdit->toPlainText().contains(*hex_RegExp)) {
				ordinarySendArea_PlainTextEdit->clear();
				ordinarySendArea_PlainTextEdit->setPlaceholderText("please input 0-9 ��a-f or A-F");
			}
		}
	}
}
