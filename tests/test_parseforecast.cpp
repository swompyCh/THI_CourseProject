#include <QtTest>
#include "../weatherapi.h"

class TestParseForecast : public QObject
{
    Q_OBJECT

private slots:
    void parseForecast();
};

void TestParseForecast::parseForecast()
{
    WeatherAPI api;

    QByteArray json = R"(
    {
        "list": [
            {
                "dt": 1710000000,
                "main": {
                    "temp": 18.4,
                    "feels_like": 16.9
                },
                "wind": {
                    "speed": 4.7
                },
                "weather": [
                    {
                        "description": "ясно"
                    }
                ]
            }
        ]
    }
    )";

    QString result = api.parseThreeHourForecast(json);

    QVERIFY(result.contains("18.4°C"));
    QVERIFY(result.contains("16.9°C"));
    QVERIFY(result.contains("4.7"));
    QVERIFY(result.contains("ясно"));
    QVERIFY(result.contains("☀️"));
}

QTEST_MAIN(TestParseForecast)

#include "test_parseforecast.moc"