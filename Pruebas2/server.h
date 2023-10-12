#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();
    void clientDisconnected();
    void sendGridData();
    int getGridValue(int row, int col) const;

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
    int grid[10][10];

};

#endif // SERVER_H
