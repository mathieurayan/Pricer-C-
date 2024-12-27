#ifndef BARRIER_OPTION_H
#define BARRIER_OPTION_H

#include "ExoticOption.h"
#include "OptionType.h" // Inclure l'�num�ration OptionType
#include <vector>

enum class BarrierType { UpAndOut, UpAndIn, DownAndOut, DownAndIn };

class BarrierOption : public ExoticOption {
public:
    double barrier;           // Niveau de la barri�re
    BarrierType barrierType;  // Type de barri�re
    OptionType optionType;    // Type d'option (Call ou Put)

    // Constructeur
    BarrierOption(double strike_, double maturity_, double barrier_, BarrierType barrierType_, OptionType optionType_);

    // M�thode pour le payoff
    double payoff(double spot) const;


    // M�thode de pricing Monte-Carlo
    double price(const BlackScholesModel& model, int numPaths, int steps) const override;

    double price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const;

    // M�thode pour calculer le co�t de r�plication
    double hedgeCost(const BlackScholesModel& model, int steps) const override;

private:
    // V�rifie si la barri�re est franchie
    bool isBarrierTouched(const std::vector<double>& path) const;
};

#endif // BARRIER_OPTION_H
