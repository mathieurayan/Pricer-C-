#ifndef PUTOPTION_H
#define PUTOPTION_H

#include "Option.h" // Inclut la classe m�re Option

class PutOption : public Option {
public:
    // Constructeur
    PutOption(double strike_, double maturity_);

    // Impl�mentation du payoff pour un Put
    double payoff(double spot) const override;

    // R�plication bas�e sur Black-Scholes
    double hedgeCost(const BlackScholesModel& model, int steps) const override;
};

#endif // PUTOPTION_H
