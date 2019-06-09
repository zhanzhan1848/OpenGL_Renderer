#pragma once

#include <QMainWindow>
#include <OpenGLScene.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    OpenGLScene *glScene;
    QWidget * glWidget;
private:
    void initUI();
};

