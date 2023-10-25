#include "server.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>

Server::Server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server couldn't start";
        return;
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

    // Initialize the grid with some data (you can modify this part as needed)
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = 0;
        }
    }
}

void Server::newConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::sendGridData);
    qDebug() << "Se conectó al server";
}

void Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clients.removeOne(clientSocket);
        clientSocket->deleteLater();
    }
}

void Server::sendGridData()
{
    int size = sizeof(int) * 10 * 10;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << size;
    stream.writeRawData(reinterpret_cast<const char*>(grid), size);

    for (QTcpSocket *client : clients) {
        client->write(data);
    }
}

int Server::getGridValue(int row, int col) const {
    return grid[row][col];
}

