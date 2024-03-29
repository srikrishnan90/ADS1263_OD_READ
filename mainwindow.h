#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ADS1263.h>
#include <DEV_Config.h>
#include <QDebug>
#include <QThread>

#include <mcp3004.h>
#include <softPwm.h>
#include <Iir.h>
#include <sr595.h>
#include <math.h>
#include <pi2c.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    int adc();

    int average_adc();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void firstpos();

    void firstposi();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void led_intensity_status(int wavelength);

    void write_led(int val[]);

    void on_pushButton_26_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
