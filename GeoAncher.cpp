#include "GeoAncher.h"

const int port = 3001;

GeoAncher::GeoAncher(QWidget *parent)
    : QWidget(parent), server(Server())
{
    ui.setupUi(this);
    server.startServer(port);

	connect(&this->server, &Server::connected, this, &GeoAncher::onConnected);
}

GeoAncher::~GeoAncher()
{}

void GeoAncher::onConnected() {
	ui.stateLabel->setText("连接成功");
}