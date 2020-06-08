#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Core/graph/curveplot.h"
#include "Core/graph/curvetypes.h"
#include "Core/curvetrigger.h"
#include "Core/curvesimulator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    CurveTrigger curveTrigger;
    CurveSimulator curveSimulator;

    qmlRegisterType<CurvePlot>("com.openthinks.qt",1,0,"CurvePlot");
    qmlRegisterUncreatableType<CurveTypes>("com.openthinks.qt",1,0,"CurveTypes","");

    engine.rootContext()->setContextProperty("curveSimulator",&curveSimulator);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    curveTrigger.start(33);

    return app.exec();
}
