#pragma once
#include <QMatrix4x4>
#include <QVector3D>
#include <memory>

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera
{
public:
    static CameraPtr create(float x=0,float y=0,float z=0);
    Camera(float x=0,float y=0,float z=0);
    ~Camera();
    QMatrix4x4 transform;
    QMatrix4x4 project;
    QMatrix4x4 getMatrix();
    QMatrix4x4 getSkyMatrix();

    QVector3D Position;
    QVector3D Forward;
    QVector3D Right;
    QVector3D Up;
    QVector3D WorldUp;

    QVector3D Target;

    const int MODE_MOVE = 0;
    const int MODE_ROTATE = 1;
    const int MODE_ZOOM = 2;
    int MODE;

    float globalSpeed = 1 / 15;
    float speedX = 0;
    float speedY = 0;
    float speedZ = 0;

    float distance;
    int w;
    int h;

    void updateCamPos();
    void setStop();
    void updateCamVectors();
    void computeDistance();
    void updateProject(int width,int height);
    void init(float x,float y,float z);
};
