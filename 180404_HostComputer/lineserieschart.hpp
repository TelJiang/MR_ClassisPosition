#pragma once
#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QPen>
#include <QPushButton>
#include <QGridLayout>
#include <QCloseEvent>
QT_CHARTS_USE_NAMESPACE
class LineSeriesChart : public QWidget {
	Q_OBJECT

public:
	LineSeriesChart(QWidget * parent = Q_NULLPTR);
	~LineSeriesChart();
	void designUI();
	void setAxis(int xAxisMin, int yAxisMin, int xAxisMax, int yAxisMax);
	void setChartTitle(QString title);
	void setChartLegend();
	void setXAxisMin(int min);
	void setXAxisMax(int max);
	void setYAxisMin(int min);
	void setYAxisMax(int max);
	void clearChart();
	void closeEvent(QCloseEvent *event);

signals:
	void emit_close();
private:
	static const int lineNum;
	//QLineSeries *myLineSeries[lineNum];
	QChart *myChart;
	QChartView *myChartView;
	QValueAxis *xAxis, *yAxis;
	QList<QLineSeries*> myLineSeries;
	QLineSeries *ZeroLineSeries;
	QGridLayout *chartGridLayout;
	QGridLayout *gridLayout;
	QPushButton *clearChart_PushButton;
	QPushButton *recoverChart_PushButton;
};
