#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotwidget = new QWidget();
    chart = new QChart();
    chartView = new QChartView(chart,this);
    layout = new QHBoxLayout();

    button1 = new QPushButton("相位噪声",this);
    button2 = new QPushButton("阿伦方差",this);
    toolbar = new QToolBar(this);//工具栏
    addToolBar(Qt::RightToolBarArea,toolbar);//在右侧区域添加工具栏
    toolbar->addWidget(button1);//工具栏添加按钮
    toolbar->addWidget(button2);
    toolbar->addSeparator();//工具栏添加分割线

    centralwidget = new QWidget();//中心部件
    setCentralWidget(centralwidget);

    series1 = new QLineSeries();//曲线数据
    chart->addSeries(series1);//添加曲线

    //X轴坐标初始化
    axisX = new QLogValueAxis();
    axisY = new QValueAxis();
    axisYlog = new QLogValueAxis();//极坐标轴
    axisX->setLabelFormat("%g");
    axisX->setRange(1, 100000);
    axisX->setBase(10);
    axisX->setMinorTickCount(10);//次要刻度线的数量

    //Y轴坐标初始化
    axisY->setRange(-195, -100);//Y轴坐标范围
    axisY->setLabelFormat("%d");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(11);//(3);
    axisY->setMinorTickCount(0);//(4);

    //添加坐标轴
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);

    chart->legend()->setVisible(false);//图例不可见
    chart->setTheme(chart->ChartThemeDark);//黑色背景
    chart->setAnimationOptions(QChart::SeriesAnimations);//曲线动画
    layout->addWidget(chartView);
    //chartView->setRenderHint(QPainter::Antialiasing);

    //连接函数（信号发送者，发送的信号（函数地址），信号接收者，处理的槽函数（函数地址））
    connect(button1,&QPushButton::clicked,this,&MainWindow::on_button1_clicked);
    connect(button2,&QPushButton::clicked,this,&MainWindow::on_button2_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button1_clicked()
{
    chart->setTitle("Phase Noise L(f) in dBc/Hz");
    chart->removeAxis(axisX);//移除坐标轴
    if(logflag)
        chart->removeAxis(axisYlog);
    else
        chart->removeAxis(axisY);
    logflag = 0;

    //找XY轴最大最小值
    datalength = sizeof(f_0) / sizeof(f_0[0]);
    xmin = f_0[0];
    xmax = f_0[datalength-2];
    float yvalue[datalength/2];
    for (i=1;i<datalength;i=i+2 )
    {
        yvalue[(i-1)/2]=f_0[i];
        //将数据y轴值另存至yvalue
    }
    ymax = *std::max_element(yvalue,yvalue + datalength/2);
    ymin = *std::min_element(yvalue,yvalue + datalength/2);

    //更新数据
    QVector<QPointF> data0;
    data0.clear();
    for (i=0;i<datalength/2;i++ )
    {
        data0.append(QPointF(f_0[2*i], f_0[2*i+1]));
        //添加曲线数据append（x轴坐标，y轴坐标）
    }

    //X轴坐标
    axisX->setLabelFormat("%g");
    axisX->setRange(xmin, xmax);//X轴坐标范围
    axisX->setBase(10);
    axisX->setMinorTickCount(10);//次要刻度线的数量
    axisX->setTitleText("Offset Frequency");
    //Y轴坐标
    axisY->setRange(ymin, ymax);//Y轴坐标范围
    axisY->setLabelFormat("%d");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(0);
    axisY->setTitleText("PSD dBc/Hz");

    series1->replace(data0);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
}

void MainWindow::on_button2_clicked()
{
    chart->setTitle("阿伦方差");
    chart->removeAxis(axisX);//移除坐标轴
    if(logflag)
        chart->removeAxis(axisYlog);
    else
        chart->removeAxis(axisY);
    logflag = 1;

    //找XY轴最大最小值
    datalength = sizeof(allan_dev) / sizeof(allan_dev[0]);
    xmin = allan_dev[0];
    xmax = allan_dev[datalength-2];
    float yvalue[datalength/2];
    for (i=1;i<datalength;i=i+2 )
    {
        yvalue[(i-1)/2]=allan_dev[i];
        //将数据y轴值另存至yvalue
    }
    ymax = *std::max_element(yvalue,yvalue + datalength/2);
    ymin = *std::min_element(yvalue,yvalue + datalength/2);

    //更新数据
    QVector<QPointF> data1;
    data1.clear();
    for (i = 0; i < 5; i++) {
        data1.append(QPointF(allan_dev[2*i] , allan_dev[2*i+1]));
    }

    //X轴坐标
    axisX->setLabelFormat("%g");
    axisX->setRange(xmin, xmax);//X轴坐标范围
    axisX->setBase(10);
    axisX->setMinorTickCount(10);
    axisX->setTitleText("取样平均时间");
    //Y轴坐标
    axisYlog->setLabelFormat("%g");
    axisYlog->setBase(10);
    axisYlog->setRange(ymin, ymax);//Y轴坐标范围
    axisYlog->setMinorTickCount(10);
    axisYlog->setTitleText("阿伦方差");

    series1->replace(data1);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisYlog, Qt::AlignLeft);
    series1->attachAxis(axisX);
    series1->attachAxis(axisYlog);
}
