
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "forecastchartwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    weatherAPI(new WeatherAPI(this))
{
    ui->setupUi(this);

    setupCityCoordinates();

    // При выборе города сразу загружается прогноз погоды (3-часовой)
    connect(ui->cityComboBox, &QComboBox::currentTextChanged, this, [this](const QString &city) {
        if (cityCoordinates.contains(city)) {
            auto [lat, lon] = cityCoordinates[city];
            weatherAPI->fetchThreeHourForecast(lat, lon);  // Используем 3-часовой прогноз
        }
    });

    connect(weatherAPI, &WeatherAPI::weatherDataReceived, this, &MainWindow::onWeatherDataReceived);

    // По умолчанию выбираем Томск и загружаем его прогноз
    ui->cityComboBox->setCurrentText("Томск");
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveForecastToFile);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    int width = this->width();
    int height = this->height();

    // forecastText занимает почти всё
    ui->forecastText->setGeometry(200, 60, width - 210, height - 70);

    // updateButton
    ui->updateButton->move(200, 20);

    // saveButton
    ui->saveButton->move(width - 160, 20);

    // graphButton
    ui->graphButton->move(20, 100);

    // cityComboBox
    ui->cityComboBox->move(20, 50);

    // statusLabel
    ui->statusLabel->move(10, 10);
}

void MainWindow::setupCityCoordinates()
{
    cityCoordinates["Москва"] = {55.7558, 37.6173};
    cityCoordinates["Санкт-Петербург"] = {59.9343, 30.3351};
    cityCoordinates["Новосибирск"] = {55.0084, 82.9357};
    cityCoordinates["Екатеринбург"] = {56.8389, 60.6057};
    cityCoordinates["Казань"] = {55.7963, 49.1088};
    cityCoordinates["Нижний Новгород"] = {56.2965, 43.9361};
    cityCoordinates["Челябинск"] = {55.1644, 61.4368};
    cityCoordinates["Самара"] = {53.1959, 50.1008};
    cityCoordinates["Уфа"] = {54.7388, 55.9721};
    cityCoordinates["Ростов-на-Дону"] = {47.2357, 39.7015};
    cityCoordinates["Красноярск"] = {56.0153, 92.8932};
    cityCoordinates["Пермь"] = {58.0105, 56.2502};
    cityCoordinates["Воронеж"] = {51.6755, 39.2089};
    cityCoordinates["Волгоград"] = {48.7080, 44.5133};
    cityCoordinates["Томск"] = {56.4847, 84.9481};

    for (const QString &city : cityCoordinates.keys()) {
        ui->cityComboBox->addItem(city);
    }
}
void MainWindow::on_graphButton_clicked()
{
    if (forecastPoints.isEmpty()) {
        QMessageBox::information(this, "Нет данных", "Сначала загрузите прогноз.");
        return;
    }

    ForecastChartWindow *chartWindow = new ForecastChartWindow(this);
    chartWindow->setForecastData(forecastPoints, weatherIcons);
    chartWindow->exec();
}
void MainWindow::onUpdateButtonClicked()
{
    QString selectedCity = ui->cityComboBox->currentText();
    if (cityCoordinates.contains(selectedCity)) {
        auto [lat, lon] = cityCoordinates[selectedCity];
        weatherAPI->fetchThreeHourForecast(lat, lon);  // Обновляем 3-часовой прогноз
    }

    ui->statusLabel->setText("Прогноз обновлен");
    ui->statusLabel->setStyleSheet("color: green; font-weight: bold;");
    ui->statusLabel->setVisible(true);

    QTimer::singleShot(3000, this, [this]() {
        ui->statusLabel->setVisible(false);
    });
}


void MainWindow::saveForecastToFile()
{
    QString forecastText = ui->forecastText->toPlainText(); // предположим, прогноз отображается в QTextEdit
    QString selectedCity = ui->cityComboBox->currentText();     // или другой виджет с городами

    if (forecastText.isEmpty()) {
        QMessageBox::warning(this, "Сохранение", "Сначала получите прогноз перед сохранением.");
        return;
    }

    QFile file("forecast_history.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Город: " << selectedCity << "\n";
        out << forecastText << "\n";
        out << "----------------------------------------\n";
        file.close();

        QMessageBox::information(this, "Сохранение", "Прогноз успешно сохранён!");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
    }
}
void MainWindow::onWeatherDataReceived(const QString &data)
{
    ui->forecastText->setText(data);

    forecastPoints.clear();
    weatherIcons.clear();

    QRegularExpression rx(R"(^(.*)\s(\d{2}:\d{2}:\d{2}):\s([-+]?\d+(?:\.\d+)?)°C.*([^\s]{1,3}))");

    QStringList lines = data.split('\n', Qt::SkipEmptyParts);
    QSet<QString> usedDates;
    QString today = QDate::currentDate().toString("dddd").toLower();

    for (const QString &line : lines) {
        if (line.contains("°C")) {
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch()) {
                QString date = match.captured(1).trimmed().toLower();
                QString time = match.captured(2);
                QString label = date + " " + time;
                double temp = match.captured(3).toDouble();
                QString icon = match.captured(4);

                static QMap<QString, QString> dayAbbreviations = {
                    {"понедельник", "пн"},
                    {"вторник", "вт"},
                    {"среда", "ср"},
                    {"четверг", "чт"},
                    {"пятница", "пт"},
                    {"суббота", "сб"},
                    {"воскресенье", "вс"}
                };

                QString key = dayAbbreviations.value(date, date);  // заменяем на сокращение

                if (!usedDates.contains(key) || time.startsWith("13:00:00")) {
                    if (usedDates.contains(key)) {
                        forecastPoints.removeLast();
                        weatherIcons.removeLast();
                    }
                    if (date == today) {
                        forecastPoints.prepend({label, temp});
                        weatherIcons.prepend(icon);
                    } else {
                        forecastPoints.append({label, temp});
                        weatherIcons.append(icon);
                    }
                    usedDates.insert(key);
                }
            }
        }
    }
}
