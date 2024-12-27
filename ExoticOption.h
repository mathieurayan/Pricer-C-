#ifndef EXOTIC_OPTION_H
#define EXOTIC_OPTION_H

#include "Option.h"
#include "BlackScholesModel.h"
#include <vector>

class ExoticOption : public Option {
public:
    ExoticOption(double strike_, double maturity_);

    // Méthode virtuelle pour le pricing par Monte-Carlo
    virtual double price(const BlackScholesModel& model, int numPaths, int steps) const = 0;

    // Méthode générique pour calculer le coût de réplication
    virtual double hedgeCost(const BlackScholesModel& model, int steps) const = 0;

    // Méthode virtuelle pure pour calculer le payoff
    virtual double payoff(double spot) const = 0;

    virtual ~ExoticOption() = default;
};

#endif // EXOTIC_OPTION_H

