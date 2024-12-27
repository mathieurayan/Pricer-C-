#include "BlackScholesModel.h"
#include <iostream>

// Constructeur
BlackScholesModel::BlackScholesModel(double spot_, double rate_, double volatility_, double dividend_)
    : spot(spot_), rate(rate_), volatility(volatility_), dividend(dividend_) {}

// Méthode pour le calcul analytique des options vanilles
double BlackScholesModel::priceAnalytic(const Option *option, bool isCall) const {
    double d1 = (std::log(spot / option->strike) +
                 (rate - dividend + 0.5 * volatility * volatility) * option->maturity) /
                (volatility * std::sqrt(option->maturity));
    double d2 = d1 - volatility * std::sqrt(option->maturity);

    double Nd1 = normalCDF(d1);
    double Nd2 = normalCDF(d2);
    double Nmd1 = normalCDF(-d1);
    double Nmd2 = normalCDF(-d2);
    if (isCall) {
        return spot * std::exp(-dividend * option->maturity) * Nd1 -
               option->strike * std::exp(-rate * option->maturity) * Nd2;
    } else {
        return option->strike * std::exp(-rate * option->maturity) * Nmd2 -
               spot * std::exp(-dividend * option->maturity) * Nmd1;
    }
}

// Méthode privée pour la CDF de la loi normale standard
double BlackScholesModel::normalCDF(double x) const {
    return 0.5 * std::erfc(-x * M_SQRT1_2); // M_SQRT1_2 = 1 / sqrt(2)
}
