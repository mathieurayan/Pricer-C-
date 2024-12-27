#ifndef CALLOPTION_H
#define CALLOPTION_H

#include "Option.h" // Inclut la classe m�re Option

class CallOption : public Option {
public:
    // Constructeur
    CallOption(double strike_, double maturity_);

    // Impl�mentation du payoff pour un Call
    double payoff(double spot) const override;

    // R�plication bas�e sur Black-Scholes
    double hedgeCost(const BlackScholesModel& model, int steps) const override;
};

#endif // CALLOPTION_H
