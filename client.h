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
    void handleButton1Click ();
    void handleButton2Click();
    void handleButton3Click();


private slots:
    void receiveGridData();
    //void placeRandomSquare(const QString &color, const std::vector<std::pair<int, int>>& occupiedPositions);
    void findShortestPath();
    bool isValid(int row, int col) const;
    bool isNearObstacle(int row, int col);
    bool isRedSquare(int row, int col);
    bool isYellowSquare(int row, int col);
    bool isBlueSquare(int row, int col);
    //void updateLifeLabel(int life);
    bool isNearColoredSquare(int row, int col);
    void placePlayerSquares();
    void placeSquare(const QString &color, int row, int col);
    void clearGrid();
    //void startGreenSquareMovement();
    //bool isNearPlayerSquare(int row, int col);
    void moveToGoal();

private:
    QTcpSocket *tcpSocket;
    QGridLayout *gridLayout;
    QLabel *labels[10][10];
    int currentRow;
    int currentCol;
    bool obstacles[10][10];
    QLabel *lifeLabel;
    int goalRow;
    int goalCol;
    int life;
    QTimer *timer;

};

#endif
