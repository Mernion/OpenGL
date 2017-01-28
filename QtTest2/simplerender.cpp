#include "SimpleRender.h"

SimpleRender::SimpleRender(QWidget *parent) : QOpenGLWidget(parent)
{
    //makeCurrent();
}

void SimpleRender::initializeGL()
{
    QSurfaceFormat setup;
    setup.setVersion(3,3);
    setup.setProfile(QSurfaceFormat::CoreProfile);

    context()->setFormat(setup);
    context()->create();
    makeCurrent();

    if(!initializeOpenGLFunctions())
    {
        qDebug() <<"failed";
    }

    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    QFile vertexFile(":/shaders/vertexshader.txt");
    if (!vertexFile.exists())
    {
        qDebug() << "Vertex shader file not found";
    }
    if (!vertexShader.compileSourceFile(vertexFile.fileName()))
    {
        qDebug() << vertexShader.log();
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    QFile fragmentFile(":/shaders/fragmentshader.frag");
    if (!fragmentFile.exists())
    {
        qDebug() << "Fragment shader file not found";
    }
    if (!fragmentShader.compileSourceFile(fragmentFile.fileName()))
    {
        qDebug() << fragmentShader.log();
    }

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShader(&vertexShader);
    shaderProgram->addShader(&fragmentShader);
    if (!shaderProgram->link())
    {
        qDebug() << shaderProgram->log();
    }

    QVector<GLfloat> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    qDebug() << "size of vertices = " << sizeof(vertices);

    VAO = new QOpenGLVertexArrayObject();
    VAO->create();

    VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    VBO->create();
    VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);

    VAO->bind();

    VBO->bind();
    VBO->allocate(vertices.size() * sizeof(vertices));
    VBO->write(0, &vertices, vertices.size() * sizeof(vertices));
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    VAO->release();

}

void SimpleRender::resizeGL(int w, int h)
{
    ;
}

void SimpleRender::paintGL()
{
    QOpenGLFunctions_3_3_Core *f = context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    f->glClearColor(0.2f,0.3f, 0.3f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram->bind();
    VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    VAO->release();
    shaderProgram->release();

}
