#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::receiveGridData);

    // Create a GUI for displaying the grid
    QWidget *window = new QWidget();
    window->setWindowTitle("Samurai Stronghold GBP");
    window->resize(800, 800);

    gridLayout = new QGridLayout(window);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            labels[i][j] = new QLabel("");
            labels[i][j]->setStyleSheet("background-color: white; border: 1px solid black;");
            gridLayout->addWidget(labels[i][j], i, j);
        }
    }

    window->show();
}

void Client::connectToServer(QString host, int port)
{
    tcpSocket->connectToHost(host, port);
    qDebug() << "Se conectó al cliente";
}

void Client::receiveGridData()
{
    QDataStream in(tcpSocket);
    int size = 0;
    in >> size;
    if (tcpSocket->bytesAvailable() < size) {
        return;
    }

    int receivedGrid[10][10];
    in.readRawData(reinterpret_cast<char*>(receivedGrid), sizeof(int) * 10 * 10);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int value = receivedGrid[row][col];
            setGridValue(row, col, value);
        }
    }
}

void Client::setGridValue(int row, int col, int value)
{
    if (value == 0) {
        labels[row][col]->setStyleSheet("background-color: white; border: 1px solid black;");
    } else {
        labels[row][col]->setStyleSheet("background-color: black; border: 1px solid black;");
    }
}
