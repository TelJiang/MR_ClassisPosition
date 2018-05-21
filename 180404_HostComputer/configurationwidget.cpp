#include "configurationwidget.hpp"
#pragma execution_character_set("utf-8")
ConfigurationWidget::ConfigurationWidget(QWidget * parent) : QWidget(parent) {

	designUI();
	init();
	signalAndSlots();
}

ConfigurationWidget::~ConfigurationWidget() {

}

//************************************
// 函数名:   	ConfigurationWidget::designUI
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		对配置界面的设计
// 日期：		2018/4/10 17:12:13
//************************************
void ConfigurationWidget::designUI()
{
	ConfigurationWidget::setFont(QFont("微软雅黑", 11));
	ConfigurationWidget::resize(900, 550);
	//ConfigurationWidget::setWindowModality(Qt::ApplicationModal);

	customDeviceName_Label = new QLabel(this);
	customDeviceName_Label->setText(tr("custom device name"));
	customDeviceName_Label->setAlignment(Qt::AlignHCenter);

	systemDeviceName_Label = new QLabel(this);
	systemDeviceName_Label->setText(tr("system device name"));
	systemDeviceName_Label->setAlignment(Qt::AlignHCenter);

	senderDevice_Label = new QLabel(this);
	senderDevice_Label->setText(tr("sender device"));
	senderDevice_Label->setAlignment(Qt::AlignHCenter);

	receiveDevice_Label = new QLabel(this);
	receiveDevice_Label->setText(tr("receiver device"));
	receiveDevice_Label->setAlignment(Qt::AlignHCenter);

	customDeviceName_ListWidget = new QListWidget(this);
	systemDeviceName_ListWidget = new QListWidget(this);

	senderDevice_ComboBox = new QComboBox(this);
	receiverDevice_ComboBox = new QComboBox(this);

	verify_PushButton = new QPushButton(this);
	verify_PushButton->setText(tr("vertify"));

	tip_TextBrowser = new QTextBrowser(this);

	customDeviceName_GroupBox = new QGroupBox(this);

	customTransportDirection_GroupBox = new QGroupBox(this);

	customDeviceName_GroupBox_GridLayout = new QGridLayout(customDeviceName_GroupBox);

	customTransportDirection_GroupBox_GridLayout = new QGridLayout(customTransportDirection_GroupBox);

	entirety_GridLayout = new QGridLayout(this);

	customDeviceName_GroupBox_GridLayout->addWidget(customDeviceName_Label, 0, 0);
	customDeviceName_GroupBox_GridLayout->addWidget(systemDeviceName_Label, 0, 1);
	customDeviceName_GroupBox_GridLayout->addWidget(customDeviceName_ListWidget, 1, 0);
	customDeviceName_GroupBox_GridLayout->addWidget(systemDeviceName_ListWidget, 1, 1);

	customTransportDirection_GroupBox_GridLayout->addWidget(senderDevice_Label, 0, 0);
	customTransportDirection_GroupBox_GridLayout->addWidget(receiveDevice_Label, 0, 1);
	customTransportDirection_GroupBox_GridLayout->addWidget(senderDevice_ComboBox, 1, 0);
	customTransportDirection_GroupBox_GridLayout->addWidget(receiverDevice_ComboBox, 1, 1);
	customTransportDirection_GroupBox_GridLayout->addWidget(verify_PushButton, 2, 1);

	entirety_GridLayout->addWidget(customDeviceName_GroupBox, 0, 0, 1, 1);
	entirety_GridLayout->addWidget(customTransportDirection_GroupBox, 1, 0, 1, 1);
	entirety_GridLayout->addWidget(tip_TextBrowser, 0, 1, 2, 1);
}

//************************************
// 函数名:   	ConfigurationWidget::LoadQSSFile
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		const QString style
// 作者：		码上看世界
// 说明：		加载QSS文件
// 日期：		2018/4/10 18:36:00
//************************************
void ConfigurationWidget::LoadQSSFile(const QString style)
{
	QFile qss(style);
	qss.open(QFile::ReadOnly);
	ConfigurationWidget::setStyleSheet(qss.readAll());
	qss.close();
}

//************************************
// 函数名:   	ConfigurationWidget::signalAndSlots
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		信号和槽的连接
// 日期：		2018/4/10 20:18:51
//************************************
void ConfigurationWidget::signalAndSlots()
{
	connect(customDeviceName_ListWidget, &QListWidget::itemChanged, this,
		&ConfigurationWidget::on_customDeviceName_ListWidget_itemChanged);
	connect(verify_PushButton, &QPushButton::clicked, this,
		&ConfigurationWidget::on_verify_PushButton_clicked);
}

