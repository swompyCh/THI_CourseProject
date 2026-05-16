
#ifndef FORECASTCHARTWINDOW_H
#define FORECASTCHARTWINDOW_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class ForecastChartWindow; }
QT_END_NAMESPACE



class ForecastChartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ForecastChartWindow(QWidget *parent = nullptr);
    void setForecastData(const QList<QPair<QString, double>> &data, const QList<QString> &icons);
    ~ForecastChartWindow();

private:
    QChartView *chartView;
    QChart *chart;
    QLineSeries *series;
};

#endif
