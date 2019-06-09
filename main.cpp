#include "mainwindow.h"
#include <QApplication>
#include <OpenGLScene.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.glScene->setFormat(format);
    w.show();
    w.resize(640, 480);
    return a.exec();
}
