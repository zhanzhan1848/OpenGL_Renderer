#include "Skybox.h"
#include <string>
#include <SOIL.h>
#include <stb_image.h>

SkyboxPtr Skybox::create()
{
    SkyboxPtr s{new Skybox()};
    return s;
}

Skybox::Skybox()
{
    initializeOpenGLFunctions();

    //init shader
    std::string path = "../OpenGL_Renderer/shader/";
    std::string vert = path + "skybox.vert";
    std::string frag = path + "skybox.frag";
    shader = Shader::Create(vert,frag);
    //gen buffers
    VAO.create();
    VBO.create();

    //allocate buffers
    static float data[] = {
    -100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,

    -100.0f, -100.0f,  100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,

    -100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,

     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,

    -100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f, -100.0f,

    -100.0f,  100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f, -100.0f
    };

    //bind buffer
    shader->bind();
    VAO.bind();
    VBO.bind();

    //allocate VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    //data to shader
    int stride = 3*sizeof(GLfloat);
    shader->enableAttributeArray(shader->posAttr);
    shader->setAttributeBuffer(shader->posAttr,GL_FLOAT,0,3,stride);
    VAO.release();
    VBO.release();
    shader->release();

    //texpaths
    QString p = "../data/Skybox/A/";
    texPath.append(p +"right.jpg" );
    texPath.append(p +"left.jpg" );
    texPath.append(p +"top.jpg" );
    texPath.append(p +"bottom.jpg" );
    texPath.append(p +"front.jpg" );
    texPath.append(p +"back.jpg" );

    initCubeTexture();
}

Skybox::~Skybox()
{
    texPath.clear();
    shader->destroyed();
    //texture->destroy();
}

void Skybox::initCubeTexture()
{
    QImage posX = QImage(texPath[0]).convertToFormat(QImage::Format_RGB888); //Right，默认读取的纹理为32位RGB，不符合CubeMap的要求，必须转为24位RGB。
    QImage negX = QImage(texPath[1]).convertToFormat(QImage::Format_RGB888); //Left
    QImage posY = QImage(texPath[2]).convertToFormat(QImage::Format_RGB888); //Top
    QImage negY = QImage(texPath[3]).convertToFormat(QImage::Format_RGB888); //Bottom
    QImage posZ = QImage(texPath[4]).convertToFormat(QImage::Format_RGB888); //Front
    QImage negZ = QImage(texPath[5]).convertToFormat(QImage::Format_RGB888); //Back

    //注释变量类型，QOpenGLTexture *texture;
    texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    texture->create();
    texture->setSize(posX.width(), posX.height(), posX.depth()); //这个我猜测 是确定一面纹理的尺寸，然后allocate分配函数，根据TargeCubeMap，分配六面纹理的空间
    texture->setFormat(QOpenGLTexture::RGBFormat);  //Vries设置的就是GL_RGB,这里同步
    texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8); //分配内存 ，UInt8等价于GL_UNSIGNED_BYTE

    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posX.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posY.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posZ.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negX.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negY.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negZ.bits());

    texture->setMinificationFilter(QOpenGLTexture::Linear);     //纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);   //设置纹理边缘的扩展方法
}

void Skybox::draw(QMatrix4x4 matrix)
{
    shader->bind();
    shader->setViewMatrix(matrix);
    shader->setUniformValue("skybox", 0);
    texture->bind(0);

    // Draw mesh
    VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    VAO.release();

    texture->release();
    shader->release();
}

SkydomePtr Skydome::create()
{
    SkydomePtr dm{new Skydome()};
    return dm;
}

Skydome::Skydome()
{
    std::string dm = "../data/skydome.obj";
    dome = model::create(dm);
    mesh = dome->data[0];
    mesh->transform.scale(1,-1,1);
    initMaterial();
}

Skydome::~Skydome()
{
    //texture->destroy();
    //delete texture;
    glDeleteTextures(1,&textureID);
}

void Skydome::setImage(std::string &texPath)
{
//    if(texture->isCreated())
//        texture->destroy();
    path = texPath;
//    QImage hdri = QImage(path.c_str()).convertToFormat(QImage::Format_RGB888);
//    texture = new QOpenGLTexture(hdri.mirrored());
//    texture->create();
//    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    //stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0 + textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    qDebug()<<sizeof (data)<<"."<<width<<","<<height<<","<<nrComponents;
}

void Skydome::initMaterial()
{
    mesh->shader->destroyed();
    std::string path = "../OpenGL_Renderer/shader/";
    std::string vert = path + "skydome.vert";
    std::string frag = path + "skydome.frag";
    mesh->shader = Shader::Create(vert,frag);
    mesh->setup();
}

void Skydome::draw(QMatrix4x4 matrix)
{
    //setup shader
    mesh->shader->bind();
    mesh->shader->setViewMatrix(matrix);
    mesh->shader->setUniformValue(mesh->shader->transMatrix, mesh->transform);
    glBindTexture(GL_TEXTURE_2D, textureID);
    mesh->shader->setUniformValue("skydome", textureID);
    //texture->bind(0);

    // Draw mesh
    mesh->VAO.bind();

    glDrawArrays(GL_TRIANGLES, 0, int(mesh->posElements.size()));

    //release
    mesh->VAO.release();
    //texture->release();
    mesh->shader->release();
}
