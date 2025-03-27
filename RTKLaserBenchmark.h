#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RTKLaserBenchmark.h"

class RTKLaserBenchmark : public QMainWindow
{
    Q_OBJECT

public:
    RTKLaserBenchmark(QWidget *parent = nullptr);
    ~RTKLaserBenchmark();

private:
    Ui::RTKLaserBenchmarkClass ui;
};
