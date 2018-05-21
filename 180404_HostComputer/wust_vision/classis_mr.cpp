#include "classis_mr.h"

namespace mv
{
	using namespace std;
	GetLineIFM_MR::GetLineIFM_MR()
	{
		Flag1 = false;
		Flag2 = false;
		shuxianFlag = false;
		lastX = 0.f;
		lastY = 0.f;
		LeftRightOutArea = 0;
		UpDownOutArea = 0;
		Line1K = 0;
		Line2K = 0;
		Line1B = 0;
		Line2B = 0;
		Factor30MM = 36;
		FactorRange = 8;
		CenterDst = cv::Point2f(0.f, 0.f);
		CenterRobot = cv::Point2f(simpleCameraWidth*0.25f, simpleCameraHeight*0.25f);
		LastSrc = cv::Mat::zeros(cv::Size(simpleCameraWidth*resizeHalf, simpleCameraHeight*resizeHalf), CV_8UC3);
		LineCordinateConvert = Counterclockwise90;
		Line_X_Y_Swap = 0;
		Line_X_Contrary = 0;
		Line_Y_Contrary = 0;
		switch (LineCordinateConvert)
		{
		case Straight:Line_X_Y_Swap = 0;
			Line_X_Contrary = -1;
			Line_Y_Contrary = 1;
			break;
		case Counterclockwise90:Line_X_Y_Swap = 1;
			Line_X_Contrary = -1;
			Line_Y_Contrary = -1;
			break;
		case Clockwise90:Line_X_Y_Swap = 1;
			Line_X_Contrary = 1;
			Line_Y_Contrary = 1;
			break;
		case Counter:Line_X_Y_Swap = 0;
			Line_X_Contrary = 1;
			Line_Y_Contrary = -1;
			break;
		default:
			break;
		}

		/*cv::FileStorage fs2("params/genius.xml", cv::FileStorage::READ);

		cv::Mat cameraMatrix2, distCoeffs2;
		fs2["cameraMatrix"] >> cameraMatrix2;
		fs2["cameraDistcoeff"] >> distCoeffs2;

		cv::fisheye::initUndistortRectifyMap(cameraMatrix2, distCoeffs2, cv::Matx33d::eye(), cameraMatrix2, cv::Size(640, 480), CV_16SC2, map1, map2);


		cv::FileStorage fs1("params/map_genius.xml", cv::FileStorage::WRITE);
		fs1 << "map1" << map1;
		fs1 << "map2" << map2;*/

		cv::FileStorage fs("params/map_genius.xml", cv::FileStorage::READ);
		fs["map1"] >> map1;
		fs["map2"] >> map2;

		//outputVideo.open(outputVideoPath, CV_FOURCC('M', 'J', 'P', 'G'), 25.0, videoResolution, true);
		

	}

	GetLineIFM_MR::~GetLineIFM_MR()
	{
		/*if (CameraIsOpen)
		{
			Camera0.CloseCamera();
		}*/
	}

