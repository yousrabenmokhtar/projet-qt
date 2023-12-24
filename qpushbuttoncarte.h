#ifndef QPUSHBUTTONCARTE_H
#define QPUSHBUTTONCARTE_H

// QPushButtonCarte.h

#include <QPushButton>
#include "carte.h"

class QPushButtonCarte : public QPushButton
{
    Q_OBJECT

public:
   QPushButtonCarte(QWidget *parent = nullptr);

    Carte* getCarte() const { return carte_; }
    void setCarte(Carte* carte) { carte_ = carte; }

private:
    Carte* carte_;
};

#endif // QPUSHBUTTONCARTE_H
