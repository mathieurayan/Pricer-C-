#include "AsianOption.h"
#include <numeric>  // Pour std::accumulate
#include <cmath>    // Pour std::exp
#include <random>   // Pour std::mt19937 et std::normal_distribution
#include <stdexcept>  // Pour std::logic_error

AsianOption::AsianOption(double strike_, double maturity_, OptionType optionType_)
    : ExoticOption(strike_, maturity_), optionType(optionType_) {}

double AsianOption::payoff(const std::vector<double>& path) const {
    double average = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
    if (optionType == OptionType::Call) {
        return std::max(average - strike, 0.0);
    } else { // OptionType::Put
        return std::max(strike - average, 0.0);
    }
}

// Implémentation de la méthode inutilisée
double AsianOption::payoff(double spot) const {
    throw std::logic_error("payoff(double spot) is not applicable for AsianOption.");
}

double AsianOption::price(const BlackScholesModel& model, int numPaths, int steps) const { // Surcharge de la méthode price pour prendre la maturité de l'option par défaut
    return this->price(model, numPaths, steps, maturity);
}

double AsianOption::price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const {
    double sumPayoffs = 0.0;
    double dt = adjustedMaturity / steps;

    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<> dist(0.0, 1.0);

    for (int i = 0; i < numPaths; ++i) {
        double spot = model.spot;
        std::vector<double> path;
        path.reserve(steps);

        for (int j = 0; j < steps; ++j) {
            double drift = (model.rate - model.dividend - 0.5 * model.volatility * model.volatility) * dt;
            double diffusion = model.volatility * std::sqrt(dt) * dist(rng);
            spot *= std::exp(drift + diffusion);
            path.push_back(spot);
        }

        sumPayoffs += payoff(path);
    }

    return std::exp(-model.rate * maturity) * (sumPayoffs / numPaths);
}

double AsianOption::hedgeCost(const BlackScholesModel& model,int steps) const {
    int numPaths = 10000;
    double epsilon = 0.01 * model.spot;

    // Initialisation du delta
    BlackScholesModel modelUp = model;
    modelUp.spot += epsilon;
    BlackScholesModel modelDown = model;
    modelDown.spot -= epsilon;

    double priceUp = this->price(modelUp, numPaths, steps);
    double priceDown = this->price(modelDown, numPaths, steps);

    double previousDelta = 0;
    double delta = (priceUp - priceDown) / (2 * epsilon);

    // Réplication dynamique
    double dt = maturity / steps;
    double spot = model.spot;
    double cash = delta*spot;
    // Pour sauvegarder la trajectoire
    std::vector<double> path;
    path.push_back(spot);

    for (int i = 1; i < steps; ++i) {

        // Maturité ajustée
        double adjustedMaturity = maturity - i * dt;

        // Simulation du prix du sous-jacent
        double drift = (model.rate - model.dividend - 0.5 * model.volatility * model.volatility) * dt;
        double diffusion = model.volatility * std::sqrt(dt) * ((double)rand() / RAND_MAX - 0.5);
        spot *= std::exp(drift + diffusion);

        // Ajouter le spot actuel au chemin
        path.push_back(spot);

        // Calcul du nouveau delta
        double previousDelta = delta;

        modelUp.spot = spot + epsilon;
        modelDown.spot = spot - epsilon;

        priceUp = this->price(modelUp, numPaths, steps - i);
        priceDown = this->price(modelDown, numPaths, steps - i);

        delta = (priceUp - priceDown) / (2 * epsilon);

        // Ajustement du portefeuille
        cash += (delta - previousDelta) * spot;
        cash *= std::exp(model.rate * dt);


    }

    // Retourner le coût total de réplication
    return cash - delta * spot + payoff(path);
}
