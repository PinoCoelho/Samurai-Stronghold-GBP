// client.cpp
#include "client.h"
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QRandomGenerator>
#include <queue>
#include <QCoreApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QRandomGenerator>

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

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            occupiedPositions[i][j] = false;
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

void Client::clearGrid()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            setGridValue(row, col, 0); // Esto establecerá todas las casillas en blanco
        }
    }
}

void Client::handleButton1Click()
{
    // Limpiar el tablero antes de iniciar
    clearGrid();

    // Inicializar la posición actual en (9,9)
    currentRow = 9;
    currentCol = 9;

    // Permitir que el jugador coloque los cuadrados a su elección
    placePlayerSquares();

    // Llamar a la función de camino más corto para mover el cuadrado verde de (9,9) a (0,0)
    //findShortestPath();  // Iniciar desde (9,9) y llegar a (0,0)
}

void Client::handleButton3Click()
{
    moveToGoal();
}

void Client::placePlayerSquares()
{
    // Cambia la declaración en client.cpp de std::pair a booleano
    bool occupiedPositions[10][10];


    for (int i = 0; i < 3; ++i) {
        QString color;
        bool validPlacement = false;
        int row, col;

        while (!validPlacement) {
            bool ok;
            QStringList items = {"Blue", "Red", "Yellow"};
            color = QInputDialog::getItem(nullptr, "Choose Square Color",
                                          "Select the color for square " + QString::number(i + 1) + ":",
                                          items, 0, false, &ok);

            if (!ok) {
                break;
            }

            row = QInputDialog::getInt(nullptr, "Choose Square Location",
                                       "Enter row (0-9) for square " + QString::number(i + 1) + ":",
                                       0, 0, 9, 1, &ok);

            if (!ok) {
                break;
            }

            col = QInputDialog::getInt(nullptr, "Choose Square Location",
                                       "Enter column (0-9) for square " + QString::number(i + 1) + ":",
                                       0, 0, 9, 1, &ok);

            if (!ok) {
                break;
            }

            if ((row == 0 && col == 0) || (row == 9 && col == 9)) {
                QMessageBox::warning(nullptr, "Invalid Placement",
                                     "No puedes colocar un cuadro en el punto de entrada o salida.");
            } else if (labels[row][col]->styleSheet() != "background-color: white; border: 1px solid black;") {
                QMessageBox::warning(nullptr, "Invalid Placement",
                                     "La posición seleccionada ya está ocupada por otro cuadro.");
            } else {
                validPlacement = true;
            }
        }

        if (!validPlacement) {
            qDebug() << "Colocación de cuadro cancelada por el jugador.";
            break;
        }

        placeSquare(color, row, col);

        // Actualiza la matriz de posiciones ocupadas
        occupiedPositions[row][col] = true;
    }
}

void Client::placeSquare(const QString &color, int row, int col)
{
    if (color.toLower() == "blue") {
        setGridValue(row, col, 1); // Azul
    } else if (color.toLower() == "red") {
        setGridValue(row, col, 2); // Rojo
    } else if (color.toLower() == "yellow") {
        setGridValue(row, col, 3); // Amarillo
    }
}

bool Client::isValid(int row, int col) const
{
    return row >= 0 && row < 10 && col >= 0 && col < 10 &&
           labels[row][col]->styleSheet() == "background-color: white; border: 1px solid black;";
}

void Client::moveToGoal()
{
    if (currentRow == goalRow && currentCol == goalCol) {
        timer->stop();
        lifeLabel->setText("Life: " + QString::number(life));
        return;
    }

    findShortestPath();

    lifeLabel->setText("Life: " + QString::number(life));
    QCoreApplication::processEvents();

    if (life <= 0) {
        timer->stop();
        QMessageBox::information(nullptr, "Game Over", "Your life reached zero. Game over!");
    }

    greenMoves++;

    if (greenMoves % 3 == 0) {
        moveRandomSquares();
    }
}


// Resto de tu código

