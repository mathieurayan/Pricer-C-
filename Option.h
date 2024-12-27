#ifndef OPTION_H
#define OPTION_H

// Déclaration forward de la classe BlackScholesModel
class BlackScholesModel;

// Classe abstraite Option
class Option {
public:
    double strike;   // Prix d'exercice
    double maturity; // Maturité de l'option (en années)

    // Constructeur
    Option(double strike_, double maturity_);

    // Méthode virtuelle pure pour calculer le payoff
    virtual double payoff(double spot) const = 0;

    // Méthode virtuelle pure pour la réplication (basée sur Black-Scholes)
    virtual double hedgeCost(const BlackScholesModel& model, int steps) const = 0;

    // Destructeur virtuel
    virtual ~Option();
};

#endif // OPTION_H
