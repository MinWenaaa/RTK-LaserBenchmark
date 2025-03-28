#pragma once

#include <QtWidgets/QWidget>
#include "ui_GeoAncher.h"
#include "server.h"

class GeoAncher : public QWidget
{
    Q_OBJECT

public:
    GeoAncher(QWidget *parent = nullptr);
    ~GeoAncher();

private slots:
    void onConnected();

private:
    Ui::GeoAncherClass ui;
	Server server;
};
