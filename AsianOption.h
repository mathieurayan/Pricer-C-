#ifndef ASIAN_OPTION_H
#define ASIAN_OPTION_H

#include "ExoticOption.h"
#include "OptionType.h" // Inclure l'énumération OptionType
#include <vector>


class AsianOption : public ExoticOption {
public:
    OptionType optionType;   // Type d'option (Call ou Put)

    // Constructeur
    AsianOption(double strike_, double maturity_, OptionType optionType_);

    // Méthode pour le payoff basé sur une trajectoire complète
    double payoff(const std::vector<double>& path) const;

    // Implémentation de la méthode virtuelle pour le payoff (par défaut inutilisée)
    double payoff(double spot) const override;

    // Méthode de pricing Monte-Carlo
    double price(const BlackScholesModel& model, int numPaths, int steps) const override;

    double price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const;

    // Coût de réplication basé sur la stratégie de couverture dynamique
    double hedgeCost(const BlackScholesModel& model, int steps) const override;
};

#endif // ASIAN_OPTION_H
