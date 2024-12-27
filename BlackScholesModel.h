#ifndef BLACK_SCHOLES_MODEL_H
#define BLACK_SCHOLES_MODEL_H

#include <cmath> // Pour std::log, std::sqrt, std::exp
#include "Option.h" // Inclut la classe abstraite Option

class BlackScholesModel {
public:
    // Membres publics pour les param�tres du mod�le
    double spot;        // Prix initial de l'actif sous-jacent
    double rate;        // Taux sans risque
    double volatility;  // Volatilit� du sous-jacent
    double dividend;    // Taux de dividende

    // Constructeur
    BlackScholesModel(double spot_, double rate_, double volatility_, double dividend_);

    // M�thode pour calculer le prix analytique des options vanilles (Call ou Put)
    double priceAnalytic(const Option *option, bool isCall) const;

private:
    // M�thode priv�e pour calculer la fonction CDF de la loi normale standard
    double normalCDF(double x) const;
};

#endif // BLACK_SCHOLES_MODEL_H
