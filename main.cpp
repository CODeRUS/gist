#include <QCoreApplication>
#include <QTimer>

#include "gist.h"

int main(int argc, char *argv[])
{
    QGuiApplication *app = new QGuiApplication(argc, argv);

    Gist *gist = new Gist(app);
    QTimer::singleShot(1, gist, SLOT(start()));

    return app->exec();
}
