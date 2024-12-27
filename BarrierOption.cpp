#include "BarrierOption.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

BarrierOption::BarrierOption(double strike_, double maturity_, double barrier_, BarrierType barrierType_, OptionType optionType_)
    : ExoticOption(strike_, maturity_), barrier(barrier_), barrierType(barrierType_), optionType(optionType_) {}

double BarrierOption::payoff(double spot) const {
    if (optionType == OptionType::Call) {
        return std::max(spot - strike, 0.0); // Payoff pour un call
    } else { // OptionType::Put
        return std::max(strike - spot, 0.0); // Payoff pour un put
    }
}

bool BarrierOption::isBarrierTouched(const std::vector<double>& path) const {
    switch (barrierType) {
        case BarrierType::UpAndOut:
        case BarrierType::UpAndIn:
            return std::any_of(path.begin(), path.end(), [this](double s) { return s >= barrier; });
        case BarrierType::DownAndOut:
        case BarrierType::DownAndIn:
            return std::any_of(path.begin(), path.end(), [this](double s) { return s <= barrier; });
        default:
            return false;
    }
}

double BarrierOption::price(const BlackScholesModel& model, int numPaths, int steps) const { // Surcharge de la méthode price pour prendre la maturité de l'option par défaut
    return this->price(model, numPaths, steps, maturity);
}

double BarrierOption::price(const BlackScholesModel& model, int numPaths, int steps, double adjustedMaturity) const {
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

        // Vérifier si la barrière est franchie
        bool touched = isBarrierTouched(path);

        if ((barrierType == BarrierType::UpAndOut || barrierType == BarrierType::DownAndOut) && touched) {
            sumPayoffs += 0.0; // Option invalide
        } else if ((barrierType == BarrierType::UpAndIn || barrierType == BarrierType::DownAndIn) && !touched) {
            sumPayoffs += 0.0; // Option n'existe pas si la barrière n'est pas franchie
        } else {
            sumPayoffs += payoff(path.back()); // Calculer le payoff standard
        }
    }

    return std::exp(-model.rate * adjustedMaturity) * (sumPayoffs / numPaths);
}

double BarrierOption::hedgeCost(const BlackScholesModel& model, int steps) const {
    int numPaths = 10000;
    double epsilon = 0.01 * model.spot;

    // Utiliser la méthode `price` pour S + epsilon et S - epsilon
    BlackScholesModel modelUp = model;
    modelUp.spot += epsilon;
    double priceUp = this->price(modelUp, numPaths, steps);

    BlackScholesModel modelDown = model;
    modelDown.spot -= epsilon;
    double priceDown = this->price(modelDown, numPaths, steps);

    double previousDelta = 0;
    // Calcul du delta par dérivée centrée
    double delta = (priceUp - priceDown) / (2 * epsilon);

    // Réplication dynamique
    double dt = maturity / steps;
    double spot = model.spot;
    double cash = delta*spot;
    // Pour sauvegarder la trajectoire
    std::vector<double> path;
    path.push_back(spot);

    // Vérification de la barrière après la boucle
    bool barrierTouched = isBarrierTouched(path);
    for (int i = 1; i < steps; ++i) {

        // Maturité ajustée
        double adjustedMaturity = maturity - i * dt;

        // Simuler le prix du sous-jacent
        double drift = (model.rate - model.dividend - 0.5 * model.volatility * model.volatility) * dt;
        double diffusion = model.volatility * std::sqrt(dt) * ((double)rand() / RAND_MAX - 0.5);
        spot *= std::exp(drift + diffusion);

        // Ajuster la position
        previousDelta = delta;


        // Ajouter le spot actuel au chemin
        path.push_back(spot);
        barrierTouched = isBarrierTouched(path);
        if (barrierTouched){delta = 0;
        }else{
        modelUp.spot = spot + epsilon;
        modelDown.spot = spot - epsilon;
        priceUp = this->price(modelUp, numPaths, steps - i, adjustedMaturity);
        priceDown = this->price(modelDown, numPaths, steps - i, adjustedMaturity);
        delta = (priceUp - priceDown) / (2 * epsilon);
        }
        cash += (delta - previousDelta) * spot;
        cash *= std::exp(model.rate * dt);
        /*std::cout << "Delta : " << delta;
        std::cout << "Spot : " << spot;
        std::cout << "Cash : " << cash;*/
    }




        // Soustraction conditionnelle du payoff
    double finalPayoff = payoff(spot);
    if ((barrierType == BarrierType::UpAndIn || barrierType == BarrierType::DownAndIn) && barrierTouched) {
        cash += finalPayoff;
    } else if ((barrierType == BarrierType::UpAndOut || barrierType == BarrierType::DownAndOut) && !barrierTouched) {
        cash += finalPayoff;
    }
    return cash - delta * spot;

}