void ConfigurationWidget::init()
{
	isJump = false;
	transferRole = { 0 };
}

//************************************
// 函数名:   	ConfigurationWidget::getParameter
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QStringList serialPortName
// 形参: 		QStringList cameraName
// 作者：		码上看世界
// 说明：		获取设备名称,并显示在配置界面上,完成加载配置文件的任务
// 日期：		2018/4/10 18:26:07
//************************************
void ConfigurationWidget::getParameter(QStringList serialPortName, QStringList cameraName)
{
	availableSerialPortName = serialPortName;
	availableCameraName = cameraName;

	appendItemsToListWidget(systemDeviceName_ListWidget, availableSerialPortName + availableCameraName, false);

	for (int i = 0; i < serialPortName.length(); i++) {
		customSerialPortName.append(tr("串口%1").arg(i));
	}
	for (int i = 0; i < cameraName.length(); i++) {
		customCameraName.append(tr("摄像头%1").arg(i));
	}

	appendItemsToListWidget(customDeviceName_ListWidget, customSerialPortName + customCameraName, true);

	senderDevice_ComboBox->clear();
	receiverDevice_ComboBox->clear();
	senderDevice_ComboBox->addItems(customSerialPortName + customCameraName);
	receiverDevice_ComboBox->addItems(customSerialPortName + customCameraName);

	QStringList t_availableSerialPortName;
	QStringList t_availableCameraName;
	QStringList t_customSerialPortName;
	QStringList t_customCameraName;
	QStringList t_transferRule;

	int ret;
	//为了一键启动
	if (isJump) {
		ret = QMessageBox::Ok;
		isJump = false;
	}
	else {
		ret = QMessageBox::information(this, tr("提示"), tr("是否加载配置文件？"), QMessageBox::Ok, QMessageBox::Cancel);
	}
	if (ret == QMessageBox::Ok) {
		QFile file("conf.txt");
		if (file.open(QIODevice::ReadOnly)) {
			writetips(">> 配置文件已打开！");
			writetips(">> 正在加载配置文件...");
			QString lineInfo;

			lineInfo = QString::fromLocal8Bit(file.readLine());
			lineInfo = QString::fromLocal8Bit(file.readLine());//去掉配置文件的第一行
			lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
			if (lineInfo.contains("串口设备：")) {
				lineInfo = QString::fromLocal8Bit(file.readLine());
				lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				while (!lineInfo.contains("*")) {
					t_availableSerialPortName << lineInfo.mid(0, lineInfo.indexOf("->"));
					t_customSerialPortName << lineInfo.mid(lineInfo.indexOf("[") + 1,
						lineInfo.indexOf("]") - lineInfo.indexOf("[") - 1);
					lineInfo = QString::fromLocal8Bit(file.readLine());
					lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				}
			}
			lineInfo = QString::fromLocal8Bit(file.readLine());
			lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
			if (lineInfo.contains("摄像头设备：")) {
				lineInfo = QString::fromLocal8Bit(file.readLine());
				lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				while (!lineInfo.contains("*")) {
					t_availableCameraName << lineInfo.mid(0, lineInfo.indexOf("->"));
					t_customCameraName << lineInfo.mid(lineInfo.indexOf("->") + 3,
						lineInfo.indexOf("]", lineInfo.indexOf("-")) - lineInfo.indexOf(">") - 2);
					lineInfo = QString::fromLocal8Bit(file.readLine());
					lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				}
			}
			lineInfo = QString::fromLocal8Bit(file.readLine());
			lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
			if (lineInfo.contains("传输规则：")) {
				lineInfo = QString::fromLocal8Bit(file.readLine());
				lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				while (!lineInfo.contains("*")) {
					t_transferRule << lineInfo;
					lineInfo = QString::fromLocal8Bit(file.readLine());
					lineInfo = lineInfo.mid(0, lineInfo.length() - 2);
				}
			}

			if (t_availableSerialPortName == availableSerialPortName) {
				if (t_availableCameraName == availableCameraName) {
					writetips(">> 当前系统外部设备较上次使用并未发生改变！");
					//加载文件
					customDeviceName_ListWidget->clear();
					appendItemsToListWidget(customDeviceName_ListWidget,
						t_customSerialPortName + t_customCameraName, true);
					for (int i = 0; i < t_transferRule.length(); i++) {
						transferRole.append(t_transferRule.at(i).toInt());
					}
					writetips(">> 配置文件加载完成！");
					writetips(">> 当前传输规则如下：");
					if (transferRole.isEmpty()) {
						writetips(">> 暂未制定传输规则！");
					}
					for (int i = 0; i < transferRole.length(); i++) {
						writetips(">> " + senderDevice_ComboBox->itemText(transferRole.at(i) / 16) + " -> " +
							receiverDevice_ComboBox->itemText(transferRole.at(i) % 16));
					}
				}
				else {
					writetips(">> 系统的摄像头设备较上一次使用发生了改变！");
					writetips(">> 配置文件加载失败！");
				}
			}
			else {
				writetips(">> 系统的串口设备较上一次使用发生了改变！");
				writetips(">> 配置文件加载失败！");
			}
		}
		else {
			writetips(">> 无法打开配置文件！");
		}
	}
	else {
		writetips(">> 未加载配置文件！");
	}
}

