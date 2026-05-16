
#include "forecastchartwindow.h"
#include <QVBoxLayout>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QLabel>

ForecastChartWindow::ForecastChartWindow(QWidget *parent)
    : QDialog(parent), chartView(new QChartView), chart(new QChart), series(new QLineSeries)
{
    setWindowTitle("График температуры");
    resize(800, 500);

    chart->legend()->hide();
    chart->addSeries(series);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

void ForecastChartWindow::setForecastData(const QList<QPair<QString, double>> &data, const QList<QString> &icons)
{
    if (data.isEmpty()) return;

    series->clear();
    QStringList categories;
    double minTemp = data.first().second;
    double maxTemp = data.first().second;

    for (int i = 0; i < data.size(); ++i) {
        double temp = data[i].second;
        series->append(i, temp);

        QString label = data[i].first;
        if (i < icons.size()) {
            label += " " + icons[i];
        }

        categories << label;

        if (temp < minTemp) minTemp = temp;
        if (temp > maxTemp) maxTemp = temp;
    }

    if (chart->axisX()) chart->removeAxis(chart->axisX());
    if (chart->axisY()) chart->removeAxis(chart->axisY());

    auto *axisX = new QCategoryAxis;
    axisX->setTitleText("Дата и время");
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->setLabelsAngle(-45);

    for (int i = 0; i < categories.size(); ++i) {
        axisX->append(categories[i], i);
    }
    axisX->setRange(0, categories.size() - 1);

    auto *axisY = new QValueAxis;
    axisY->setLabelFormat("%.1f " "C");
    axisY->setTitleText("Температура");
    axisY->setRange(minTemp - 2, maxTemp + 2);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

ForecastChartWindow::~ForecastChartWindow()
{
    delete series;
    delete chart;
    delete chartView;
}
