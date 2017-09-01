#include <QGuiApplication>
#include <QFontDatabase>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QUrl>
#include <QQmlComponent>
#include <QQmlEngine>

class ElapsedTimeDebug
{
public:
    ElapsedTimeDebug(const char *functionName)
        : m_functionName(functionName),
          m_pingCount(0),
          m_lastPing(0),
          m_max(0),
          m_min(0)
    {
        m_timer.start();
    }

    void ping()
    {
        quint64 pingTime = m_timer.elapsed();
        m_max = qMax(m_max, pingTime - m_lastPing);
        m_min = qMin(m_min, pingTime - m_lastPing);
        m_lastPing = pingTime;
        m_pingCount++;
    }

    ~ElapsedTimeDebug()
    {
        quint64 elapsed = m_timer.elapsed();
        qDebug() << m_functionName;
        qDebug() << "\tElapsed" << elapsed << "ms";
        qDebug() << "\tMin:" << m_min << "Max" << m_max << "Average:" << ((double) elapsed)/m_pingCount;
    }
private:
    QString m_functionName;
    QElapsedTimer m_timer;
    uint m_pingCount;
    quint64 m_lastPing;
    quint64 m_max;
    quint64 m_min;
};

void queryFontFromNativeTheme(int times)
{
    ElapsedTimeDebug eTime(__PRETTY_FUNCTION__);
    for(auto i = 0; i < times; i++) {
        QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        eTime.ping();
    }
}

void queryFontAfterImportControls(int times)
{
    QQmlEngine *engine = new QQmlEngine;
    QQmlComponent *qml = new QQmlComponent(engine);
    qml->setData("import QtQuick.Controls 2.2; Button {}", QUrl::fromLocalFile("/tmp/test.qml"));
    auto item = qml->create();
    ElapsedTimeDebug eTime(__PRETTY_FUNCTION__);
    for(auto i = 0; i < times; i++) {
        QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        eTime.ping();
    }
}

void start(int times)
{
    queryFontFromNativeTheme(times);
    queryFontAfterImportControls(times);
    QGuiApplication::instance()->quit();
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    if (app.arguments().size() != 2) {
        qWarning() << "Usage: <app> <num-interactions>";
        return 0;
    }
    const int times = app.arguments().at(1).toInt();
    QTimer::singleShot(100, [times]() {
        start(times);
    });
    return app.exec();
}
