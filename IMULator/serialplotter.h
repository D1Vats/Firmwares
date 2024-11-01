#ifndef SERIALPLOTTER_H
#define SERIALPLOTTER_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QTimer>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QLegend>
#include <QPushButton>
#include <QLineEdit>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QByteArray>


namespace Ui {
class SerialPlotter;
}

class SerialPlotter : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPlotter(QWidget *parent = nullptr);
    ~SerialPlotter();

    void updateData(float x_Value, float y_Value, float z_Value);
    bool newDataAvailable;

signals:
    void plotterDataReady(const QByteArray &data);

private slots:
    void handleMarkerClicked();
    void plotStopButtonClicked();
    void plotData();
    void sendData();

private:
    Ui::SerialPlotter *ui;
    QChartView *chartView = nullptr;
    QLineSeries *series_X, *series_Y, *series_Z;
    QChart *chart;
    QValueAxis *x_Axis;
    QValueAxis *y_Axis;
    QTimer *timer;
    qreal x;
    bool is_Running;
    float x_Value, y_Value, z_Value;
    QPushButton *plotStop_Button;
    QPushButton *dataSend_Button;
    QLineEdit *lineEdit;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // SERIALPLOTTER_H
