#include "SimpleRender.h"

SimpleRender::SimpleRender()
{

}

void SimpleRender::initializeGL()
{
    QSurfaceFormat setup;
    setup.setVersion(3,3);
    setup.setProfile(QSurfaceFormat::CoreProfile);

    context()->setFormat(setup);

    if (!initializeOpenGLFunctions())
    {
        qDebug() << "OpenGL functions init fail";
    }

    makeCurrent();
}

void SimpleRender::resizeGL(int w, int h)
{
    ;
}

void SimpleRender::paintGL()
{
    context()->functions()->glClearColor(0.2f,0.3f, 0.3f, 1.0f);
    context()->functions()->glClear(GL_COLOR_BUFFER_BIT);
}
