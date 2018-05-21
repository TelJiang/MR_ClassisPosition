#include "myprotocol.hpp"
#pragma execution_character_set("utf-8")


//************************************
// 限定: 	    : QObject(parent)
// 函数名:    	MyProtocol
// 形参: 	    QObject * parent
// 返回值:   	
// 作者:    	郑志兵
// 说明:		构造函数
// 日期:      	2018-2-22 14:53:06
//************************************
MyProtocol::MyProtocol(QObject * parent) : QObject(parent) {
	
	//you can select it
	CRC8_GP = CRC8_31;
}

//************************************
// 限定: 	   
// 函数名:    	~MyProtocol
// 返回值:   	
// 作者:    	郑志兵
// 说明:		析构函数
// 日期:      	2018-2-22 14:53:20
//************************************
MyProtocol::~MyProtocol() {

}

//************************************
// 限定: 	   
// 函数名:    	CRC8_8
// 形参: 	    char chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		这是通过位操作对8位的数据生成CRC8校验码
// 日期:      	2018-2-22 14:54:03
//************************************
char MyProtocol::CRC8_8(char chr)
{
	unsigned char CRC = unsigned char (chr);
	for (int i = 0; i < 8; i++) {
		//judge whether the highest digit is 0
		if (CRC & 0x80) {       
			CRC = (CRC << 1) ^ CRC8_GP;
		}
		else
			CRC = CRC << 1;
	}
	return char(CRC);
}

//************************************
// 限定: 	   
// 函数名:    	CRC8_32
// 形参: 	    char * chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		这是通过位操作对32位数据生成CRC8校验码
// 日期:      	2018-2-22 14:54:51
//************************************
char MyProtocol::CRC8_32(char * chr)
{
	unsigned char CRC = 0x00;
	for (int j = 0; j < 4; j++) {
		CRC ^= unsigned char(*chr++);
		for (int i = 0; i < 8; i++) {
			if (CRC & 0x80) {
				CRC = (CRC << 1) ^ CRC8_GP;
			}
			else
				CRC = CRC << 1;
		}
	}
	return unsigned char(CRC);
}

//************************************
// 限定: 	   
// 函数名:    	CRC8Table_32
// 形参: 	    char * chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		这是通过查表的方式快速的生成32位数据的CRC8校验码
// 日期:      	2018-2-22 14:56:21
//************************************
char MyProtocol::CRC8Table_32(char * chr)
{
	unsigned char CRC = 0x00;
	for (int i = 0; i < 4; i++) {
		CRC = CRC8Table[CRC ^ (unsigned char(*chr++))];
	}
	return char(CRC);
}


//************************************
// 限定: 	   
// 函数名:    	CRC8Table_32
// 形参: 	    QByteArray chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		这是通过查表的方式快速的生成32位数据的CRC8校验码
// 日期:      	2018-2-22 14:57:06
//************************************
char MyProtocol::CRC8Table_32(QByteArray chr)
{
	unsigned char CRC = 0x00;
	for (int i = 0; i < 4; i++) {
		CRC = CRC8Table[CRC ^ (unsigned char(chr.at(i)))];
	}
	return char(CRC);
}

//************************************
// 限定: 	   
// 函数名:    	CRC8Table_40
// 形参: 	    char * chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		通过查表的方式生成40位数据的校验码
// 日期:      	2018-2-22 14:57:18
//************************************
char MyProtocol::CRC8Table_40(char * chr)
{
	unsigned char CRC = 0x00;
	for (int i = 0; i < 5; i++) {
		CRC = CRC8Table[CRC ^ (unsigned char(*chr++))];
	}
	return char(CRC);
}

//************************************
// 限定: 	   
// 函数名:    	CRC8Table_40
// 形参: 	    QByteArray chr
// 返回值:   	char
// 作者:    	郑志兵
// 说明:		通过查表的方式生成40位数据的校验码
// 日期:      	2018-2-22 14:57:53
//************************************
char MyProtocol::CRC8Table_40(QByteArray chr)
{
	unsigned char CRC = 0x00;
	for (int i = 0; i < 5; i++) {
		CRC = CRC8Table[CRC ^ (unsigned char(chr.at(i)))];
	}
	return char(CRC);
}

//************************************
// 限定: 	   
// 函数名:    	generateCRCResult
// 形参: 	    QByteArray CRC40
// 形参: 	    float f_data
// 返回值:   	QT_NAMESPACE::QString
// 作者:    	郑志兵
// 说明:		这是检验传输是否正确的函数
// 日期:      	2018-2-22 14:58:10
//************************************
inline QString MyProtocol::generateCRCResult(QByteArray CRC40, float f_data)
{
	QString result;
	if (CRC8Table_40(CRC40)) {
		result.append("CRC is error：" + CRC40.toHex());
	}
	else
	{
		result.append(tr("%1").arg(f_data) + "	");
	}
	return result;
}

