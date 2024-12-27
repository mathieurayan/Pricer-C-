#ifndef EXOTIC_OPTION_H
#define EXOTIC_OPTION_H

#include "Option.h"
#include "BlackScholesModel.h"
#include <vector>

class ExoticOption : public Option {
public:
    ExoticOption(double strike_, double maturity_);

    // M�thode virtuelle pour le pricing par Monte-Carlo
    virtual double price(const BlackScholesModel& model, int numPaths, int steps) const = 0;

    // M�thode g�n�rique pour calculer le co�t de r�plication
    virtual double hedgeCost(const BlackScholesModel& model, int steps) const = 0;

    // M�thode virtuelle pure pour calculer le payoff
    virtual double payoff(double spot) const = 0;

    virtual ~ExoticOption() = default;
};

#endif // EXOTIC_OPTION_H

