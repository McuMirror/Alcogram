#include <QApplication>
#include <QFontDatabase>
#include <QtGlobal>

#include "mainWindow.h"
#include "logger.h"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type) {
    case QtDebugMsg:
        Logger::instance()->log(msg);
        break;
    case QtInfoMsg:
        break;
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        break;
    case QtFatalMsg:
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageOutput);
    QApplication a(argc, argv);

    Logger::instance();

    MainWindow w;
    w.show();

    return a.exec();
}
