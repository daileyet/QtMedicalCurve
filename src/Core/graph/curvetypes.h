#ifndef CURVETYPES_H
#define CURVETYPES_H

#include <QObject>

class CurveTypes : public QObject
{
    Q_OBJECT
    Q_ENUMS(MedicalType)
public:
    enum MedicalType
    {
        MEDICAL_PRESSURE, MEDICAL_FLOW, MEDICAL_VOLUME, MEDICAL_CO2
    };
    explicit CurveTypes(QObject *parent = nullptr);
    ~CurveTypes();
};

#endif // CURVETYPES_H
