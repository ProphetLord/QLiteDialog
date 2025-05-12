#include <qapplication.h>

#include <qlogcollector.h>
#include <styledstring.h>
#include <qdebug.h>

#include "app.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget:focus{outline:none;}");

    /**
     * 初始化日志收集器
     */
    logcollector::styleConfig
        .consoleApp()
        .ide_clion(false)
        .wordWrap(120)
    ;

    logcollector::QLogCollector::instance().registerLog();

    /**
     * 打印应用版本等信息
     */
    qDebug() << styled("------application version:").g() << styled(APP_VERSION, true).g() << styled("------").g();

    App app;
    app.showMaximized();

    return a.exec();
}