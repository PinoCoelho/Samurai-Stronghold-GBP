#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(QString host, int port);
    void setGridValue(int row, int col, int value);

private slots:
    void receiveGridData();

private:
    QTcpSocket *tcpSocket;
    QGridLayout *gridLayout;
    QLabel *labels[10][10];
};

#endif
