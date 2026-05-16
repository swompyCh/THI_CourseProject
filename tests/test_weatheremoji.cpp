#include <QtTest>
#include "../weatherapi.h"

class TestWeatherEmoji : public QObject
{
    Q_OBJECT

private slots:
    void emojiMatching();
};

void TestWeatherEmoji::emojiMatching()
{
    WeatherAPI api;

    QCOMPARE(api.weatherEmoji("дождь"), QString("🌧️"));
    QCOMPARE(api.weatherEmoji("гроза"), QString("⛈️"));
    QCOMPARE(api.weatherEmoji("снег"), QString("❄️"));
    QCOMPARE(api.weatherEmoji("ясно"), QString("☀️"));
    QCOMPARE(api.weatherEmoji("туман"), QString("🌫️"));

    // Проверка значения по умолчанию
    QCOMPARE(api.weatherEmoji("аномальная погода"), QString("🌡️"));
}

QTEST_MAIN(TestWeatherEmoji)

#include "test_weatheremoji.moc"