#include <QtTest>
#include "../weatherapi.h"

class TestInvalidJson : public QObject
{
    Q_OBJECT

private slots:
    void invalidJsonHandling();
};

void TestInvalidJson::invalidJsonHandling()
{
    WeatherAPI api;

    QByteArray invalidJson = "INVALID_JSON_DATA";

    QString result = api.parseThreeHourForecast(invalidJson);

    QCOMPARE(result, QString("Ошибка парсинга прогноза"));
}

QTEST_MAIN(TestInvalidJson)

#include "test_invalidjson.moc"