#include "client.h"
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QEventLoop>
#include <QRandomGenerator>
#include <queue>
#include <QCoreApplication>

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::receiveGridData);

    // Create a GUI for displaying the grid
    QWidget *window = new QWidget();
    window->setWindowTitle("Samurai Stronghold GBP");
    window->resize(800, 800);

    gridLayout = new QGridLayout(window);

    // Initialize the grid and other UI elements
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            labels[i][j] = new QLabel("");
            labels[i][j]->setStyleSheet("background-color: white; border: 1px solid black;");
            gridLayout->addWidget(labels[i][j], i, j);
        }
    }

    // Create a toolbar in the top left corner
    QToolBar *toolBar = new QToolBar(window);
    window->layout()->setMenuBar(toolBar);

    // Add buttons to the toolbar
    QAction *button1 = toolBar->addAction("Samurai");
    QAction *button2 = toolBar->addAction("Statistics");
    QAction *button3 = toolBar->addAction("Button 3");

    connect(button1, &QAction::triggered, this, &Client::handleButton1Click);
    connect(button2, &QAction::triggered, this, &Client::handleButton2Click);
    connect(button3, &QAction::triggered, this, &Client::handleButton3Click);

    // Create and configure the lifeLabel
    lifeLabel = new QLabel("Life: 100", window);
    lifeLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    lifeLabel->setStyleSheet("background-color: transparent; border: 1px solid black; padding: 5px;");
    window->layout()->addWidget(lifeLabel);

    // Raise the lifeLabel to the top layer
    lifeLabel->raise();

    window->show();
}

void Client::connectToServer(QString host, int port)
{
    tcpSocket->connectToHost(host, port);
    qDebug() << "Connected to the server";
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
    } else if (value == 1) {
        labels[row][col]->setStyleSheet("background-color: blue; border: 1px solid black;");
    } else if (value == 2) {
        labels[row][col]->setStyleSheet("background-color: red; border: 1px solid black;");
    } else if (value == 3) {
        labels[row][col]->setStyleSheet("background-color: yellow; border: 1px solid black;");
    }
}

void Client::handleButton1Click()
{
    // Inicializar la posición actual en (9,9)
    currentRow = 9;
    currentCol = 9;

    // Colocar cuadros aleatorios en el mapa, asegurándose de que no estén en (0,0) y (9,9)
    std::vector<std::pair<int, int>> occupiedPositions;
    occupiedPositions.push_back({0, 0}); // Inicial
    occupiedPositions.push_back({9, 9}); // Salida
    placeRandomSquare("blue", occupiedPositions);
    placeRandomSquare("red", occupiedPositions);
    placeRandomSquare("yellow", occupiedPositions);

    // Llamar a la función de camino más corto para mover el cuadrado verde de (9,9) a (0,0)
    findShortestPath();  // Iniciar desde (9,9) y llegar a (0,0)
}


void Client::placeRandomSquare(const QString &color, const std::vector<std::pair<int, int>>& occupiedPositions)
{
    int randomRow, randomCol;

    // Haz una copia de la variable ocupada para que puedas modificarla
    std::vector<std::pair<int, int>> occupiedCopy = occupiedPositions;

    do {
        randomRow = QRandomGenerator::global()->bounded(10);
        randomCol = QRandomGenerator::global()->bounded(10);
    } while (labels[randomRow][randomCol]->styleSheet() != "background-color: white; border: 1px solid black;" ||
             std::find(occupiedCopy.begin(), occupiedCopy.end(), std::make_pair(randomRow, randomCol)) != occupiedCopy.end());

    if (color == "blue") {
        labels[randomRow][randomCol]->setStyleSheet("background-color: blue; border: 1px solid black;");
    } else if (color == "red") {
        labels[randomRow][randomCol]->setStyleSheet("background-color: red; border: 1px solid black;");
    } else if (color == "yellow") {
        labels[randomRow][randomCol]->setStyleSheet("background-color: yellow; border: 1px solid black;");
    }

    // Agregar la posición ocupada a la copia
    occupiedCopy.push_back({randomRow, randomCol});
}


bool Client::isValid(int row, int col) const
{
    return row >= 0 && row < 10 && col >= 0 && col < 10 &&
           labels[row][col]->styleSheet() == "background-color: white; border: 1px solid black;";
}


