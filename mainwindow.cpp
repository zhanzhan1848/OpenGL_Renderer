#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <OpenGLScene.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
    glScene = new OpenGLScene();
    glWidget = createWindowContainer(glScene);
    setCentralWidget(glWidget);
    glScene->setAnimating(true);


}
