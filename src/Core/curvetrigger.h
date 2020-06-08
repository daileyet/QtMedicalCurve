#ifndef CURVETRIGGER_H
#define CURVETRIGGER_H

#include <QtCore/QObject>
#include <QtCore/QList>

class CurveTrigger : public QObject
{
      Q_OBJECT
   public:
      explicit CurveTrigger(QObject* parent = nullptr);
      ~CurveTrigger();
   signals:
      void triggerCurveUpdate();
   public slots:
      void start(int milsecond);
      void stop();
   protected:
      virtual void timerEvent(QTimerEvent* event);
   private:
      int m_timer;
};


#endif // CURVETRIGGER_H