void Client::findShortestPath()
{
    std::queue<std::pair<int, int>> queue;
    std::vector<std::vector<int>> distance(10, std::vector<int>(10, -1));
    std::vector<std::vector<bool>> visited(10, std::vector<bool>(10, false));
    int life = 100; // Puntos de vida iniciales

    // Iniciar desde la casilla inicial
    int startRow = 9;
    int startCol = 9;
    int goalRow = 0;
    int goalCol = 0;

    queue.push({startRow, startCol});
    distance[startRow][startCol] = 0;

    while (!queue.empty() && life > 0) {
        auto current = queue.front();
        int row = current.first;
        int col = current.second;
        queue.pop();
        visited[row][col] = true;

        // Verificar si hemos llegado a la meta
        if (row == goalRow && col == goalCol) {
            break;
        }

        // Movimientos posibles (arriba, abajo, izquierda, derecha)
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            if (isValid(newRow, newCol) && distance[newRow][newCol] == -1) {
                queue.push({newRow, newCol});
                distance[newRow][newCol] = distance[row][col] + 1;

                // Actualizar el lifeLabel después de cada movimiento
                lifeLabel->setText("Life: " + QString::number(life));
                QCoreApplication::processEvents();
                QEventLoop loop;
                QTimer::singleShot(1000, &loop, &QEventLoop::quit);
                loop.exec();

                // Verificar si está cerca de un obstáculo y reducir la vida
                if (isNearObstacle(newRow, newCol)) {
                    if (isRedSquare(newRow, newCol)) {
                        life -= 30; // Rojo
                    } else if (isYellowSquare(newRow, newCol)) {
                        life -= 25; // Amarillo
                    } else if (isBlueSquare(newRow, newCol)) {
                        life -= 20; // Azul
                    }
                }
            }
        }
    }

    // Reconstruir el camino desde la meta hacia atrás
    int row = goalRow;
    int col = goalCol;

    while (row != startRow || col != startCol) {
        int minDistance = distance[row][col];
        int bestRow = -1;
        int bestCol = -1;

        // Movimientos posibles (arriba, abajo, izquierda, derecha)
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            if (isValid(newRow, newCol) && distance[newRow][newCol] < minDistance) {
                minDistance = distance[newRow][newCol];
                bestRow = newRow;
                bestCol = newCol;
            }
        }

        // Mover al siguiente paso del camino
        row = bestRow;
        col = bestCol;
        labels[row][col]->setStyleSheet("background-color: green; border: 1px solid black;");

        // Verificar si está adyacente a un cuadro amarillo, rojo o azul
        if (isNearColoredSquare(row, col)) {
            if (isRedSquare(row, col)) {
                life -= 30; // Rojo
            } else if (isYellowSquare(row, col)) {
                life -= 25; // Amarillo
            } else if (isBlueSquare(row, col)) {
                life -= 20; // Azul
            }
        }

        // Actualizar el lifeLabel después de cada movimiento
        lifeLabel->setText("Life: " + QString::number(life));
        QCoreApplication::processEvents();
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

bool Client::isNearColoredSquare(int row, int col)
{
    // Verificar si está adyacente a un cuadro amarillo, rojo o azul (arriba, abajo, izquierda, derecha)
    return (isYellowSquare(row - 1, col) || isYellowSquare(row + 1, col) ||
            isYellowSquare(row, col - 1) || isYellowSquare(row, col + 1) ||
            isRedSquare(row - 1, col) || isRedSquare(row + 1, col) ||
            isRedSquare(row, col - 1) || isRedSquare(row, col + 1) ||
            isBlueSquare(row - 1, col) || isBlueSquare(row + 1, col) ||
            isBlueSquare(row, col - 1) || isBlueSquare(row, col + 1));
}



bool Client::isNearObstacle(int row, int col)
{
    // Verificar si está cerca de un obstáculo (azul, amarillo o rojo).
    // Puedes personalizar esta lógica según tus necesidades.
    return isRedSquare(row, col) || isYellowSquare(row, col) || isBlueSquare(row, col);
}

bool Client::isRedSquare(int row, int col)
{
    // Verificar si la casilla en (row, col) es un cuadro rojo.
    // Puedes personalizar esta lógica según tu implementación.
    return labels[row][col]->styleSheet() == "background-color: red; border: 1px solid black;";
}

bool Client::isYellowSquare(int row, int col)
{
    // Verificar si la casilla en (row, col) es un cuadro amarillo.
    // Puedes personalizar esta lógica según tu implementación.
    return labels[row][col]->styleSheet() == "background-color: yellow; border: 1px solid black;";
}

bool Client::isBlueSquare(int row, int col)
{
    // Verificar si la casilla en (row, col) es un cuadro azul.
    // Puedes personalizar esta lógica según tu implementación.
    return labels[row][col]->styleSheet() == "background-color: blue; border: 1px solid black;";
}


void Client::handleButton2Click()
{
    // Logic for handling Button 2 click
}

void Client::handleButton3Click()
{
    // Logic for handling Button 3 click
}
