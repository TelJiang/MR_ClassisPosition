#pragma once

#ifndef _CLASSIS_MR_H
#define _CLASSIS_MR_H
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "CameraDS.h"
#include "visiontools.h"
namespace mv
{
	class GetLineIFM_MR
	{
		//这种方式不稳定的话，且增加rng次数严重影响程序速度的话
		//可以考略，两个方向上 仅找一根线的方式，结合二值图去处理
	public:
		GetLineIFM_MR();
		~GetLineIFM_MR();
		vector<float> IFM();
		void calculateMiddleLine(const std::vector<cv::Point2f> &points_left, const std::vector<cv::Point2f> &points_right, int _mode);
		void solvePointsY(const std::vector<cv::Point2f> &points_y, cv::Mat& for_show, cv::Mat& _Bin);
		void solvePointsX(const std::vector<cv::Point2f> &points_x, cv::Mat& for_show, cv::Mat& _Bin);
		void CloseOrOpenCamera(OpenOrCloseCamera close_open, int Camera_num);
		void CoordinateConvert(vector<float> &_IFM, float Delta_X, float Delta_Y, float Angel);
		void GetMode(bool _mode);

		enum LINE_COORDINATE_CONVERT
		{
			Straight = 1,
			Counterclockwise90 = 2,
			Clockwise90 = 3,
			Counter = 4
		};

		enum CLASSIS_MODE
		{
			Classis_Line = 1,
			Classis_Junction = 2
		};

	private:
		//CCameraDS Camera0;
		cv::Mat LastSrc;
		cv::VideoCapture cap;
		cv::Point2f CenterDst;
		cv::Point2f CenterRobot;
		int FactorRange;
		int Factor30MM;
		bool Flag1;
		bool Flag2;
		bool CameraIsOpen = false;
		float Line1K;
		float Line2K;
		float Line1B;
		float Line2B;
		bool LeftRightOutArea;
		bool UpDownOutArea;
		bool shuxianFlag;//专门处理竖线的flag
		double DSP;
		//LINE_COORDINATE_CONVERT
		bool Line_X_Y_Swap;
		char Line_X_Contrary;
		char Line_Y_Contrary;
		LINE_COORDINATE_CONVERT LineCordinateConvert;
		CLASSIS_MODE classis_mode = Classis_Line;

		cv::Mat map1;
		cv::Mat map2;
		cv::RNG line_rng;
		float lastX;
		float lastY;
		/*string outputVideoPath = "line.avi";
		cv::Size videoResolution = cv::Size(640, 480);
		cv::VideoWriter outputVideo;*/
	};
}

#endif //_VISION_H