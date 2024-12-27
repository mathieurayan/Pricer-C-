#ifndef PUTOPTION_H
#define PUTOPTION_H

#include "Option.h" // Inclut la classe mère Option

class PutOption : public Option {
public:
    // Constructeur
    PutOption(double strike_, double maturity_);

    // Implémentation du payoff pour un Put
    double payoff(double spot) const override;

    // Réplication basée sur Black-Scholes
    double hedgeCost(const BlackScholesModel& model, int steps) const override;
};

#endif // PUTOPTION_H
