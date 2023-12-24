#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "carte.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>
#include <QPixmap>
#include <QTime>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QMessageBox> // Pour inclure la classe QMessageBox

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    paquetDeCartesGenere(false),
    carteSelectionneeParJoueur(nullptr),  // Initialisez la variable à nullptr
    scoreJoueur1(0),  // Initialise le score du joueur à 0
    scoreAI(0),  // Initialise le score de l'ordinateur à 0
    tourJoueur(true)  // Initialise le tour du joueur à true

{
    ui->setupUi(this);

    // Connecter le signal editingFinished() de lineEdit au slot entreruser()
    connect ( ui->lineEdit , &QLineEdit::editingFinished , this ,[=]{this->entreruser ();});
    connect (ui->pushButton_5 , &QAbstractButton::clicked , this ,[=]{this->onNiveauDifficulteClicked();});
    connect (ui->pushButton_4 , &QAbstractButton::clicked , this ,[=]{this->onNiveauDifficulteClicked();});
    connect (ui->pushButton_3 , &QAbstractButton::clicked , this ,[=]{this->onNiveauDifficulteClicked();});


    // Désactiver les boutons de sélection de niveau initialement
    ui->pushButton_5->setEnabled(false);  // Bouton "Facile"
    ui->pushButton_4->setEnabled(false);  // Bouton "Moyen"
    ui->pushButton_3->setEnabled(false);  // Bouton "Difficile"

    //  d'utilisation de la fonction statique genererPaquetDeCartes
    QList<Carte*> paquet = Carte::genererPaquetDeCartes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::entreruser()
{
    QString nomJoueur = ui->lineEdit->text();

    if (nomJoueur.isEmpty()) {
        ui->lineEdit->setStyleSheet("border: 2px solid red;");
        ui->pushButton_5->setEnabled(false);  // Désactiver le bouton "Facile"
        ui->pushButton_4->setEnabled(false);  // Désactiver le bouton "Moyen"
        ui->pushButton_3->setEnabled(false);  // Désactiver le bouton "Difficile"
    } else {
        ui->lineEdit->setStyleSheet("");  // Réinitialiser le style
        ui->pushButton_5->setEnabled(true);  // Activer le bouton "Facile"
        ui->pushButton_4->setEnabled(true);  // Activer le bouton "Moyen"
        ui->pushButton_3->setEnabled(true);  // Activer le bouton "Difficile"

        QString message = "CHOOSE A LEVEL, " + nomJoueur;

        // Définir le style du QLabel pour centrer le texte et appliquer d'autres styles
        ui->label_7->setStyleSheet("font-size: 40px; font-style: italic; text-align: center; font-family:Chiller;");

        // Mettre à jour le texte du QLabel
        ui->label_7->setText(message);
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked()
{
    // Page Facile
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_pushButton_5_clicked()
{
    // Page Difficile
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_pushButton_4_clicked()
{
    // Page Moyen
    ui->stackedWidget->setCurrentIndex(2);

}
//////////////////////////////////////////////////////:


//////////////////////////////////////////////////

void MainWindow::onNiveauDifficulteClicked()
{
    // Vérifiez si le paquet a déjà été généré
    if (!paquetDeCartesGenere) {
        // Appelez votre fonction genererPaquetDeCartes ici
        paquetDeCartes = Carte::genererPaquetDeCartes();

        qDebug() << "Nombre de cartes générées : " << paquetDeCartes.size();

                              // Mélangez aléatoirement le paquet
                           std::random_shuffle(paquetDeCartes.begin(), paquetDeCartes.end());

        // Chargez l'image de la carte "back"
        QPixmap backCardPixmap(":/Cards/back.jpg");

        // Affichez l'image de la carte "back" en tant qu'icône du bouton
        ui->pushButtonGenererPaquet->setIcon(QIcon(backCardPixmap));
        ui->pushButtonGenererPaquet->setIconSize(backCardPixmap.size());

        // Mettez à jour l'indicateur pour indiquer que le paquet a été généré
        paquetDeCartesGenere = true;
        // Distribuez les cartes sur les layouts
        distribuerCartes();
    }

}
/////////////////////////////////

/////////////////////////////////////:
void MainWindow::distribuerCartes()
{
    for (int i = 0; i < 8; ++i) {

        QString nomBouton = "pushButton_" + QString::number(i + 6);
        QPushButton *bouton = findChild<QPushButton *>(nomBouton);
        QPixmap cartePixmap(paquetDeCartes[i]->getChemin());
        bouton->setObjectName(paquetDeCartes[i]->getNAME());

        // Redimensionnez l'image si nécessaire
        cartePixmap = cartePixmap.scaled(QSize(100, 150));  // Modifiez la taille selon vos besoins

        bouton->setIcon(QIcon(cartePixmap));
        bouton->setIconSize(cartePixmap.size());

        // Ajoutez les 4 premiers boutons à layoutCartes1
        if (i < 4) {
            ui->layoutCartes1->addWidget(bouton);
        }
        // Ajoutez les 4 boutons suivants à layoutCartes2
        else {
            ui->layoutCartes2->addWidget(bouton);
        }

        // Connectez le clic de l'utilisateur sur le bouton
        connect(bouton, &QPushButton::clicked, this, [=](){
            onCarteButtonClicked(bouton);
        });
    }}
/////////////////////////////////////

void MainWindow::onCarteButtonClicked(QPushButton *carteButton)
{
    if (carteButton) {
        carteJoueurActuelle = paquetDeCartes[ui->layoutCartes2->indexOf(carteButton)];
        // Effacez les cartes du layoutCartes3
        effacerCartesLayout3();
        deplacerCarteAvecAnimation(ui->layoutCartes2, ui->layoutCartes3, carteButton);

        disconnect(carteButton, &QPushButton::clicked, this, nullptr);

        // Connecter le signalAnimationTerminee à la fonction tourOrdinateurApresJoueur
        connect(this, &MainWindow::signalAnimationTerminee, this, &MainWindow::tourOrdinateur);
    }
}





///////////////////////////


void MainWindow::deplacerCarteAvecAnimation(QLayout *sourceLayout, QLayout *destinationLayout, QPushButton *carteButton)
{
    // Créer une animation de translation
    QPropertyAnimation *animation = new QPropertyAnimation(carteButton, "pos", this);
    animation->setDuration(1000); // 1000 millisecondes (1 seconde)
    animation->setStartValue(carteButton->pos());
    animation->setEndValue(destinationLayout->geometry().topLeft());

    // Connecter le signal finished de l'animation à la fonction tourOrdinateur
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
        // Retirez le bouton du layout source
        sourceLayout->removeWidget(carteButton);

        // Ajoutez le bouton au layout de destination
        destinationLayout->addWidget(carteButton);

        // Rafraîchissez les layouts pour vous assurer que les changements sont pris en compte
        sourceLayout->update();
        destinationLayout->update();

        // Rafraîchissez également la géométrie du widget parent pour éviter les problèmes d'affichage
        QWidget *parentWidget = qobject_cast<QWidget*>(sourceLayout->parent());
        if (parentWidget) {
            parentWidget->updateGeometry();
        }

        parentWidget = qobject_cast<QWidget*>(destinationLayout->parent());
        if (parentWidget) {
            parentWidget->updateGeometry();
        }

        // Appeler directement tourOrdinateur après l'animation
        tourOrdinateur();
    });

    // Démarrer l'animation
    animation->start();
}




///////////////:::::::::::::
void MainWindow::deplacerCarteAvecAnimation1(QLayout *sourceLayout, QLayout *destinationLayout, QPushButton *carteButton)
{
    // Créer une animation de translation
    QPropertyAnimation *animation = new QPropertyAnimation(carteButton, "pos", this);
    animation->setDuration(1000); // 1000 millisecondes (1 seconde)
    animation->setStartValue(carteButton->pos());
    animation->setEndValue(destinationLayout->geometry().topLeft());

    // Connecter le signal finished de l'animation à la fonction tourOrdinateur
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
        // Retirez le bouton du layout source
        sourceLayout->removeWidget(carteButton);

        // Ajoutez le bouton au layout de destination
        destinationLayout->addWidget(carteButton);

        // Rafraîchissez les layouts pour vous assurer que les changements sont pris en compte
        sourceLayout->update();
        destinationLayout->update();

        // Rafraîchissez également la géométrie du widget parent pour éviter les problèmes d'affichage
        QWidget *parentWidget = qobject_cast<QWidget*>(sourceLayout->parent());
        if (parentWidget) {
            parentWidget->updateGeometry();
        }

        parentWidget = qobject_cast<QWidget*>(destinationLayout->parent());
        if (parentWidget) {
            parentWidget->updateGeometry();
        }
        comparerCartes();

    });

    // Démarrer l'animation
    animation->start();

}

// ...

void MainWindow::tourOrdinateur()
{
    if (ui->layoutCartes1->count() > 0) {
        int indexCarteAleatoire = QRandomGenerator::global()->bounded(ui->layoutCartes1->count());
        QPushButton* carteButton = qobject_cast<QPushButton*>(ui->layoutCartes1->itemAt(indexCarteAleatoire)->widget());

        if (carteButton) {
            carteOrdinateurActuelle = paquetDeCartes[indexCarteAleatoire];
            deplacerCarteAvecAnimation1(ui->layoutCartes1, ui->layoutCartes3, carteButton);

            // Connecter le signalAnimationTerminee à la fonction comparerCartes
            connect(this, &MainWindow::signalAnimationTerminee, this, &MainWindow::comparerCartes);
        }
    }
}

///////////////////////////////////

void  MainWindow::comparerCartes()
{
    if (carteJoueurActuelle && carteOrdinateurActuelle) {
        int valeurJoueur = carteJoueurActuelle->getValeur();
        int valeurOrdinateur = carteOrdinateurActuelle->getValeur();

        if (valeurJoueur > valeurOrdinateur) {
            scoreJoueur1 += 2;
            int nouvelleTaillePolice = 12;

            // Créer une police avec la nouvelle taille
            QFont nouvellePolice;
            nouvellePolice.setPointSize(nouvelleTaillePolice);

            // Appliquer la nouvelle police au widget de texte (scoreJoueur1)
            ui->scoreJoueur1->setFont(nouvellePolice);

            ui->scoreJoueur1->setText(QString("%1").arg(scoreJoueur1));
        } else if (valeurJoueur < valeurOrdinateur) {
            scoreAI += 2;
            int nouvelleTaillePolice = 12;

            // Créer une police avec la nouvelle taille
            QFont nouvellePolice;
            nouvellePolice.setPointSize(nouvelleTaillePolice);

            // Appliquer la nouvelle police au widget de texte (scoreJoueur1)
            ui->scoreJoueur2->setFont(nouvellePolice);

            ui->scoreJoueur2->setText(QString("%1").arg(scoreAI));
        } else {
            scoreJoueur1 += 1;
            scoreAI += 1;
            ui->scoreJoueur1->setText(QString("%1").arg(scoreJoueur1));
            ui->scoreJoueur2->setText(QString("%1").arg(scoreAI));
        }

        // Réinitialisez les cartes actuelles
        carteJoueurActuelle = nullptr;
        carteOrdinateurActuelle = nullptr;

        // Relancez le tour du joueur
        QTimer::singleShot(1000, this, [=]() {
            tourJoueur = true;
            // Vous pouvez ajouter des logiques supplémentaires pour le tour du joueur ici si nécessaire

        });
    }
}
///////////////////////////////////
void MainWindow::effacerCartesLayout3()
{
    // Parcours de tous les widgets dans le layoutCartes3 et les supprimer
    QLayoutItem *item;
    while ((item = ui->layoutCartes3->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

