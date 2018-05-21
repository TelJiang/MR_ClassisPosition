#pragma once
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QtMath>
class MyMath : public QObject {
	Q_OBJECT

public:
	MyMath(QObject * parent = Q_NULLPTR);
	~MyMath();

	QByteArray floatToFourBytes(float data);
	float fourBytesToFloat(char *chr);
	float fourBytesToFloat(QByteArray data);
	QByteArray stringToHex(QByteArray str);
	QByteArray floatToHex(float data);
private:
	
};