//************************************
// 限定: 	   
// 函数名:    	generateCRCResult
// 形参: 	    QByteArray CRC40
// 形参: 	    QByteArray dat
// 返回值:   	QT_NAMESPACE::QString
// 作者:    	郑志兵
// 说明:		这是检验传输是否正确的函数
// 日期:      	2018-2-22 15:17:06
//************************************
QString MyProtocol::generateCRCResult(QByteArray CRC40, QByteArray dat)
{
	QString result;
	if (CRC8Table_40(CRC40)) {
		result.append("CRC is error：" + CRC40.toHex());
	}
	else
	{
		result.append(dat);
	}
	return result;
}

//************************************
// 限定: 	   
// 函数名:    	createProtocolPacket
// 形参: 	    SenderID senderid
// 形参: 	    DataID dataid
// 形参: 	    float data
// 返回值:   	QT_NAMESPACE::QByteArray
// 作者:    	郑志兵
// 说明:		这是将数据以规定的格式打包
// 日期:      	2018-2-22 14:59:48
//************************************
QByteArray MyProtocol::createProtocolPacket(SenderID senderid, DataID dataid, float data)
{
	QByteArray packet;
	QByteArray str;
	packet.append(senderid);
	packet.append(dataid);
	str = mymath.floatToFourBytes(data);
	for (int i = 0;i < 4;i++) {
		packet.append(str.at(i));
	}
	packet.append(CRC8Table_32(str));
	return packet;
}

//************************************
// 限定: 	   
// 函数名:    	createProtocolPacket
// 形参: 	    SenderID senderid
// 形参: 	    DataID dataid
// 形参: 	    QByteArray data
// 返回值:   	QT_NAMESPACE::QByteArray
// 作者:    	郑志兵
// 说明:		这是将数据以规定的格式打包，需要注意的时候data的长度不能超过4字节
//				如果长度超过4就会被截断
// 日期:      	2018-2-22 15:00:54
//************************************
QByteArray MyProtocol::createProtocolPacket(SenderID senderid, DataID dataid, QByteArray data)
{
	QByteArray packet;
	for (int i = 0; i < (4 - data.length()); i++) {
		data.append('\0');
	}
	if (data.length() > 4) {
		data = data.mid(0, 4);
	}
	packet.append(senderid);
	packet.append(dataid);
	packet.append(data);
	packet.append(CRC8Table_32(data));
	return packet;
}

