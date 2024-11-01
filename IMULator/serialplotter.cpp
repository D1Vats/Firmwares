#include "serialplotter.h"
#include "ui_serialplotter.h"
#include <QDebug>
#include <QVBoxLayout>


SerialPlotter::SerialPlotter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialPlotter)
    ,x(0)
    ,is_Running(false)
{
    ui->setupUi(this);
    setWindowTitle("Serial Plotter");
    resize(800, 480);

    QFont labelFont("Comic Sans Ms", 10, QFont::Bold);
    QFont titleFont("Comic Sans Ms", 10, QFont::Bold);
    QFont chartTitleFont("Comic Sans Ms", 12, QFont::Bold);

    // Setup the series and the chart
    series_X = new QLineSeries();
    series_X->setName("X-Axis");
    series_X->setUseOpenGL(true);
    series_Y = new QLineSeries();
    series_Y->setName("Y-Axis");
    series_Y->setUseOpenGL(true);
    series_Z = new QLineSeries();
    series_Z->setName("Z-Axis");
    series_Z->setUseOpenGL(true);

    chart = new QChart();
    chart->addSeries(series_X);
    chart->addSeries(series_Y);
    chart->addSeries(series_Z);
    chart->setTitle("Real Time Plotting");
    chart->setTitleBrush(QBrush(QColor(Qt::darkCyan)));
    chart->setTitleFont(chartTitleFont);
    chart->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setContentsMargins(0, 0, 0, 0);

    QLegend* legend = chart->legend();
    QFont legendFont("Comic Sans Ms", 9, QFont::Bold);
    legend->setFont(legendFont);

    //Setup the axes
    x_Axis = new QValueAxis();
    //x_Axis->setRange(0, 100);
    x_Axis->setLabelFormat("%.1f");
    x_Axis->setTitleText("Time(s)");
    x_Axis->setLabelsColor(QColor(Qt::darkCyan));
    x_Axis->setTitleBrush(QBrush(QColor(Qt::darkCyan)));
    x_Axis->setGridLinePen(QPen(QColor(230, 230, 230, 50)));
    x_Axis->setLabelsFont(labelFont);
    x_Axis->setTitleFont(titleFont);
    chart->addAxis(x_Axis, Qt::AlignBottom);
    series_X->attachAxis(x_Axis);
    series_Y->attachAxis(x_Axis);
    series_Z->attachAxis(x_Axis);

    y_Axis = new QValueAxis();
    //y_Axis->setRange(-1000, 1000);
    y_Axis->setLabelFormat("%.1f");
    y_Axis->setTitleText("Value");
    y_Axis->setLabelsColor(QColor(Qt::darkCyan));
    y_Axis->setTitleBrush(QBrush(QColor(Qt::darkCyan)));
    y_Axis->setGridLinePen(QPen(QColor(230, 230, 230, 50)));
    y_Axis->setLabelsFont(labelFont);
    y_Axis->setTitleFont(titleFont);
    chart->addAxis(y_Axis, Qt::AlignLeft);
    series_X->attachAxis(y_Axis);
    series_Y->attachAxis(y_Axis);
    series_Z->attachAxis(y_Axis);


    chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setFrameStyle(QFrame::NoFrame);
    chartView->setViewport(new QOpenGLWidget);

    QFont font("Comic Sans Ms", 10, QFont::Bold);

    plotStop_Button = new QPushButton("Stop");
    plotStop_Button->setFont(font);
    plotStop_Button->setStyleSheet("background-color:rgb(250, 20, 20)");
    dataSend_Button = new QPushButton("Send");
    dataSend_Button->setFont(font);
    lineEdit = new QLineEdit();
    lineEdit->setFont(font);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(dataSend_Button);
    hLayout->addWidget(plotStop_Button);

    QVBoxLayout *vLayout = new QVBoxLayout(this);

    vLayout->addWidget(chartView);
    vLayout->addLayout(hLayout);

    foreach (QLegendMarker* marker, chart->legend()->markers())
    {
        QObject::connect(marker, &QLegendMarker::clicked, this, &SerialPlotter::handleMarkerClicked);
    }

    connect(plotStop_Button, &QPushButton::clicked, this, &SerialPlotter::plotStopButtonClicked);
    connect(dataSend_Button, &QPushButton::clicked, this, &SerialPlotter::sendData);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SerialPlotter::plotData);
    timer->start(10);
    newDataAvailable = false;

}

SerialPlotter::~SerialPlotter()
{
    delete ui;
}

void SerialPlotter::updateData(float x_Value, float y_Value, float z_Value)
{
    this->x_Value = x_Value;
    this->y_Value = y_Value;
    this->z_Value = z_Value;
}

void SerialPlotter::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*>(sender());
    if (marker->type() == QLegendMarker::LegendMarkerTypeXY)
    {
        // Toggle the visibility of the corresponding series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Dim the marker if the series is not visible
        marker->setVisible(true);
        qreal alpha = marker->series()->isVisible() ? 1.0 : 0.5;
        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);
        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);
        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
    }
}

void SerialPlotter::plotStopButtonClicked()
{
    if(is_Running)
    {
        timer->start(10);
        plotStop_Button->setText("Stop");
        plotStop_Button->setStyleSheet("background-color:rgb(250, 20, 20)");
    }
    else
    {
        timer->stop();
        plotStop_Button->setText("Start");
        plotStop_Button->setStyleSheet("background-color:rgb(0, 139, 139)");
    }

    is_Running = !is_Running;
}

void SerialPlotter::plotData()
{
    if(newDataAvailable)
    {
        series_X->append(x, x_Value);
        if(series_X->count() > 100)
        {
            series_X->remove(0);
        }

        series_Y->append(x, y_Value);
        if(series_Y->count() > 100)
        {
            series_Y->remove(0);
        }

        series_Z->append(x, z_Value);
        if(series_Z->count() > 100)
        {
            series_Z->remove(0);
        }

        int maxValue = qMax(qMax(x_Value, y_Value), z_Value);
        int minValue = qMin(qMin(x_Value, y_Value), z_Value);

        x_Axis->setRange(qMax(0.0, x - 10), x);
        y_Axis->setRange(minValue-1, maxValue+1);

        x += 0.1;
        newDataAvailable = false;
    }
}

void SerialPlotter::sendData()
{
    emit plotterDataReady(lineEdit->text().toLocal8Bit());
    lineEdit->clear();
}


void SerialPlotter::wheelEvent(QWheelEvent *event)
{
    // Zoom factor
    qreal factor = 1.1;
    if (event->angleDelta().y() > 0) {
        chart->zoom(factor);  // Zoom in
    } else {
        chart->zoom(1.0 / factor);  // Zoom out
    }
}
