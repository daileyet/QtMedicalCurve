#ifndef DEFINE_H
#define DEFINE_H

#include <QtCore/QObject>
#include <QtCore/QPair>
#include <time.h>

#ifndef DECLARE_PROPERTY
#define DECLARE_PROPERTY(type, name, Name) \
    Q_PROPERTY(type name READ name WRITE set##Name NOTIFY name##Changed) \
    public: \
    Q_INVOKABLE type name() const {return m_##name;} \
    Q_INVOKABLE void set##Name(type name) {if(m_##name == name) return; m_##name = name; emit name##Changed();} \
    private: \
    type m_##name;
#endif

#ifndef DECLARE_PROPERTY_READONLY
#define DECLARE_PROPERTY_READONLY(type, name, Name) \
    Q_PROPERTY(type name READ name NOTIFY name##Changed) \
    public: \
    Q_INVOKABLE type name() const {return m_##name;} \
    private: \
    void set##Name(type name) {if(m_##name == name) return; m_##name = name; emit name##Changed();} \
    type m_##name;
#endif

// curve data
typedef QPair<timespec, qreal> CurveData;
typedef QPair<int,int> CurveScalePair;
#endif // DEFINE_H