//************************************
// 限定: 	   
// 函数名:    	analyseProtocolPacket
// 形参: 	    QByteArray packet
// 返回值:   	QT_NAMESPACE::QString
// 作者:    	郑志兵
// 说明:		这是对接收到的数据进行分析，如果接收的数据长度不是7的倍数，也就是说
//				接收到的不是数据包，那么将直接返回原值，如果是数据包那么将根据既定
//				的协议进行解析
// 日期:      	2018-2-22 15:11:49
//************************************
QString MyProtocol::analyseProtocolPacket(QByteArray packet)
{
	QString result = {};
	QByteArray data;
	QByteArray CRC40;
	float f_data=0.0;
	int length=packet.length();
	
	if (length % 7) {
		result = QString::fromLocal8Bit(packet);
	}
	else {
		int multiple = length / 7;
		int mul = multiple;
		while (multiple)
		{
			data.clear();
			CRC40.clear();
			f_data = 0;
			QByteArray packet1;
			packet1 = packet.mid((mul - multiple) * 7, 7);

			for (int i = 0; i < 4; i++) {
				data.append(packet1.at(i + 2));
				CRC40.append(packet1.at(i + 2));
			}
			CRC40.append(packet1.at(6));
			f_data = mymath.fourBytesToFloat(data);
			//Check the id of the slave
			switch (packet1.at(0))
			{
			case MyProtocol::PC:
				result.append("PC:");
				break;
			case MyProtocol::AR_Chasis:
				result.append("AR_Chasis：");
				break;
			case MyProtocol::MR_Chasis:
				result.append("MR_Chasis：");
				break;
			case MyProtocol::Execute:
				result.append("Execute：");
				break;
			case MyProtocol::GamePad:
				result.append("GamePad：");
				break;
			default:
				result.append(tr("SenderID is error：" + packet1.toHex().at(0)));
				break;
			}

			switch (packet1.at(1))
			{
			case MyProtocol::Check:
				result.append("Check "); break;
			case MyProtocol::AR_ChasisCamera_dx:
				result.append("AR_ChasisCamera_dx "); break;
			case MyProtocol::AR_ChasisCamera_dy:
				result.append("AR_ChasisCamera_dy "); break;
			case MyProtocol::AR_ChasisCamera_angle:
				result.append("AR_ChasisCamera_angle "); break;
			case MyProtocol::AR_Kinect_dx:
				result.append( "AR_Kinect_dx "); break;
			case MyProtocol::AR_Kinect_dz:
				result.append( "AR_Kinect_dz "); break;
			case MyProtocol::AR_Kinect_bool:
				result.append( "AR_Kinect_bool "); break;
			case MyProtocol::AR_RelativeCamera_dx:
				result.append( "AR_RelativeCamera_dx "); break;
			case MyProtocol::AR_RelativeCamera_dy:
				result.append( "AR_RelativeCamera_dy "); break;
			case MyProtocol::AR_RelativeCamera_data:
				result.append( "AR_RelativeCamera_data "); break;
			case MyProtocol::MR_ChasisCamera_dx:
				result.append( "MR_ChasisCamera_dx "); break;
			case MyProtocol::MR_ChasisCamera_dy:
				result.append( "MR_ChasisCamera_dy "); break;
			case MyProtocol::MR_ChasisCamera_angle:
				result.append( "MR_ChasisCamera_angle "); break;
			case MyProtocol::MR_BallRockCameraHorizon_dx:
				result.append( "MR_BallRockCameraHorizon_dx "); break;
			case MyProtocol::MR_BallRockCameraHorizon_dy:
				result.append( "MR_BallRockCameraHorizon_dy "); break;
			case MyProtocol::MR_BallRockCameraHorizon_data:
				result.append( "MR_BallRockCameraHorizon_data "); break;
			case MyProtocol::MR_BallRockCameraVertical_dx:
				result.append("MR_BallRockCameraVertical_dx "); break;
			case MyProtocol::MR_BallRockCameraVertical_dy:
				result.append("MR_BallRockCameraVertical_dy "); break;
			case MyProtocol::MR_BallRockCameraVertical_data:
				result.append("MR_BallRockCameraVertical_data "); break;
			case MyProtocol::GamePad_A:
				result.append( "GamePad_A "); break;
			case MyProtocol::GamePad_B:
				result.append( "GamePad_B "); break;
			case MyProtocol::GamePad_C:
				result.append( "GamePad_C "); break;
			case MyProtocol::GamePad_D:
				result.append( "GamePad_D "); break;
			case MyProtocol::GamePad_E:
				result.append( "GamePad_E "); break;
			case MyProtocol::GamePad_F:
				result.append( "GamePad_F "); break;
			case MyProtocol::GamePad_G:
				result.append( "GamePad_G "); break;
			case MyProtocol::GamePad_H:
				result.append( "GamePad_H "); break;
			case MyProtocol::GamePad_I:
				result.append( "GamePad_I "); break;
			case MyProtocol::GamePad_J:
				result.append( "GamePad_J "); break;
			case MyProtocol::GamePad_K:
				result.append( "GamePad_K "); break;
			case MyProtocol::GamePad_L:
				result.append( "GamePad_L "); break;
			case MyProtocol::GamePad_M:
				result.append( "GamePad_M "); break;
			case MyProtocol::GamePad_N:
				result.append( "GamePad_N "); break;
			case MyProtocol::GamePad_O:
				result.append( "GamePad_O "); break;
			case MyProtocol::GamePad_P:
				result.append( "GamePad_P "); break;
			case MyProtocol::GamePad_Q:
				result.append( "GamePad_Q "); break;
			case MyProtocol::GamePad_R:
				result.append( "GamePad_R "); break;
			case MyProtocol::GamePad_S:
				result.append( "GamePad_S "); break;
			case MyProtocol::GamePad_T:
				result.append( "GamePad_T "); break;
			case MyProtocol::GamePad_U:
				result.append( "GamePad_U "); break;
			case MyProtocol::GamePad_V:
				result.append( "GamePad_V "); break;
			case MyProtocol::GamePad_W:
				result.append( "GamePad_W "); break;
			case MyProtocol::GamePad_X:
				result.append( "GamePad_X "); break;
			case MyProtocol::GamePad_Y:
				result.append( "GamePad_Y "); break;
			case MyProtocol::GamePad_Z:
				result.append( "GamePad_Z "); break;
			default:
				result.append( "DataID error: " + packet1.toHex().at(1));
				break;
			}
			if (packet1.at(1) >= MyProtocol::GamePad_A) {
				result.append(generateCRCResult(CRC40, data));
			}
			else {
				result.append(generateCRCResult(CRC40, f_data));
			}
			multiple--;		
		}	
	}
	//result.append("\n");
	return result;
}
