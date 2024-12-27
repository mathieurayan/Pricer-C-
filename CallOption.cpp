#include "CallOption.h"
#include "BlackScholesModel.h" // Nécessaire pour accéder aux membres de BlackScholesModel
#include <cmath> // Pour std::max
#include <algorithm> // Inclure pour std::max
#include <iostream>
CallOption::CallOption(double strike_, double maturity_)
    : Option(strike_, maturity_) {}

double CallOption::payoff(double spot) const {
    return std::max(spot - strike, 0.0);
}

double CallOption::hedgeCost(const BlackScholesModel& model, int steps) const {
    double dt = maturity / steps;
    double spot = model.spot;
    double previousDelta = 0.0;

        // Calcul du delta initial
    double d1 = (std::log(spot / strike) +
            (model.rate - model.dividend + 0.5 * model.volatility * model.volatility) * (maturity)) /
            (model.volatility * std::sqrt(maturity));
    double currentDelta = std::exp(-model.dividend * (maturity)) * 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
    double cash = currentDelta*spot;

    for (int i = 1; i <= steps; ++i) {
        double time = i * dt;
        // Simulation du prix du sous-jacent
        if (i < steps) {
            double drift = (model.rate - model.dividend - 0.5 * model.volatility * model.volatility) * dt;
            double diffusion = model.volatility * std::sqrt(dt) * ((double)rand() / RAND_MAX - 0.5);
            spot *= std::exp(drift + diffusion);
        }
        previousDelta = currentDelta;
        // Calcul du delta à l'instant t
        d1 = (std::log(spot / strike) +
                     (model.rate - model.dividend + 0.5 * model.volatility * model.volatility) * (maturity - time)) /
                    (model.volatility * std::sqrt(maturity - time));
        currentDelta = std::exp(-model.dividend * (maturity - time)) * 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
        // Ajustement du portefeuille
        cash += (currentDelta - previousDelta) * spot;
        cash *= std::exp(model.rate * dt); // Actualisation du cash


    }

    return cash - currentDelta * spot + payoff(spot);
}
