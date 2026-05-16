
#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WeatherAPI : public QObject
{
    Q_OBJECT
public:
    explicit WeatherAPI(QObject *parent = nullptr);
    void fetchThreeHourForecast(double lat, double lon);
    QUrl buildForecastUrl(double lat, double lon);
    QString parseThreeHourForecast(const QByteArray &response);
    QString weatherEmoji(const QString &description);
signals:
    void weatherDataReceived(const QString &data);

private slots:
    void onWeatherReply(QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    QString apiKey = "c4b7042e910d2339421b9d56d563c6cd";  // Вставьте сюда свой ключ OpenWeatherMap


};

#endif