void Client::findShortestPath()
{
    // Inicializa las variables y coordenadas del cuadro verde
    int greenRow = 9;
    int greenCol = 9;

    // Inicializa un contador para el movimiento del cuadro verde
    int greenMoves = 0;

    // Bucle principal para controlar el movimiento del cuadro verde
    while (greenRow != 0 || greenCol != 0) {
        // Realiza la búsqueda de la ruta para mover el cuadro verde
        std::queue<std::pair<int, int>> queue;
        std::vector<std::vector<int>> distance(10, std::vector<int>(10, -1));
        std::vector<std::vector<bool>> visited(10, std::vector<bool>(10, false));
        int life = 100; // Puntos de vida iniciales

        // Objetivo
        int goalRow = 0;
        int goalCol = 0;

        queue.push({greenRow, greenCol});
        distance[greenRow][greenCol] = 0;

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

                // Verificar si la casilla es válida y no ha sido visitada
                if (isValid(newRow, newCol) && distance[newRow][newCol] == -1) {
                    queue.push({newRow, newCol});
                    distance[newRow][newCol] = distance[row][col] + 1;

                    // Verificar si está cerca de un obstáculo y reducir la vida
                    if (isNearObstacle(newRow, newCol)) {
                        if (isRedSquare(newRow, newCol)) {
                            life -= 30; // Rojo
                            qDebug() << life;
                        } else if (isYellowSquare(newRow, newCol)) {
                            life -= 25; // Amarillo
                            qDebug() << life;
                        } else if (isBlueSquare(newRow, newCol)) {
                            life -= 20; // Azul
                            qDebug() << life;
                        }
                        lifeLabel->setText("Life: " + QString::number(life));
                        qDebug() << lifeLabel;
                    }
                }
            }
        }

        // Reconstruye el camino desde la meta hacia atrás
        int row = goalRow;
        int col = goalCol;

        while (row != greenRow || col != greenCol) {
            int minDistance = distance[row][col];

            // Movimientos posibles (arriba, abajo, izquierda, derecha)
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            int bestRow = -1;
            int bestCol = -1;

            for (int i = 0; i < 4; i++) {
                int newRow = row + dr[i];
                int newCol = col + dc[i];

                // Verificar si la casilla es válida y tiene una distancia menor
                if (isValid(newRow, newCol) && distance[newRow][newCol] < minDistance) {
                    minDistance = distance[newRow][newCol];
                    bestRow = newRow;
                    bestCol = newCol;
                }
            }

            // Mueve al siguiente paso del camino
            row = bestRow;
            col = bestCol;
            labels[row][col]->setStyleSheet("background-color: green; border: 1px solid black;");
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, &QEventLoop::quit);
            loop.exec();
        }

        // Actualiza la posición actual del cuadro verde
        greenRow = 0;
        greenCol = 0;

        // Incrementa el contador de movimientos del cuadro verde
        greenMoves++;

        // Cuando el cuadro verde ha completado 3 movimientos
        if ((greenRow - row) % 3 == 0 && (greenCol - col) % 3 == 0) {
            moveRandomSquares();
            greenMoves = 0;
        }

            // Reinicia el contador de movimientos del cuadro verde
        //greenMoves = 0;
        moveToGoal();
    }
}

// Función para mover cuadros amarillo, azul y rojo de forma aleatoria
void Client::moveRandomSquares()
{
    std::vector<std::pair<int, int>> occupiedPositions;

    // Elimina los cuadros amarillo, azul y rojo existentes
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (isRedSquare(i, j) || isYellowSquare(i, j) || isBlueSquare(i, j)) {
                setGridValue(i, j, 0);
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        int randomRow, randomCol;

        do {
            randomRow = QRandomGenerator::global()->bounded(10);
            randomCol = QRandomGenerator::global()->bounded(10);
        } while (std::find(occupiedPositions.begin(), occupiedPositions.end(), std::make_pair(randomRow, randomCol)) != occupiedPositions.end() ||
                 (randomRow == currentRow && randomCol == currentCol)); // Evita colocar cuadros en la posición del cuadro verde

        QString color;
        bool validPlacement = false;

        while (!validPlacement) {
            bool ok;
            QStringList items = {"Blue", "Red", "Yellow"};
            color = QInputDialog::getItem(nullptr, "Choose Square Color",
                                          "Select the color for square " + QString::number(i + 1) + ":",
                                          items, 0, false, &ok);

            if (!ok) {
                break;
            }

            if (color.toLower() == "blue") {
                setGridValue(randomRow, randomCol, 1); // Azul
            } else if (color.toLower() == "red") {
                setGridValue(randomRow, randomCol, 2); // Rojo
            } else if (color.toLower() == "yellow") {
                setGridValue(randomRow, randomCol, 3); // Amarillo
            }

            occupiedPositions.push_back({randomRow, randomCol});
            validPlacement = true;
        }
    }
}


bool Client::isColoredSquare(int row, int col)
{
    // Verificar si la casilla en (row, col) contiene un cuadro de color (azul, rojo o amarillo)
    return isBlueSquare(row, col) || isRedSquare(row, col) || isYellowSquare(row, col);
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
