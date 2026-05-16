
#include "location.h"

Location::Location(QObject *parent) : QObject(parent) {
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        connect(source, &QGeoPositionInfoSource::positionUpdated, this, &Location::positionUpdated);
        connect(source, &QGeoPositionInfoSource::errorOccurred, this, &Location::positionError);
        source->setUpdateInterval(2000);
    }
}

void Location::requestLocation() {
    if (source)
        source->requestUpdate();
    else
        emit locationError("Источник геопозиции не найден");
}

void Location::positionUpdated(const QGeoPositionInfo &info) {
    QGeoCoordinate coord = info.coordinate();
    emit locationUpdated(coord.latitude(), coord.longitude());
}

void Location::positionError(QGeoPositionInfoSource::Error error) {
    Q_UNUSED(error);
    emit locationError("Ошибка при получении геопозиции");
}
