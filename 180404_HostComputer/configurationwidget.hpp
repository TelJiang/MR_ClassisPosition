#pragma once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QCloseEvent>
#include <QDebug>
class ConfigurationWidget : public QWidget {
	Q_OBJECT

public:
	ConfigurationWidget(QWidget * parent = Q_NULLPTR);
	~ConfigurationWidget();
	void designUI();
	void LoadQSSFile(const QString style);
	void signalAndSlots();
	void init();
	void getParameter(QStringList serialPortName, QStringList cameraName);
	void appendItemsToListWidget(QListWidget *listwidget, QStringList list, bool iseditable);
	void writetips(QString tip);
	void writetips(QStringList tips);
	void closeEvent(QCloseEvent *e);
	void oneButtonStart();
	void jumpVerify();

signals:
	void configurationInfo(QStringList availableSerialPortName, QStringList availableCameraName,
		QStringList customSerialPortName, QStringList customCameraName, QList<int> transferRole);

private slots:
	void on_customDeviceName_ListWidget_itemChanged(QListWidgetItem *item);
	void on_verify_PushButton_clicked();
private:
	QLabel *customDeviceName_Label;
	QLabel *systemDeviceName_Label;
	QLabel *senderDevice_Label;
	QLabel *receiveDevice_Label;

	QComboBox *senderDevice_ComboBox;
	QComboBox *receiverDevice_ComboBox;

	QPushButton *verify_PushButton;

	QTextBrowser *tip_TextBrowser;

	QGroupBox *customDeviceName_GroupBox;
	QGroupBox *customTransportDirection_GroupBox;

	QListWidget *customDeviceName_ListWidget;
	QListWidget *systemDeviceName_ListWidget;

	QGridLayout *customDeviceName_GroupBox_GridLayout;
	QGridLayout *customTransportDirection_GroupBox_GridLayout;
	QGridLayout *entirety_GridLayout;

	QStringList availableSerialPortName, availableCameraName;
	QStringList customSerialPortName, customCameraName;
	QList<int> transferRole;
	bool isJump;
};