	//_IFM 1*3数组，分别为Δx Δy angel
	vector<float> GetLineIFM_MR::IFM()
	{
		if (!CameraIsOpen)
		{
			std::cout << "相机未打开。" << std::endl;
		}
		else
		{
			vector<float> _IFM;
			for (int i = 0; i < 3; i++)
			{
				_IFM.push_back(0.f);
			}
			double minp = 0.0;
			double maxp = 0.0;
			if (classis_mode == Classis_Line)
			{
				int64 time = cv::getTickCount();
				cv::Mat src, src_resize, dst, Bin;
				cv::Mat for_show;
				//src=imread("wust_vision/image/7.bmp");

				cap >> src;
				if (src.empty())
				{
					system("pause");
				}

				cv::Mat remap_src;
				remap(src, remap_src, map1, map2, cv::INTER_LINEAR);
				cv::Mat __src(remap_src, cv::Rect(55, 40, 490, 430));
				cv::resize(__src, src_resize, cv::Size((int)simpleCameraWidth*resizeHalf, (int)simpleCameraHeight*resizeHalf));
				//do
				//{
				//	IplImage *pFrame1 = Camera0.QueryFrame();
				//	if (pFrame1 != NULL)
				//	{
				//		src = cv::cvarrToMat(pFrame1, true);

				//		cv::Mat remap_src;
				//		remap(src, remap_src, map1, map2, cv::INTER_LINEAR);
				//		//cv::Mat __src(remap_src, cv::Rect(0, 0, simpleCameraWidth, simpleCameraHeight));
				//		cv::Mat __src(remap_src, cv::Rect(60, 0, 450, 370));
				//		cv::resize(__src, src_resize, cv::Size(simpleCameraWidth*resizeHalf, simpleCameraHeight*resizeHalf));
				//		break;
				//	}
				//} while (true);

				src_resize.copyTo(LastSrc);
				src_resize.copyTo(for_show);
				cv::Mat image_R, image_G, image_B;
				/*my_split(src_resize, image_R, MV_GET_R);
				my_split(src_resize, image_G, MV_GET_G);
				dst = image_G - image_R;*/
				my_split(src_resize, image_R, MV_GET_R);
				my_split(src_resize, image_G, MV_GET_G);
				my_split(src_resize, image_B, MV_GET_B);


				/*cv::Mat image_s;
				my_split(src_resize, image_s, MV_GET_S);
				cv::imshow("image_s", image_s);*/
				dst = image_G - image_R;
				//dst = cv::Mat(cv::Size(simpleCameraWidth*resizeHalf, simpleCameraHeight*resizeHalf), CV_8UC3, cv::Scalar(20));
				blur(dst, dst, cv::Size(5, 5));

				cv::Mat _mask(cv::Size(simpleCameraWidth*resizeHalf, simpleCameraHeight*resizeHalf), CV_8UC1, cv::Scalar(80));
				_mask.copyTo(dst, dst > 10);
				_mask.copyTo(dst, image_B < 80);
				_mask.copyTo(dst, image_G < 80);
				_mask.copyTo(dst, image_R < 80);

				//不再自己去遍历了，因为debug模式下很慢
				//for (int i = 0; i < simpleCameraHeight*resizeHalf; i++)
				//{
				//	Vec3b *p = src_resize.ptr<Vec3b>(i);
				//	for (int j = 0; j < simpleCameraWidth*resizeHalf; j++)
				//	{
				//		/*int _middle;
				//		_middle = (p[j][0] + p[j][1] + p[j][2]) / 3;
				//		int _BC;
				//		_BC = 0;
				//		for (int i = 0; i < 3; i++)
				//		{
				//		_BC += abs(p[j][i]-_middle);
				//		}*/

				//		if (p[j][1] - p[j][2]>20
				//			|| p[j][0] < 120
				//			|| p[j][1] < 120
				//			|| p[j][2] < 120)
				//			dst.ptr(i)[j] = 80;
				//		//if (p[j][0] > 120
				//		//	&& p[j][1] > 120
				//		//	&& p[j][2] > 120
				//		//	&& (abs(p[j][1] - p[j][2]) <= 15
				//		//		||abs(p[j][0]-p[j][2]<=20))
				//		//	/*&& _BC<30*/
				//		//	)
				//		//{

				//		//}
				//		//else
				//		//{
				//		//	dst.ptr(i)[j] = 80;
				//		//}
				//	}
				//}

				//threshold(dst, dst, 30, 255, THRESH_TRUNC);
				threshold(dst, Bin, 30, 255, CV_THRESH_BINARY_INV);

				cv::Mat _element;
				_element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
				morphologyEx(Bin, Bin, cv::MORPH_OPEN, _element);
				cv::Mat _forContour;
				vector<vector<cv::Point>> _points;
				Bin.copyTo(_forContour);
				findContours(_forContour, _points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

				//找到边缘点数最多的轮廓
				int contoursNum = _points.size();
				int maxPointsConNum = -1;
				int maxPoints = 300;
				for (size_t i = 0; i < contoursNum; i++)
				{
					if (_points[i].size() > maxPoints)
					{
						maxPoints = _points[i].size();
						maxPointsConNum = i;
					}
				}

				cv::Mat lineContour(cv::Size(simpleCameraWidth*resizeHalf, simpleCameraHeight*resizeHalf), CV_8UC1, cv::Scalar(0));
				if (maxPointsConNum > -1)
				{
					drawContours(lineContour, _points, maxPointsConNum, cv::Scalar(255), -1);
				}
				/*cv::Mat _element;
				_element = getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
				morphologyEx(Bin, Bin, cv::MORPH_OPEN, _element);*/
				//GaussianBlur(dst, dst, cv::Size(5, 5), 0, 0);
				cv::Mat canny_dst;
				//blur(dst, dst, cv::Size(5, 5));
				Canny(lineContour, canny_dst, /*41, 121*/41, 41 * 3, 3);
				line(canny_dst, cv::Point(0, 0), cv::Point(simpleCameraWidth*resizeHalf - 1, 0), cv::Scalar(0), 2);
				line(canny_dst, cv::Point(simpleCameraWidth*resizeHalf - 1, 0), cv::Point(simpleCameraWidth*resizeHalf - 1, simpleCameraHeight*resizeHalf - 1), cv::Scalar(0), 2);
				line(canny_dst, cv::Point(0, simpleCameraHeight*resizeHalf - 1), cv::Point(simpleCameraWidth*resizeHalf - 1, simpleCameraHeight*resizeHalf - 1), cv::Scalar(0), 2);
				line(canny_dst, cv::Point(0, 0), cv::Point(0, simpleCameraHeight*resizeHalf - 1), cv::Scalar(0), 2);
				//cv::rectangle(canny_dst, cv::Rect(0, 165, 49, 75), cv::Scalar(0), -1);
				Canny(canny_dst, canny_dst, 41, 41 * 3, 3);

				//线变化不能限制最大长度
				//若线特别好（很直不粗糙，就必须奖励，人为增加点数占比）
				vector<cv::Vec4i> lines;
				HoughLinesP(canny_dst, lines, 1, CV_PI / 180, 5, 5, 1);
				vector<cv::Point2f> points_x;
				vector<cv::Point2f> points_y;
				//将点分类
				int lineSize = lines.size();
				for (int i = 0; i < lineSize; i++)
				{
					cv::Vec4i l = lines[i];

					if (l[0] != l[2])//x
					{
						if (abs(l[3] - l[1]) > abs(l[2] - l[0]))//Δy>Δx 竖线
						{
							if (abs(l[3] - l[1]) > 15)
							{
								char _flag_x = 0;
								char _flag_y = 0;
								if (l[0] > l[2])
									_flag_x = -1;
								else
									_flag_x = 1;
								if (l[1] > l[3])
									_flag_y = -1;
								else
									_flag_y = 1;
								float step = 5.f;
								float _x = step * abs(l[0] - l[2]) / abs(l[3] - l[1]);//x_step
								for (int i = 0, j = 1; i < abs(l[3] - l[1]);)
								{
									points_x.push_back(cv::Point2f(l[0] + _flag_x*_x*j, l[1] + _flag_y * step * j));
									j++;
									points_x.push_back(cv::Point2f(l[0] + _flag_x*_x*j, l[1] + _flag_y * step * j));
									j++;
									//最后会看到线长被double的现象
									i += step;
								}
							}
							points_x.push_back(cv::Point2f(l[0], l[1]));
							points_x.push_back(cv::Point2f(l[2], l[3]));
						}
						else
						{
							if (abs(l[2] - l[0]) > 15)
							{
								char _flag_x = 0;
								char _flag_y = 0;
								if (l[0] > l[2])
									_flag_x = -1;
								else
									_flag_x = 1;
								if (l[1] > l[3])
									_flag_y = -1;
								else
									_flag_y = 1;
								float step = 6.f;
								float _y = step * abs(l[3] - l[1]) / abs(l[2] - l[0]);
								for (int i = 0, j = 1; i < abs(l[2] - l[0]);)
								{
									points_y.push_back(cv::Point2f(l[0] + _flag_x*step*j, l[1] + _flag_y * _y * j));
									j++;
									points_y.push_back(cv::Point2f(l[0] + _flag_x*step*j, l[1] + _flag_y * _y * j));
									j++;
									i += step;
								}
							}
							points_y.push_back(cv::Point2f(l[0], l[1]));
							points_y.push_back(cv::Point2f(l[2], l[3]));
						}
					}
					else
					{
						if (abs(l[3] - l[1]) > 15)
						{
							char _flag_x = 0;
							char _flag_y = 0;
							if (l[0] > l[2])
								_flag_x = -1;
							else
								_flag_x = 1;
							if (l[1] > l[3])
								_flag_y = -1;
							else
								_flag_y = 1;
							float step = 5.f;
							float _x = step * abs(l[0] - l[2]) / abs(l[3] - l[1]);
							for (int i = 0, j = 1; i < abs(l[3] - l[1]);)
							{
								points_x.push_back(cv::Point2f(l[0] + _flag_x*_x*j, l[1] + _flag_y * step * j));
								j++;
								points_x.push_back(cv::Point2f(l[0] + _flag_x*_x*j, l[1] + _flag_y * step * j));
								j++;
								i += step;
							}
						}
						points_x.push_back(cv::Point2f(l[0], l[1]));
						points_x.push_back(cv::Point2f(l[2], l[3]));
					}
				}
				//画点
				/*int point_ySize = points_y.size();
				for (int i = 0; i < point_ySize; i++)
				{
					circle(for_show, points_y[i], 4, cv::Scalar(0, 255, 0), -1);
				}
				int point_xSize = points_x.size();
				for (int i = 0; i < point_xSize; i++)
				{
					cv::circle(for_show, points_x[i], 4, cv::Scalar(0, 0, 255), -1, -1);
				}*/

				solvePointsX(points_x, for_show, Bin);
				solvePointsY(points_y, for_show, Bin);


				cv::Point2f _center = CenterDst;

				if (!Flag2)
				{
					if (UpDownOutArea == 0)
					{
						Line2K = 0;
						Line2B = 10;
					}
					else
					{
						Line2K = 0;
						Line2B = simpleCameraHeight*resizeHalf - 10;
					}
				}


				float _deltaX, _deleteY, _angel;


				if (!Flag1)
				{
					if (shuxianFlag)
					{
						CenterDst.x = Line1B;
						CenterDst.y = Line2K*CenterDst.x + Line2B;
						shuxianFlag = false;
						_angel = 0.0;
					}
					else
					{
						if (LeftRightOutArea == 0)
						{
							CenterDst.x = 10;
							CenterDst.y = Line2K*CenterDst.x + Line2B;
						}
						else
						{
							CenterDst.x = simpleCameraWidth*resizeHalf - 10;
							CenterDst.y = Line2K*CenterDst.x + Line2B;
						}
						_angel = atan(Line2K) * 180.0 / 3.14159;
					}

				}
				else
				{
					CenterDst.x = (Line2B - Line1B) / (Line1K - Line2K);
					CenterDst.y = Line1K*CenterDst.x + Line1B;
					_angel = atan(Line1K) * 180.0 / 3.14159;
					//if (Flag2)
					//{
					//	_angel = atan(Line2K) * 180.0 / 3.14159;
					//}
					//else
					//{
					if (_angel > 0.f)
					{
						_angel -= 90.f;
					}
					else
					{
						_angel += 90.f;
					}
					//}

				}


				if ((int)CenterDst.x < 10)
					CenterDst.x = 10;
				else if ((int)CenterDst.x > simpleCameraWidth*resizeHalf - 10)
					CenterDst.x = simpleCameraWidth*resizeHalf - 10;

				if ((int)CenterDst.y < 10)
					CenterDst.y = 10;
				else if ((int)CenterDst.y > simpleCameraHeight*resizeHalf - 10)
					CenterDst.y = simpleCameraHeight*resizeHalf - 10;

				if (!Flag2&&points_y.size() > 100)
				{
					CenterDst.y = lastY;
				}
				else
				{
					lastY = CenterDst.y;
				}

				if (!Flag1&&points_x.size() > 100)
				{
					CenterDst.x = lastX;
				}
				else
				{
					lastX = CenterDst.x;
				}
				_deltaX = (CenterRobot.x - CenterDst.x)*30.0 / Factor30MM;
				_deleteY = (CenterRobot.y - CenterDst.y)*30.0 / Factor30MM;
				CoordinateConvert(_IFM, _deltaX, _deleteY, -_angel);

				if (_deltaX > 0)
					LeftRightOutArea = 0;
				else
					LeftRightOutArea = 1;
				if (_deleteY > 0)
					UpDownOutArea = 0;
				else
					UpDownOutArea = 1;




				/*if (abs(_center.x - CenterDst.x) > 30 || abs(_center.y - CenterDst.y) > 30)
				{
				cv::waitKey();
				}*/
				//cv::line(for_show, cv::Point(560 / 2, 0), cv::Point(560 / 2, 480 / 2), cv::Scalar(180, 255, 100), 2);

				cv::circle(for_show, CenterDst, 7, cv::Scalar(255, 0, 0), -1);
				cv::circle(for_show, CenterRobot, 7, cv::Scalar(200, 100, 250), -1);
				cv::imshow("canny_dst", canny_dst);
				cv::imshow("for_show", for_show);
				cv::imshow("dst", dst);
				cv::imshow("Bin", Bin);
				cv::waitKey(1);

				// 				if ((points_x.size() > 100 || points_y.size() > 100)
				// 					&& (!Flag1 || !Flag2))
				// 				{
				// 					cv::waitKey();
				// 				}

				DSP = cv::getTickFrequency() / (cv::getTickCount() - time);
				//_IFM[0] = (float)DSP;
				//std::cout << "x.size():	" << points_x.size() << "   y.size():	" << points_y.size();
				//std::cout << "	帧率：" << getTickFrequency() / (getTickCount() - time) << endl;
				//cout << "DSP" << 1 / DSP << endl;

				Flag1 = false;
				Flag2 = false;

			}
			return _IFM;
		}
	}

	void GetLineIFM_MR::calculateMiddleLine(const vector<cv::Point2f> &points_left, const vector<cv::Point2f> &points_right, int _mode)
	{
		double a_left, b_left, c_left, q_left;
		double a_right, b_right, c_right, q_right;
		lineFit_X(points_left, a_left, b_left, c_left, q_left);
		lineFit_X(points_right, a_right, b_right, c_right, q_right);
		if (_mode == 1)
		{
			vector<cv::Point2f> _points;
			for (int i = 0; i < simpleCameraHeight*resizeHalf; i += simpleCameraHeight / 6)
			{
				cv::Point2f _point = cv::Point2f(((-c_left - b_left*i) / a_left + (-c_right - b_right*i) / a_right) / 2.f, (float)i);
				_points.push_back(_point);
			}
			double a_last, b_last, c_last, q_last;
			lineFit_X(_points, a_last, b_last, c_last, q_last);
			if (b_last == 0.)
			{
				shuxianFlag = true;
				Line1B = (float)-(c_last / a_last);
			}
			else
			{
				Line1K = (float)-(a_last / b_last);
				Line1B = (float)-(c_last / b_last);
				Flag1 = true;
			}
		}
		else if (_mode == 2)
		{
			vector<cv::Point2f> _points;
			for (int i = 0; i < simpleCameraWidth*resizeHalf; i += simpleCameraWidth / 6)
			{
				cv::Point2f _point = cv::Point2f((float)i, ((-c_left - a_left*i) / b_left + (-c_right - a_right*i) / b_right) / 2.f);
				_points.push_back(_point);
			}
			double a_last, b_last, c_last, q_last;
			lineFit_X(_points, a_last, b_last, c_last, q_last);
			Line2K = (float)-(a_last / b_last);
			Line2B = (float)-(c_last / b_last);
			Flag2 = true;
		}

	}
	void GetLineIFM_MR::GetMode(bool _mode)
	{
		if (this != nullptr)
		{
			if (_mode)
			{
				classis_mode = Classis_Junction;
			}
			else
			{
				classis_mode = Classis_Line;
			}
		}
	}

	void GetLineIFM_MR::solvePointsX(const std::vector<cv::Point2f> &points_x, cv::Mat& for_show, cv::Mat& _Bin)
	{
		vector<vector<cv::Point2f>> points_ElsePart;
		vector<vector<cv::Point2f>> points_UsePart;
		cout << "points_x.size():" << points_x.size() << endl;
		if (points_x.size() > 60)
		{
			points_ElsePart.push_back(points_x);
			vector<cv::Vec4d> line_IFM;
			bool noLine_flag = false;
			int line_num = 0;
			int rng_circle_num = 0;
			int elsePartSize = points_ElsePart[line_num].size();
			int points_judge = 38;
			while (!noLine_flag&&
				elsePartSize >= points_judge)
			{

				if (!line_num)
				{
					rng_circle_num = 295;
				}
				else
				{
					rng_circle_num = 290;
				}
				for (int i = 0, j = line_num; i < rng_circle_num&&j == line_num; i++)//拟合rng_circle_num根线
				{
					vector<cv::Point2f> point_1;
					int find_maxNum = 0;
					point_1.push_back(points_ElsePart[line_num][line_rng.uniform(0, elsePartSize)]);
					for (int i = 0; i < 3; )
					{
						int a = line_rng.uniform(0, elsePartSize);
						find_maxNum++;
						if (find_maxNum > 60)
						{
							break;
						}
						if (pow((point_1[0].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[0].y - points_ElsePart[line_num][a].y, 2) < 121)
							continue;
						if (i >= 1)
						{
							if (pow((point_1[1].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[1].y - points_ElsePart[line_num][a].y, 2) < 121)
								continue;
						}
						if (i >= 2)
						{
							if (pow((point_1[2].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[2].y - points_ElsePart[line_num][a].y, 2) < 121)
								continue;
						}
						point_1.push_back(points_ElsePart[line_num][a]);
						i++;
					}

					if (find_maxNum > 30)
					{
						noLine_flag = true;
						break;
					}

					double a = 0.0, b = 0.0, c = 0.0, p = 0.0;
					lineFit_X(point_1, a, b, c, p);

					if (p < 4.2)
					{
						//系数较小，是目标线的概率非常大
						//进行区域判断
						vector<cv::Point2f> points_judgeLine;//judge line with this values' size
						vector<cv::Point2f> points_else;
						for (int m = 0; m < elsePartSize; m += 2)
						{
							float _middle = (-c - b*points_ElsePart[line_num][m].y) / a;
							if (points_ElsePart[line_num][m].x > _middle - FactorRange && points_ElsePart[line_num][m].x < _middle + FactorRange)
							{
								points_judgeLine.push_back(points_ElsePart[line_num][m]);
								points_judgeLine.push_back(points_ElsePart[line_num][m + 1]);
							}
							else
							{
								points_else.push_back(points_ElsePart[line_num][m]);
								points_else.push_back(points_ElsePart[line_num][m + 1]);
							}
						}
						if ((points_judgeLine.size() >= points_judge
							&& points_judgeLine.size() >= points_x.size() * 0.3)
							|| points_judgeLine.size() >= points_ElsePart[line_num].size()*0.6)//区域内有多余points_judge个点存在,此处100%是目标线(至少得这样认为)
						{
							cv::Vec4d line_x;
							line_x[0] = a;
							line_x[1] = b;
							line_x[2] = c;
							line_x[3] = p;
							line_IFM.push_back(line_x);
							points_ElsePart.push_back(points_else);
							points_UsePart.push_back(points_judgeLine);
							//cout << "i:	" << i << "		" << "find_maxNum:	" << find_maxNum << endl;
							line_num++;
							elsePartSize = points_ElsePart[line_num].size();
						}
					}
					if (i == rng_circle_num - 1)
					{
						//没线集合了
						noLine_flag = true;
					}
				}

			}

			int lineIFMSize = line_IFM.size();
			//cout << "X---line_IFM.size():	" << lineIFMSize << endl;
			/*for (int i = 0; i < lineIFMSize; i++)
			{
				line(for_show, cv::Point2f((-line_IFM[i][2]) / line_IFM[i][0], 0.0), cv::Point2f(((-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0]), 240.0f), cv::Scalar(0, 180, 250), 3);
				line(for_show, cv::Point2f((-line_IFM[i][2]) / line_IFM[i][0] + FactorRange, 0.0), cv::Point2f(((-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0]) + FactorRange, 240.0f), cv::Scalar(200, 180, 250), 1);
				line(for_show, cv::Point2f((-line_IFM[i][2]) / line_IFM[i][0] - FactorRange, 0.0), cv::Point2f(((-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0]) - FactorRange, 240.0f), cv::Scalar(200, 180, 250), 1);
			}*/

			//只要有线，就必须保证能检测出来，然后下一步		
			if (lineIFMSize >= 2)
			{
				//float _minArea = Factor30MM * simpleCameraHeight * resizeHalf * 0.5f;
				for (int i = 0; i < lineIFMSize; i++)
				{
					float up_1 = 0.f, down_1 = 0.f;
					up_1 = (-line_IFM[i][2]) / line_IFM[i][0];
					down_1 = (-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0];
					for (int j = i + 1; j < lineIFMSize; j++)
					{
						cv::Mat Bin;
						_Bin.copyTo(Bin);
						float up_2 = 0.f, down_2 = 0.f;
						up_2 = (-line_IFM[j][2]) / line_IFM[j][0];
						down_2 = (-line_IFM[j][2] - line_IFM[j][1] * 240) / line_IFM[j][0];

						float _minArea = (fabs(up_1 - up_2) + fabs(down_1 - down_2)) * 0.5f * simpleCameraHeight * resizeHalf * 0.5f;
						//左右之分
						if (up_1 - up_2<Factor30MM*4.f
							&&down_1 - down_2<Factor30MM*4.f
							&& up_1 - up_2 > Factor30MM / 2.f
							&&down_1 - down_2 > Factor30MM / 2.f)
						{
							//cout << "up_1: is right !" << endl;
							line(Bin, cv::Point2f((-line_IFM[i][2]) / line_IFM[i][0], 0.0), cv::Point2f(((-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0]), 240.0f), cv::Scalar(0), 1);
							line(Bin, cv::Point2f((-line_IFM[j][2]) / line_IFM[j][0], 0.0), cv::Point2f(((-line_IFM[j][2] - line_IFM[j][1] * 240) / line_IFM[j][0]), 240.0f), cv::Scalar(0), 1);
							//腐蚀
							cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
							erode(Bin, Bin, element);

							cv::Mat forContour;
							Bin.copyTo(forContour);
							vector<vector<cv::Point>> contour_Points;
							findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
							int contourSize = contour_Points.size();
							for (int m = 0; m < contourSize; m++)
							{
								cv::Moments mu;
								cv::Point2f center;
								float area = contourArea(contour_Points[m]);
								if (area > _minArea)
								{
									mu = moments(contour_Points[m], false);
									center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
									if (center.x > (-line_IFM[j][2] - line_IFM[j][1] * center.y) / line_IFM[j][0]
										&& center.x < (-line_IFM[i][2] - line_IFM[i][1] * center.y) / line_IFM[i][0])
									{
										calculateMiddleLine(points_UsePart[j], points_UsePart[i], 1);
										//cout << "nice!" << endl;
									}
									cv::circle(Bin, center, 5, cv::Scalar(150), -1);
								}

							}

						}
						else if (fabs(up_1 - up_2) < Factor30MM*4.f
								 &&fabs(down_1 - down_2) < Factor30MM*4.f
							&& fabs(up_1 - up_2) > Factor30MM / 2.f
							&&fabs(down_1 - down_2) > Factor30MM / 2.f)
						{
							//cout << "up_2: is right !" << endl;
							line(Bin, cv::Point2f((-line_IFM[i][2]) / line_IFM[i][0], 0.0), cv::Point2f(((-line_IFM[i][2] - line_IFM[i][1] * 240) / line_IFM[i][0]), 240.0f), cv::Scalar(0), 1);
							line(Bin, cv::Point2f((-line_IFM[j][2]) / line_IFM[j][0], 0.0), cv::Point2f(((-line_IFM[j][2] - line_IFM[j][1] * 240) / line_IFM[j][0]), 240.0f), cv::Scalar(0), 1);
							//腐蚀
							cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
							erode(Bin, Bin, element);

							cv::Mat forContour;
							Bin.copyTo(forContour);
							vector<vector<cv::Point>> contour_Points;
							findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
							int contourSize = contour_Points.size();
							for (int m = 0; m < contourSize; m++)
							{
								cv::Moments mu;
								cv::Point2f center;
								float area = contourArea(contour_Points[m]);
								if (area > _minArea)
								{
									mu = moments(contour_Points[m], false);
									center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
									if (center.x < (-line_IFM[j][2] - line_IFM[j][1] * center.y) / line_IFM[j][0]
										&& center.x >(-line_IFM[i][2] - line_IFM[i][1] * center.y) / line_IFM[i][0])
									{
										calculateMiddleLine(points_UsePart[i], points_UsePart[j], 1);
										//cout << "nice!" << endl;
									}
									cv::circle(Bin, center, 5, cv::Scalar(150), -1);
								}

							}
						}

						//cout << "up_1 - up_2:	" << up_1 - up_2 << endl;
						//cout << "down_1 - down_2:	" << down_1 - down_2 << endl;

					}
				}
			}
			else if (lineIFMSize == 1)
			{
				cv::Mat Bin;
				_Bin.copyTo(Bin);
				float _minArea = 20 * simpleCameraHeight * resizeHalf * 0.4f;

				//左半
				line(Bin, cv::Point2f((-line_IFM[0][2]) / line_IFM[0][0], 0.0), cv::Point2f(((-line_IFM[0][2] - line_IFM[0][1] * 240) / line_IFM[0][0]), 240.0f), cv::Scalar(0), 1);
				line(Bin, cv::Point2f((-line_IFM[0][2]) / line_IFM[0][0] - 20, 0.0), cv::Point2f(((-line_IFM[0][2] - line_IFM[0][1] * 240) / line_IFM[0][0]) - 20, 240.0f), cv::Scalar(0), 1);
				//腐蚀，不能少。
				cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
				erode(Bin, Bin, element);

				cv::Mat forContour;
				Bin.copyTo(forContour);
				vector<vector<cv::Point>> contour_Points;
				findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				int contourSize = contour_Points.size();
				for (int m = 0; m < contourSize; m++)
				{
					cv::Moments mu;
					cv::Point2f center;
					float area = contourArea(contour_Points[m]);
					if (area > _minArea)
					{
						mu = moments(contour_Points[m], false);
						center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
						if (center.x > (-line_IFM[0][2] - line_IFM[0][1] * center.y) / line_IFM[0][0] - 20
							&& center.x < (-line_IFM[0][2] - line_IFM[0][1] * center.y) / line_IFM[0][0])
						{
							//cout << "nice!" << endl;
							double a_last, b_last, c_last, q_last;
							lineFit_X(points_UsePart[0], a_last, b_last, c_last, q_last);
							Line1K = (float)-(a_last / b_last);
							Line1B = (float)-(c_last / b_last) + Line1K*Factor30MM*0.5;
							Flag1 = true;
							cv::circle(Bin, center, 5, cv::Scalar(150), -1);
							//cout << "???????????????????????" << endl;
							return;
						}

					}

				}
				_Bin.copyTo(Bin);

				//右半
				line(Bin, cv::Point2f((-line_IFM[0][2]) / line_IFM[0][0], 0.0), cv::Point2f(((-line_IFM[0][2] - line_IFM[0][1] * 240) / line_IFM[0][0]), 240.0f), cv::Scalar(0), 1);
				line(Bin, cv::Point2f((-line_IFM[0][2]) / line_IFM[0][0] + 20, 0.0), cv::Point2f(((-line_IFM[0][2] - line_IFM[0][1] * 240) / line_IFM[0][0]) + 20, 240.0f), cv::Scalar(0), 1);
				//腐蚀
				erode(Bin, Bin, element);

				Bin.copyTo(forContour);
				vector<vector<cv::Point>> contour_Points_1;
				findContours(forContour, contour_Points_1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
				contourSize = contour_Points_1.size();
				for (int m = 0; m < contourSize; m++)
				{
					cv::Moments mu;
					cv::Point2f center;
					float area = contourArea(contour_Points_1[m]);
					if (area > _minArea)
					{
						mu = moments(contour_Points_1[m], false);
						center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
						if (center.x < (-line_IFM[0][2] - line_IFM[0][1] * center.y) / line_IFM[0][0] + 20
							&& center.x >(-line_IFM[0][2] - line_IFM[0][1] * center.y) / line_IFM[0][0])
						{
							double a_last, b_last, c_last, q_last;
							lineFit_X(points_UsePart[0], a_last, b_last, c_last, q_last);
							Line1K = (float)-(a_last / b_last);
							Line1B = (float)-(c_last / b_last) - Line1K*Factor30MM*0.5;
							Flag1 = true;
							//cout << "nice!" << endl;
							return;
						}
						//circle(Bin, center, 5, cv::Scalar(150), -1);
					}

				}
			}

			//imshow("__Bin", Bin);
		}
	}

	void GetLineIFM_MR::solvePointsY(const std::vector<cv::Point2f> &points_y, cv::Mat& for_show, cv::Mat& _Bin)
	{
		vector<vector<cv::Point2f>> points_ElsePart;
		vector<vector<cv::Point2f>> points_UsePart;
		cout << "points_y.size():" << points_y.size() << endl;
		if ((int)points_y.size() > 70)
		{
			points_ElsePart.push_back(points_y);
			vector<cv::Vec4d> line_IFM;
			bool noLine_flag = false;
			int line_num = 0;
			int rng_circle_num = 0;
			int elsePartSize = points_ElsePart[line_num].size();
			int points_judge = 38;
			while (!noLine_flag &&
				elsePartSize >= points_judge)
			{
				if (!line_num)
				{
					rng_circle_num = 295;
				}
				else
				{
					rng_circle_num = 290;
				}
				for (int i = 0, j = line_num; i < rng_circle_num&&j == line_num; i++)//拟合95根线}
				{
					vector<cv::Point2f> point_1;
					int find_maxNum = 0;
					point_1.push_back(points_ElsePart[line_num][line_rng.uniform(0, elsePartSize)]);
					for (int i = 0; i < 3; )
					{
						int a = line_rng.uniform(0, elsePartSize);
						find_maxNum++;
						if (find_maxNum > 60)
						{
							break;
						}
						if (pow((point_1[0].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[0].y - points_ElsePart[line_num][a].y, 2) < 121)
							continue;
						if (i >= 1)
						{
							if (pow((point_1[1].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[1].y - points_ElsePart[line_num][a].y, 2) < 121)
								continue;
						}
						if (i >= 2)
						{
							if (pow((point_1[2].x - points_ElsePart[line_num][a].x), 2) + pow(point_1[2].y - points_ElsePart[line_num][a].y, 2) < 121)
								continue;
						}
						point_1.push_back(points_ElsePart[line_num][a]);
						i++;
					}

					if (find_maxNum > 30)
					{
						noLine_flag = true;
						break;
					}

					double a = 0.0, b = 0.0, c = 0.0, p = 0.0;
					lineFit_Y(point_1, a, b, c, p);

					if (p < 4.2)
					{
						//系数较小，是目标线的概率非常大
						//进行区域判断
						vector<cv::Point2f> points_judgeLine;//judge line with this values' size
						vector<cv::Point2f> points_else;
						for (int i = 0; i < elsePartSize; i += 2)
						{
							float _middle = (-c - a*points_ElsePart[line_num][i].x) / b;
							if (points_ElsePart[line_num][i].y > _middle - FactorRange && points_ElsePart[line_num][i].y < _middle + FactorRange)
							{
								points_judgeLine.push_back(points_ElsePart[line_num][i]);
								points_judgeLine.push_back(points_ElsePart[line_num][i + 1]);
							}
							else
							{
								points_else.push_back(points_ElsePart[line_num][i]);
								points_else.push_back(points_ElsePart[line_num][i + 1]);
							}
						}

						if ((points_judgeLine.size() > points_judge
							&& points_judgeLine.size() >= points_y.size() * 0.3)
							|| points_judgeLine.size() >= points_ElsePart[line_num].size()*0.6)//区域内有多余points_judge个点存在,此处100%是目标线(至少得这样认为)
						{
							//cout << "???????????????????" << endl;
							cv::Vec4d line_x;
							line_x[0] = a;
							line_x[1] = b;
							line_x[2] = c;
							line_x[3] = p;
							line_IFM.push_back(line_x);
							points_ElsePart.push_back(points_else);
							points_UsePart.push_back(points_judgeLine);
							line_num++;
							elsePartSize = points_ElsePart[line_num].size();
						}
					}
					if (i == rng_circle_num - 1)
					{
						//没线集合了
						noLine_flag = true;
					}
				}

			}


			int lineIFMSize = line_IFM.size();
			//cout << "Y--line_IFM.size():	" << lineIFMSize << endl;
			/*for (int i = 0; i < lineIFMSize; i++)
			{
				line(for_show, cv::Point2f(0.0, (-line_IFM[i][2]) / line_IFM[i][1]), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[i][0] * simpleCameraWidth*resizeHalf - line_IFM[i][2]) / line_IFM[i][1]), cv::Scalar(186, 89, 201), 3);
				line(for_show, cv::Point2f(0.0, (-line_IFM[i][2]) / line_IFM[i][1] + FactorRange), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[i][0] * simpleCameraWidth*resizeHalf - line_IFM[i][2]) / line_IFM[i][1] + FactorRange), cv::Scalar(186, 89, 201), 1);
				line(for_show, cv::Point2f(0.0, (-line_IFM[i][2]) / line_IFM[i][1] - FactorRange), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[i][0] * simpleCameraWidth*resizeHalf - line_IFM[i][2]) / line_IFM[i][1] - FactorRange), cv::Scalar(186, 89, 201), 1);
			}*/

			//只要有线，就必须保证能检测出来，然后下一步
			if (lineIFMSize >= 2)
			{
				for (int i = 0; i < lineIFMSize; i++)
				{

					float left_1 = 0.f, right_1 = 0.f;
					left_1 = (-line_IFM[i][2]) / line_IFM[i][1];
					right_1 = (-line_IFM[i][2] - line_IFM[i][0] * simpleCameraWidth*resizeHalf) / line_IFM[i][1];
					for (int j = i + 1; j < lineIFMSize; j++)
					{

						cv::Mat Bin;
						_Bin.copyTo(Bin);
						float left_2 = 0.f, right_2 = 0.f;
						left_2 = (-line_IFM[j][2]) / line_IFM[j][1];
						right_2 = (-line_IFM[j][2] - line_IFM[j][0] * simpleCameraWidth*resizeHalf) / line_IFM[j][1];
						float _minArea = (fabs(left_1 - left_2) + fabs(right_1 - right_2)) / 2.f * simpleCameraWidth * resizeHalf * 0.5f;
						//上下之分
						if (left_1 - left_2<Factor30MM*4.f
							&&right_1 - right_2<Factor30MM*4.f
							&& left_1 - left_2 > Factor30MM / 2.f
							&&right_1 - right_2 > Factor30MM / 2.f)
						{
							//cout << "left_1: is down !" << endl;
							line(Bin, cv::Point2f(0.0, (-line_IFM[i][2]) / line_IFM[i][1]), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[i][0] * simpleCameraWidth*resizeHalf - line_IFM[i][2]) / line_IFM[i][1]), cv::Scalar(0), 1);
							line(Bin, cv::Point2f(0.0, (-line_IFM[j][2]) / line_IFM[j][1]), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[j][0] * simpleCameraWidth*resizeHalf - line_IFM[j][2]) / line_IFM[j][1]), cv::Scalar(0), 1);
							//腐蚀
							cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
							erode(Bin, Bin, element);

							cv::Mat forContour;
							Bin.copyTo(forContour);
							vector<vector<cv::Point>> contour_Points;
							findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
							int contourSize = contour_Points.size();
							for (int m = 0; m < contourSize; m++)
							{
								cv::Moments mu;
								cv::Point2f center;
								float area = contourArea(contour_Points[m]);
								if (area > _minArea)
								{
									mu = moments(contour_Points[m], false);
									center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
									if (center.y > (-line_IFM[j][2] - line_IFM[j][0] * center.x) / line_IFM[j][1]
										&& center.y < (-line_IFM[i][2] - line_IFM[i][0] * center.x) / line_IFM[i][1])
									{
										calculateMiddleLine(points_UsePart[j], points_UsePart[i], 2);
										//circle(Bin, center, 5, cv::Scalar(150), -1);
										//cout << "nice!" << endl;
									}

								}

							}

						}
						else if (fabs(left_1 - left_2) < Factor30MM *4.f
							&&fabs(right_1 - right_2) < Factor30MM*4.f
							&& fabs(left_1 - left_2) > Factor30MM / 2.f
							&&fabs(right_1 - right_2) > Factor30MM / 2.f)
						{
							//cout << "up_2: is right !" << endl;
							line(Bin, cv::Point2f(0.0, (-line_IFM[i][2]) / line_IFM[i][1]), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[i][0] * simpleCameraWidth*resizeHalf - line_IFM[i][2]) / line_IFM[i][1]), cv::Scalar(0), 1);
							line(Bin, cv::Point2f(0.0, (-line_IFM[j][2]) / line_IFM[j][1]), cv::Point2f(simpleCameraWidth*resizeHalf, (-line_IFM[j][0] * simpleCameraWidth*resizeHalf - line_IFM[j][2]) / line_IFM[j][1]), cv::Scalar(0), 1);
							//腐蚀
							cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
							erode(Bin, Bin, element);

							cv::Mat forContour;
							Bin.copyTo(forContour);
							vector<vector<cv::Point>> contour_Points;
							findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
							int contourSize = contour_Points.size();
							for (int m = 0; m < contourSize; m++)
							{
								cv::Moments mu;
								cv::Point2f center;
								float area = contourArea(contour_Points[m]);
								if (area > _minArea)
								{
									mu = moments(contour_Points[m], false);
									center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
									if (center.y < (-line_IFM[j][2] - line_IFM[j][0] * center.x) / line_IFM[j][1]
										&& center.y >(-line_IFM[i][2] - line_IFM[i][0] * center.x) / line_IFM[i][1])
									{
										calculateMiddleLine(points_UsePart[i], points_UsePart[j], 2);
										//circle(Bin, center, 5, cv::Scalar(150), -1);
										//cout << "nice!" << endl;
									}

								}

							}
						}
						//cout << "left1-left2:	" << left_1 - left_2 << endl;
						//cout << "right1-right2:	" << right_1 - right_2 << endl;
					}
				}
			}
			else if (lineIFMSize == 1)
			{
				cv::Mat Bin;
				_Bin.copyTo(Bin);
				float _minArea = 20 * simpleCameraHeight * resizeHalf * 0.4f;
				float left_1 = 0.f, right_1 = 0.f;
				
					//上半
					line(Bin, cv::Point2f(0.f, (-line_IFM[0][2]) / line_IFM[0][1]), cv::Point2f(simpleCameraWidth*resizeHalf, ((-line_IFM[0][2] - line_IFM[0][0] * simpleCameraWidth*resizeHalf) / line_IFM[0][1])), cv::Scalar(0), 1);
					line(Bin, cv::Point2f(0.f, (-line_IFM[0][2]) / line_IFM[0][1] - 20), cv::Point2f(simpleCameraWidth*resizeHalf, ((-line_IFM[0][2] - line_IFM[0][0] * simpleCameraWidth*resizeHalf) / line_IFM[0][1]) - 20), cv::Scalar(0), 1);
					//腐蚀
					cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
					erode(Bin, Bin, element);

					cv::Mat forContour;
					Bin.copyTo(forContour);
					vector<vector<cv::Point>> contour_Points;
					findContours(forContour, contour_Points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
					int contourSize = contour_Points.size();
					for (int m = 0; m < contourSize; m++)
					{
						cv::Moments mu;
						cv::Point2f center;
						float area = contourArea(contour_Points[m]);
						if (area > _minArea)
						{
							mu = moments(contour_Points[m], false);
							center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
							if (center.y > (-line_IFM[0][2] - line_IFM[0][0] * center.x) / line_IFM[0][1] - 20
								&& center.y < (-line_IFM[0][2] - line_IFM[0][0] * center.x) / line_IFM[0][1])
							{
								double a_last, b_last, c_last, q_last;
								lineFit_X(points_UsePart[0], a_last, b_last, c_last, q_last);
								Line2K = (float)-(a_last / b_last);
								Line2B = (float)-(c_last / b_last) - Factor30MM*0.5;
								Flag2 = true;

							}
							cv::circle(Bin, center, 5, cv::Scalar(150), -1);
							//cout << "nice!" << endl;
							return;
						}

					}
				_Bin.copyTo(Bin);
				
					//下半
					line(Bin, cv::Point2f(0.f, (-line_IFM[0][2]) / line_IFM[0][1]), cv::Point2f(simpleCameraWidth*resizeHalf, ((-line_IFM[0][2] - line_IFM[0][0] * simpleCameraWidth*resizeHalf) / line_IFM[0][1])), cv::Scalar(0), 1);
					line(Bin, cv::Point2f(0.f, (-line_IFM[0][2]) / line_IFM[0][1] + 20), cv::Point2f(simpleCameraWidth*resizeHalf, ((-line_IFM[0][2] - line_IFM[0][0] * simpleCameraWidth*resizeHalf) / line_IFM[0][1]) + 20), cv::Scalar(0), 1);
					//腐蚀
					element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
					erode(Bin, Bin, element);

					Bin.copyTo(forContour);
					vector<vector<cv::Point>> contour_Points_1;
					findContours(forContour, contour_Points_1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
					contourSize = contour_Points_1.size();
					for (int m = 0; m < contourSize; m++)
					{
						cv::Moments mu;
						cv::Point2f center;
						float area = contourArea(contour_Points_1[m]);
						if (area > _minArea)
						{
							mu = moments(contour_Points_1[m], false);
							center = cv::Point2f(static_cast<float>(mu.m10 / mu.m00), static_cast<float>(mu.m01 / mu.m00));
							if (center.y < (-line_IFM[0][2] - line_IFM[0][0] * center.x) / line_IFM[0][1] + 20
								&& center.y >(-line_IFM[0][2] - line_IFM[0][0] * center.x) / line_IFM[0][1])
							{
								double a_last, b_last, c_last, q_last;
								lineFit_X(points_UsePart[0], a_last, b_last, c_last, q_last);
								Line2K = (float)-(a_last / b_last);
								Line2B = (float)-(c_last / b_last) + Factor30MM*0.5;
								Flag2 = true;
								//cout << "nice!" << endl;
							}
							cv::circle(Bin, center, 5, cv::Scalar(150), -1);
						}

					}

			}
			//imshow("????Bin", Bin);
		}
	}

	//输入的 delta 表示RobotPoint-DstPoint
	void GetLineIFM_MR::CoordinateConvert(vector<float> &_IFM, float Delta_X, float Delta_Y, float Angel)
	{
		//更新一下LineCordinateConvert
		switch (LineCordinateConvert)
		{
		case Straight:Line_X_Y_Swap = 0;
			Line_X_Contrary = -1;
			Line_Y_Contrary = 1;
			break;
		case Counterclockwise90:Line_X_Y_Swap = 1;
			Line_X_Contrary = -1;
			Line_Y_Contrary = -1;
			break;
		case Clockwise90:Line_X_Y_Swap = 1;
			Line_X_Contrary = 1;
			Line_Y_Contrary = 1;
			break;
		case Counter:Line_X_Y_Swap = 0;
			Line_X_Contrary = 1;
			Line_Y_Contrary = -1;
			break;
		default:
			break;
		}

		if (Line_X_Y_Swap)//XY转换
		{
			_IFM[0] = Line_X_Contrary*Delta_Y;
			_IFM[1] = Line_Y_Contrary*Delta_X;
		}
		else
		{
			_IFM[0] = Line_X_Contrary*Delta_X;
			_IFM[1] = Line_Y_Contrary*Delta_Y;
		}
		_IFM[2] = Angel;
		return;
	}

	//CO: 0 关，1 开。  变量Camera_num 代表打开哪个相机
	void GetLineIFM_MR::CloseOrOpenCamera(OpenOrCloseCamera close_open, int Camera_num)
	{
		if (close_open == OpenOrCloseCamera::MV_CLOSE)
		{
			cap.release();
			//Camera0.CloseCamera();

			CameraIsOpen = false;
		}
		else
		{
			//Camera0.OpenCamera(Camera_num, false, 640, 480);

			CameraIsOpen = true;
			cap.open(Camera_num);
		}
		return;
	}

}