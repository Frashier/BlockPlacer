#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    // Main image, on which the scene will be drawn
    QImage *img;

    // application's scene
    Scene *mainScene;

    // Used for rotation by clicking and dragging
    Point2D mousePress;
    int yAngle = 0;
    int xAngle = 0;
    int dragYAngle;
    int dragXAngle;

    // function enabling painting on a QImage object
    void paintEvent(QPaintEvent*);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);


private slots:
    void on_ambientLightingSlider_valueChanged(int value);
    void on_lightIntensitySlider_valueChanged(int value);
    void on_cubeButton_clicked();
    void on_slabButton_clicked();
    void on_wallbutton_clicked();
    void on_columnButton_clicked();
    void on_redSlider_valueChanged(int value);
    void on_greenSlider_valueChanged(int value);
    void on_blueSlider_valueChanged(int value);
};
#endif // MAINWINDOW_H
