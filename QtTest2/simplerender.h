#ifndef SIMPLERENDER_H
#define SIMPLERENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QFile>

class SimpleRender : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    SimpleRender(QWidget *parent=nullptr);

protected:
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void initializeGL() override;

private:
    QOpenGLBuffer *VBO, *EBO;
    QOpenGLVertexArrayObject *VAO;
    QOpenGLShaderProgram* shaderProgram;
};

#endif // SIMPLERENDER_H
