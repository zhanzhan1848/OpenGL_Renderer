#include "Camera.h"


Camera::Camera(float x, float y, float z)
{
    w = 4;
    h = 3;
    init(x,y,z);
}

QMatrix4x4 Camera::getMatrix()
{
    if (MODE == MODE_MOVE || MODE == MODE_ZOOM)
            Target = Position + Forward;
    Target = QVector3D(0,0,0);
    transform.setToIdentity();
    transform.lookAt(Position,Target,WorldUp);
    return project*transform;
}

void Camera::updateCamPos()
{
    if (MODE != MODE_ROTATE)
        Position += Forward * speedZ / (float)20.0;

    Position -= Right * speedX / (float)40.0;
    Position -= Up * speedY / (float)40.0;
    float dis = Position.distanceToPoint(Target);
    if (MODE == MODE_ROTATE)
        Position += Forward * (dis - distance);

    setStop();
    //MODE = MODE_MOVE;
}

void Camera::setStop()
{
    speedX = 0;
    speedY = 0;
    speedZ = 0;
}

void Camera::updateCamVectors()
{
    if (MODE == MODE_ROTATE)
        Forward = Target - Position;

    Forward.normalize();
    Right = QVector3D::crossProduct(Forward,WorldUp).normalized();
    Up = QVector3D::crossProduct(Forward,Right).normalized();
}

void Camera::computeDistance()
{
    distance = Position.distanceToPoint(Target);
}

void Camera::updateProject(int width, int height)
{
    w = width;
    h = height;
    project.setToIdentity();
    project.perspective(60.0f, float(w)/float(h), 0.01f, 100.0f);
}

void Camera::init(float x, float y, float z)
{
    Position = QVector3D(x,y,z);
    WorldUp = QVector3D(0,1,0);

    Forward = -Position.normalized();
    Right = QVector3D::crossProduct(Forward,WorldUp).normalized();
    Up = QVector3D::crossProduct(Forward,Right).normalized();

    Target = Position + Forward;
    //Target = QVector3D(0,0,0);
    transform.setToIdentity();
    updateProject(w,h);
    MODE = MODE_MOVE;
}

Camera::~Camera()
{
    qDebug() << "camera delete" ;
}
