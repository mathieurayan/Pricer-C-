#ifndef OPTION_H
#define OPTION_H

// D�claration forward de la classe BlackScholesModel
class BlackScholesModel;

// Classe abstraite Option
class Option {
public:
    double strike;   // Prix d'exercice
    double maturity; // Maturit� de l'option (en ann�es)

    // Constructeur
    Option(double strike_, double maturity_);

    // M�thode virtuelle pure pour calculer le payoff
    virtual double payoff(double spot) const = 0;

    // M�thode virtuelle pure pour la r�plication (bas�e sur Black-Scholes)
    virtual double hedgeCost(const BlackScholesModel& model, int steps) const = 0;

    // Destructeur virtuel
    virtual ~Option();
};

#endif // OPTION_H
