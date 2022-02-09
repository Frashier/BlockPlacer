#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Get frame's dimensions
    Settings::frameWidth = ui->frame->width();
    Settings::frameHeight = ui->frame->height();

    // Pull info from ui to Settings class
    Settings::backgroundLightIntensity = (double) (ui->ambientLightingSlider->value())/100;
    Settings::lightIntensity = (double) (ui->lightIntensitySlider->value())/10;

    // Create new QImage object with frame's dimensions and RGB32 color format
    img = new QImage(Settings::frameWidth, Settings::frameHeight, QImage::Format_RGB32);

    // initialize the scene
    mainScene = new Scene(img);

    // Connecting signal from Scene object to MainWindow's update()
    QObject::connect(mainScene, &Scene::sceneRendered, this, [&](){update();});

    // Calling the first update
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    const int modifiedBy = event->angleDelta().y() < 0 ? 50 : -50;
    const int newValue = Settings::translationMatrix.values[2][3] + modifiedBy;

    // Make it so that it's not possible to zoom past certain threshold
    if (abs(newValue) > 200)
    {
        return;
    }

    Settings::translationMatrix.values[2][3] = newValue;
    mainScene->drawScene();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool rotationChanged = false;

    switch(event->key())
    {
        case Qt::Key_Q:
            rotationChanged = true;

            if (Settings::currentRotation - 1 < 0)
            {
                Settings::currentRotation = 3;
                break;
            }

            Settings::currentRotation -= 1;
            break;

        case Qt::Key_E:
            rotationChanged = true;

            Settings::currentRotation = (Settings::currentRotation + 1) % 4;
            break;

        case Qt::Key_W:
            Settings::translationMatrix.values[1][3] -= 10;
            break;

        case Qt::Key_S:
            Settings::translationMatrix.values[1][3] += 10;
            break;

        case Qt::Key_D:
            Settings::translationMatrix.values[0][3] += 10;
            break;

        case Qt::Key_A:
            Settings::translationMatrix.values[0][3] -= 10;
            break;
    }

    // If wall's rotation was changed
    // change label on wall radio button
    if (rotationChanged)
    {
        QString text;

        switch (Settings::currentRotation)
        {
            case 0:
                text = "Wall (S)";
                break;

            case 1:
                text = "Wall (W)";
                break;

            case 2:
                text = "Wall (N)";
                break;

            case 3:
                text = "Wall (E)";
                break;
        }

        ui->wallbutton->setText(text);
    }

    mainScene->drawScene();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Settings::deleteMode = false;

    Point2D pointPressed = Point2D(event->pos().x(), event->pos().y());

    // Check whether press was within frame's boundaries
    if (pointPressed.x < ui->frame->x() ||
            pointPressed.x > ui->frame->x() + Settings::frameWidth ||
            pointPressed.y < ui->frame->y() ||
            pointPressed.y > ui->frame->y() + Settings::frameHeight)
    {
        return;
    }

    // Modify coordinates so that pointPressed variable is in relation to frame dimensions,
    // not app window
    pointPressed.x -= ui->frame->x();
    pointPressed.y -= ui->frame->y();

    // If right mouse button was pressed
    // begin rotation
    if (event->buttons() == Qt::RightButton)
    {
        mousePress = pointPressed;
        return;
    }

    if (event->button() == Qt::MiddleButton)
    {
        Settings::deleteMode = true;
    }

    mainScene->modifyScene(pointPressed);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::RightButton)
    {
        return;
    }

    Point2D pointMoved = Point2D(event->pos().x(), event->pos().y());

    // Modify coordinates so that pointMoved variable is in relation to frame dimensions,
    // not app window
    pointMoved.x -= ui->frame->x();
    pointMoved.y -= ui->frame->y();

    dragYAngle = (pointMoved.y - mousePress.y + yAngle) % 360;
    double convertedDragYAngle = (dragYAngle * M_PI)/180;
    Settings::rotationMatrix_X.values[1][1] = cos(convertedDragYAngle);
    Settings::rotationMatrix_X.values[1][2] = -sin(convertedDragYAngle);
    Settings::rotationMatrix_X.values[2][1] = sin(convertedDragYAngle);
    Settings::rotationMatrix_X.values[2][2] = cos(convertedDragYAngle);

    dragXAngle = (pointMoved.x - mousePress.x + xAngle) % 360;
    double convertedDragXAngle = (dragXAngle * M_PI)/180;
    Settings::rotationMatrix_Y.values[0][0] = cos(convertedDragXAngle);
    Settings::rotationMatrix_Y.values[0][2] = -sin(convertedDragXAngle);
    Settings::rotationMatrix_Y.values[2][0] = sin(convertedDragXAngle);
    Settings::rotationMatrix_Y.values[2][2] = cos(convertedDragXAngle);

    mainScene->drawScene();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // If not caused by right mouse button, return
    if (event->button() != Qt::RightButton)
    {
        return;
    }

    // Overwrite angles
    yAngle = dragYAngle;
    xAngle = dragXAngle;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(ui->frame->x(), ui->frame->y(), *img);
}

void MainWindow::on_ambientLightingSlider_valueChanged(int value)
{
    Settings::backgroundLightIntensity = (double) value/100;
    mainScene->drawScene();
}

void MainWindow::on_lightIntensitySlider_valueChanged(int value)
{
    Settings::lightIntensity = (double) value/10;
    mainScene->drawScene();
}

void MainWindow::on_cubeButton_clicked()
{
    Settings::currentType = cube;
}

void MainWindow::on_slabButton_clicked()
{
    Settings::currentType = slab;
}

void MainWindow::on_wallbutton_clicked()
{
    Settings::currentType = wall;
}

void MainWindow::on_columnButton_clicked()
{
    Settings::currentType = column;
}

void MainWindow::on_redSlider_valueChanged(int value)
{
    Settings::currentColor.red = value;
}

void MainWindow::on_greenSlider_valueChanged(int value)
{
    Settings::currentColor.green = value;
}

void MainWindow::on_blueSlider_valueChanged(int value)
{
    Settings::currentColor.blue = value;
}
