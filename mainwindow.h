#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QList>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPixmap>
#include "carte.h"
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool jeuTermine() const;
signals:
    void signalAnimationTerminee();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void entreruser();
   // void afficherGagnant();
    void onNiveauDifficulteClicked();

    void deplacerCarteAvecAnimation(QLayout *sourceLayout, QLayout *destinationLayout, QPushButton *carteButton);
    void deplacerCarteAvecAnimation1(QLayout *sourceLayout, QLayout *destinationLayout, QPushButton *carteButton);
    void tourOrdinateur();
    void onCarteButtonClicked(QPushButton *carteButton);
    void comparerCartes();
    void effacerCartesLayout3();
    void distribuerCartes();

private:
    Ui::MainWindow *ui;
    QList<Carte*> paquetDeCartes;
    bool paquetDeCartesGenere;
    QLabel *carteSelectionneeParJoueur;
    QLayout *layoutCartes3;
    QLayout *layoutCartes1;
 //  int scoreToWin;
    QLayout *layoutCartes2;
    QPushButton* trouverDerniereCarte(QLayout* layout);
    Carte* trouverCarteMax();
    Carte* carteJoueurActuelle;
    Carte* carteOrdinateurActuelle;
    int scoreJoueur1;  // Ajouté pour stocker le score du joueur
    int scoreAI;  // Ajouté pour stocker le score de l'ordinateur
    bool tourJoueur; // true si c'est le tour du joueur, false sinon

};

#endif // MAINWINDOW_H