//************************************
// 函数名:   	ConfigurationWidget::appendItemsToListWidget
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QListWidget * listwidget
// 形参: 		QStringList list
// 形参: 		bool iseditable
// 作者：		码上看世界
// 说明：		向某个列表部件添加条目，并设置条目是否可编辑
// 日期：		2018/4/10 18:37:49
//************************************
void ConfigurationWidget::appendItemsToListWidget(QListWidget * listwidget, QStringList list, bool iseditable)
{
	listwidget->addItems(list);
	if (iseditable) {
		for (int i = 0; i < list.length(); i++) {
			listwidget->item(i)->setFlags(Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsSelectable |
				Qt::ItemFlag::ItemIsDragEnabled | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsEnabled);
		}
	}
}

//************************************
// 函数名:   	ConfigurationWidget::writetips
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QString tip
// 作者：		码上看世界
// 说明：		写一条提示信息
// 日期：		2018/4/10 18:39:47
//************************************
void ConfigurationWidget::writetips(QString tip)
{
	tip_TextBrowser->append(tip);
}

//************************************
// 函数名:   	ConfigurationWidget::writetips
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QStringList tips
// 作者：		码上看世界
// 说明：		写多条提示信息
// 日期：		2018/4/10 18:40:09
//************************************
void ConfigurationWidget::writetips(QStringList tips)
{
	for (int i = 0; i < tips.length(); i++) {
		tip_TextBrowser->append(tips.at(i));
	}
}

//************************************
// 函数名:   	ConfigurationWidget::closeEvent
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QCloseEvent * e
// 作者：		码上看世界
// 说明：		重写closeEvent
// 日期：		2018/4/10 18:40:22
//************************************
void ConfigurationWidget::closeEvent(QCloseEvent * e)
{
	//汇总有用的信息
	int availableSerialPortNum = availableSerialPortName.length();
	int availableCameraNum = availableCameraName.length();
	availableCameraName.clear();
	availableSerialPortName.clear();
	customCameraName.clear();
	customSerialPortName.clear();
	QStringList serialPortDevice, cameraDevice;//目的是用于记录设备并写入配置文件
	for (int i = 0; i < availableSerialPortNum; i++) {
		serialPortDevice.append(systemDeviceName_ListWidget->item(i)->text());
		if (!customDeviceName_ListWidget->item(i)->text().trimmed().isEmpty()) {
			customSerialPortName.append(customDeviceName_ListWidget->item(i)->text());
			availableSerialPortName.append(systemDeviceName_ListWidget->item(i)->text());
		}
	}
	for (int i = 0; i < availableCameraNum; i++) {
		cameraDevice.append(systemDeviceName_ListWidget->item(i + serialPortDevice.length())->text());
		if (!customDeviceName_ListWidget->item(i + serialPortDevice.length())->text().trimmed().isEmpty()) {
			customCameraName.append(customDeviceName_ListWidget->item(i + serialPortDevice.length())->text());
			availableCameraName.append(systemDeviceName_ListWidget->item(i + serialPortDevice.length())->text());
		}
	}

	QStringList confInfo;
	//配置文件的内容组织
	confInfo.append("########################################");
	confInfo.append("串口设备：");
	for (int i = 0; i < serialPortDevice.length(); i++) {
		if (availableSerialPortName.contains(serialPortDevice.at(i))) {
			int t = availableSerialPortName.indexOf(serialPortDevice.at(i));
			confInfo.append(serialPortDevice.at(i) + "->[" + customSerialPortName.at(t) + "]");
		}
		else {
			confInfo.append(serialPortDevice.at(i) + "->[" + "]");
		}
	}
	confInfo.append("****************************************");
	confInfo.append("摄像头设备：");
	for (int i = 0; i < cameraDevice.length(); i++) {
		if (availableCameraName.contains(cameraDevice.at(i))) {
			int t = availableCameraName.indexOf(cameraDevice.at(i));
			confInfo.append(cameraDevice.at(i) + "->[" + customCameraName.at(t) + "]");
		}
		else {
			confInfo.append(cameraDevice.at(i) + "->[" + "]");
		}
	}
	confInfo.append("****************************************");
	confInfo.append("传输规则：");
	for (int i = 0; i < transferRole.length(); i++) {
		confInfo.append(tr("%1").arg(transferRole.at(i)));
	}
	confInfo.append("****************************************");

	//打开或者新建一个文件，并将配置信息保存到文件中
	QFile file("conf.txt");
	file.open(QIODevice::WriteOnly);
	file.close();
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream textstream(&file);
		for (int i = 0; i < confInfo.length(); i++) {
			textstream << confInfo.at(i) << endl;
		}
	}
	file.close();
	emit configurationInfo(availableSerialPortName, availableCameraName, customSerialPortName, customCameraName,
		transferRole);
}

