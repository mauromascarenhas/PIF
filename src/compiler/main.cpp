#include <QtCore>
#include <QCoreApplication>

#include "pifcompiler.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    QCoreApplication a(argc, argv);

    PIFCompiler *compiler = new PIFCompiler();
    QTimer::singleShot(0, compiler, SLOT(execute()));

    return a.exec();
}
