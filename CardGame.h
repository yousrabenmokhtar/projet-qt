#ifndef CARDGAME_H
#define CARDGAME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class CardGame : public QWidget {
    Q_OBJECT

public:
    explicit CardGame(QWidget *parent = nullptr);

public slots:
    void dealCards(const QList<QPixmap> &playerCards, const QList<QPixmap> &computerCards);

private:
    QVBoxLayout *mainLayout;
    QLabel *playerLabel;
    QLabel *computerLabel;
    QPushButton *dealButton;
    // ... d'autres membres nécessaires pour votre jeu de cartes
};

#endif // CARDGAME_H