void ConfigurationWidget::oneButtonStart()
{
	ConfigurationWidget::close();
}

void ConfigurationWidget::jumpVerify()
{
	isJump = true;
}

//************************************
// 函数名:   	ConfigurationWidget::on_verify_PushButton_clicked
// 访问控制:   	private 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		确认传输规则
// 日期：		2018/4/10 20:29:38
//************************************
void ConfigurationWidget::on_verify_PushButton_clicked()
{
	if (!receiverDevice_ComboBox->currentText().isEmpty()) {
		int ret = QMessageBox::information(this, "提示", tr("你确定制定这条传输规则？\n") +
			senderDevice_ComboBox->currentText() + " -> " + receiverDevice_ComboBox->currentText(), QMessageBox::Ok,
			QMessageBox::Discard);
		int theRule;
		theRule = senderDevice_ComboBox->currentIndex() * 16 + receiverDevice_ComboBox->currentIndex();
		if (ret == QMessageBox::Ok) {
			if (!transferRole.contains(theRule)) {
				transferRole.append(theRule);
			}
		}
		writetips(">> 当前传输规则如下：");
		for (int i = 0; i < transferRole.length(); i++) {
			writetips(">> " + senderDevice_ComboBox->itemText(transferRole.at(i) / 16) + " -> " +
				receiverDevice_ComboBox->itemText(transferRole.at(i) % 16));
		}
	}
	else {
		writetips(">> 没有设备！无法制定传输规则！");
	}
}

//************************************
// 函数名:   	ConfigurationWidget::on_customDeviceName_ListWidget_itemChanged
// 访问控制:   	private 
// 返回值:   	void
// 形参: 		QListWidgetItem * item
// 作者：		码上看世界
// 说明：		当列表的条目发生改变时，更新数据
// 日期：		2018/4/10 19:31:38
//************************************
void ConfigurationWidget::on_customDeviceName_ListWidget_itemChanged(QListWidgetItem * item)
{
	senderDevice_ComboBox->clear();
	receiverDevice_ComboBox->clear();
	customCameraName.clear();
	customSerialPortName.clear();
	for (int i = 0; i < availableSerialPortName.length(); i++) {
		if (!customDeviceName_ListWidget->item(i)->text().trimmed().isEmpty())
			customSerialPortName.append(customDeviceName_ListWidget->item(i)->text());
	}
	for (int i = 0; i < availableCameraName.length(); i++) {
		if (!customDeviceName_ListWidget->item(i + availableSerialPortName.length())->text().trimmed().isEmpty()) {
			customCameraName.append(customDeviceName_ListWidget->item(
				i + availableSerialPortName.length())->text());
		}
	}
	senderDevice_ComboBox->addItems(customSerialPortName + customCameraName);
	receiverDevice_ComboBox->addItems(customSerialPortName + customCameraName);

	transferRole.clear();
}
