#ifndef BARRIER_OPTION_H
#define BARRIER_OPTION_H

#include "ExoticOption.h"
#include "OptionType.h" // Inclure l'énumération OptionType
#include <vector>

enum class BarrierType { UpAndOut, UpAndIn, DownAndOut, DownAndIn };

class BarrierOption : public ExoticOption {
public:
    double barrier;           // Niveau de la barrière
    BarrierType barrierType;  // Type de barrière
    OptionType optionType;    // Type d'option (Call ou Put)

    // Constructeur
    BarrierOption(double strike_, double maturity_, double barrier_, BarrierType barrierType_, OptionType optionType_);

    // Méthode pour le payoff
    double payoff(double spot) const;


    // Méthode de pricing Monte-Carlo
    double price(const BlackScholesModel& model, int numPaths, int steps) const override;

    double price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const;

    // Méthode pour calculer le coût de réplication
    double hedgeCost(const BlackScholesModel& model, int steps) const override;

private:
    // Vérifie si la barrière est franchie
    bool isBarrierTouched(const std::vector<double>& path) const;
};

#endif // BARRIER_OPTION_H
