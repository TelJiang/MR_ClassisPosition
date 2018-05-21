#include "lineserieschart.hpp"
#pragma execution_character_set("utf-8")
LineSeriesChart::LineSeriesChart(QWidget * parent) : QWidget(parent) {
	designUI();
}

LineSeriesChart::~LineSeriesChart() {
	
}

void LineSeriesChart::designUI()
{
	LineSeriesChart::resize(600, 400);
	LineSeriesChart::setWindowTitle("数据分析折线图");
	LineSeriesChart::setFont(QFont("微软雅黑", 12));

	myChart = new QChart();
	myChartView = new QChartView(myChart);
	myChartView->setRenderHint(QPainter::Antialiasing);
	xAxis = new QValueAxis;
	yAxis = new QValueAxis;

	ZeroLineSeries = new QLineSeries();//基准线
	myChart->addSeries(ZeroLineSeries);
	myChart->setAxisX(xAxis, ZeroLineSeries);
	myChart->setAxisY(yAxis, ZeroLineSeries);

	setChartTitle("数据分析折线图");
	setAxis(0, -100, 1000, 100);
	setChartLegend();

	ZeroLineSeries->setUseOpenGL(true);
	ZeroLineSeries->setName("基准线");
	ZeroLineSeries->append(QPointF(0, 0));
	ZeroLineSeries->append(QPointF(10000, 0));
	QPen zeroLinePen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	ZeroLineSeries->setPen(zeroLinePen);

	recoverChart_PushButton = new QPushButton("recover chart", this);
	clearChart_PushButton = new QPushButton("clear chart", this);

	gridLayout = new QGridLayout(this);

	gridLayout->addWidget(myChartView, 0, 0, 1, 2);
	gridLayout->addWidget(recoverChart_PushButton, 1, 0, 1, 1);
	gridLayout->addWidget(clearChart_PushButton, 1, 1, 1, 1);
	LineSeriesChart::setLayout(gridLayout);
}

void LineSeriesChart::setAxis(int xAxisMin, int yAxisMin, int xAxisMax, int yAxisMax)
{
	xAxis->setRange(xAxisMin, xAxisMax);
	yAxis->setRange(yAxisMin, yAxisMax);
	xAxis->setLabelFormat("%d");
	yAxis->setLabelFormat("%d");
	xAxis->setTickCount(11);
	yAxis->setTickCount(6);
}

void LineSeriesChart::setChartTitle(QString title)
{
	myChart->setTitle(title);
	myChart->setTitleFont(QFont("微软雅黑", 12));
}

void LineSeriesChart::setChartLegend()
{
	myChart->legend()->setAlignment(Qt::AlignLeft);
	myChart->legend()->show();
}

void LineSeriesChart::setXAxisMin(int min)
{
	xAxis->setMin(min);
}

void LineSeriesChart::setXAxisMax(int max)
{
	xAxis->setMax(max);
}

void LineSeriesChart::setYAxisMin(int min)
{
	yAxis->setMin(min);
}

void LineSeriesChart::setYAxisMax(int max)
{
	yAxis->setMax(max);
}

void LineSeriesChart::clearChart()
{
}

void LineSeriesChart::closeEvent(QCloseEvent * event)
{
	emit emit_close();
}
