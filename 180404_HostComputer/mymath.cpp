#include "mymath.hpp"
#pragma execution_character_set("utf-8")

//************************************
// 限定: 	    : QObject(parent)
// 函数名:    	MyMath
// 形参: 	    QObject * parent
// 返回值:   	
// 作者:    	郑志兵
// 说明:		构造函数
// 日期:      	2018-2-22 11:54:57
//************************************
MyMath::MyMath(QObject * parent) : QObject(parent) {

}

//************************************
// 限定: 	   
// 函数名:    	~MyMath
// 返回值:   	
// 作者:    	郑志兵
// 说明:		析构函数
// 日期:      	2018-2-22 11:55:15
//************************************
MyMath::~MyMath() {

}

//************************************
// 限定: 	   
// 函数名:    	floatToFourBytes
// 形参: 	    float data
// 返回值:   	QT_NAMESPACE::QByteArray
// 作者:    	郑志兵
// 说明:		这是将单精度浮点型数据转换成长度为4字节的字符串
// 日期:      	2018-2-22 11:55:26
//************************************
QByteArray MyMath::floatToFourBytes(float data)
{
	QByteArray result;
	union FloatToChar
	{
		float temp;
		char chr[4];
	};
	FloatToChar FTC;
	FTC.temp = data;
	for (int i = 0; i < 4; i++) {
		result.append(FTC.chr[i]);
	}
	return result;
}

//************************************
// 限定: 	   
// 函数名:    	fourBytesToFloat
// 形参: 	    char * chr
// 返回值:   	float
// 作者:    	郑志兵
// 说明:		这是将4字节大小的字符串转换成单精度浮点型数据
// 日期:      	2018-2-22 11:57:47
//************************************
float MyMath::fourBytesToFloat(char * chr)
{
	union BytesToFloat {
		char temp[4];
		float result;
	};
	BytesToFloat BTF;
	for (int i = 0; i < 4; i++) {
		BTF.temp[i] = chr[i];
	}
	return BTF.result;
}

//************************************
// 限定: 	   
// 函数名:    	fourBytesToFloat
// 形参: 	    QByteArray data
// 返回值:   	float
// 作者:    	郑志兵
// 说明:		这是将4字节大小的字符串转换成单精度浮点型数据
// 日期:      	2018-2-22 11:58:42
//************************************
float MyMath::fourBytesToFloat(QByteArray data)
{
	union BytesToFloat {
		char temp[4];
		float result;
	};
	BytesToFloat BTF;
	for (int i = 0; i < 4; i++) {
		BTF.temp[i] = data.at(i);
	}
	return BTF.result;
}

//************************************
// 函数名:   	MyMath::stringToHex
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QByteArray
// 形参: 		QByteArray str
// 作者：		码上看世界
// 说明：		这个是将字符串形式的十六进制数转换成数值型的 16进制数存储在字符串中
//				"414243" ==>"ABC"
// 日期：		2018/4/12 20:36:48
//************************************
QByteArray MyMath::stringToHex(QByteArray str)
{
	QByteArray chr;
	int temp;
	for (int i = 0; i < str.length(); i += 2) {
		int Fir = (int)(str.at(i) - '0') * 16;
		int Sec = (int)(str.at(i + 1) - '0');
		chr.append(Fir + Sec);
	}
	return chr;
}

//************************************
// 限定: 	   
// 函数名:    	floatToHex
// 形参: 	    float data
// 返回值:   	QT_NAMESPACE::QByteArray
// 作者:    	郑志兵
// 说明:		这是将单精度浮点型数据以16进制数用字符的形式表现出来
//				12.456 => "41474BC6"
// 日期:      	2018-2-22 12:25:03
//************************************
QByteArray MyMath::floatToHex(float data)
{
	QByteArray hexchr;
	QByteArray chr;
	unsigned char chrr[4];
	chr = floatToFourBytes(data);
	for (int i = 0; i < 4; i++) {
		chrr[i] = (unsigned char)chr[3 - i];//Low in the right,high in the left
		int f = (int)chrr[i] / 16;
		int s = (int)chrr[i] % 16;
		if (f > 0) {
			if (f < 10) {
				hexchr.append((char)(f + 48));
			}
			else {
				switch (f)
				{
				case 10:
					hexchr.append('A');
					break;
				case 11:
					hexchr.append('B');
					break;
				case 12:
					hexchr.append('C');
					break;
				case 13:
					hexchr.append('D');
					break;
				case 14:
					hexchr.append('E');
					break;
				case 15:
					hexchr.append('F');
					break;
				default:
					break;
				}
			}
			if (s < 10) {
				hexchr.append((char)(s + 48));
			}
			else {
				switch (s)
				{
				case 10:
					hexchr.append('A');
					break;
				case 11:
					hexchr.append('B');
					break;
				case 12:
					hexchr.append('C');
				case 13:
					hexchr.append('D');
					break;
				case 14:
					hexchr.append('E');
					break;
				case 15:
					hexchr.append('F');
					break;
				default:
					break;
				}
			}
		}
		else {
			hexchr.append('0');
			if (s < 10) {
				hexchr.append((char)(s + 48));
			}
			else {
				switch (s)
				{
				case 10:
					hexchr.append('A');
					break;
				case 11:
					hexchr.append('B');
					break;
				case 12:
					hexchr.append('C');
				case 13:
					hexchr.append('D');
					break;
				case 14:
					hexchr.append('E');
					break;
				case 15:
					hexchr.append('F');
					break;
				default:
					break;
				}
			}
		}
	}
	return hexchr;
}
