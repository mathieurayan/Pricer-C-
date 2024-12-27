#ifndef ASIAN_OPTION_H
#define ASIAN_OPTION_H

#include "ExoticOption.h"
#include "OptionType.h" // Inclure l'�num�ration OptionType
#include <vector>


class AsianOption : public ExoticOption {
public:
    OptionType optionType;   // Type d'option (Call ou Put)

    // Constructeur
    AsianOption(double strike_, double maturity_, OptionType optionType_);

    // M�thode pour le payoff bas� sur une trajectoire compl�te
    double payoff(const std::vector<double>& path) const;

    // Impl�mentation de la m�thode virtuelle pour le payoff (par d�faut inutilis�e)
    double payoff(double spot) const override;

    // M�thode de pricing Monte-Carlo
    double price(const BlackScholesModel& model, int numPaths, int steps) const override;

    double price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const;

    // Co�t de r�plication bas� sur la strat�gie de couverture dynamique
    double hedgeCost(const BlackScholesModel& model, int steps) const override;
};

#endif // ASIAN_OPTION_H
