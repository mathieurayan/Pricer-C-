#include "BlackScholesModel.h"
#include "CallOption.h"
#include "PutOption.h"
#include "BarrierOption.h"
#include "AsianOption.h"
#include <iostream>
#include <memory>
#include <vector>

void displayMenu() {
    std::cout << "\n--- Menu des options disponibles ---\n";
    std::cout << "1. Call Option\n";
    std::cout << "2. Put Option\n";
    std::cout << "3. Barrier Option (Up-and-Out)\n";
    std::cout << "4. Barrier Option (Down-and-Out)\n";
    std::cout << "5. Barrier Option (Up-and-In)\n";
    std::cout << "6. Barrier Option (Down-and-In)\n";
    std::cout << "7. Asian Option (Call)\n";
    std::cout << "8. Asian Option (Put)\n";
    std::cout << "0. Quitter\n";
}

int main() {
    // Modèle de base Black-Scholes
    double spot, rate, volatility, dividend;
    std::cout << "Entrez les paramètres du modèle Black-Scholes :\n";
    std::cout << "Spot price (S0) : ";
    std::cin >> spot;
    std::cout << "Risk-free rate (r) : ";
    std::cin >> rate;
    std::cout << "Volatility (sigma) : ";
    std::cin >> volatility;
    std::cout << "Dividend yield (q) : ";
    std::cin >> dividend;

    BlackScholesModel model(spot, rate, volatility, dividend);

    while (true) {
        displayMenu();
        int choice;
        std::cout << "Choisissez une option : ";
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Merci d'avoir utilisé le programme.\n";
            break;
        }

        // Paramètres génériques pour toutes les options
        double strike, maturity;
        std::cout << "Entrez le strike (K) : ";
        std::cin >> strike;
        std::cout << "Entrez la maturité (T, en années) : ";
        std::cin >> maturity;

        int numPaths = 10000;  // Nombre de simulations Monte-Carlo
        int steps = 100;       // Nombre de pas temporels

        if (choice == 1) {
            // Call Option
            CallOption callOption(strike, maturity);
            double price = model.priceAnalytic(&callOption, true);
            double hedgeCost = callOption.hedgeCost(model, steps);
            std::cout << "Prix du call option : " << price << "\n";
            std::cout << "Coût de réplication : " << hedgeCost << "\n";

        } else if (choice == 2) {
            // Put Option
            PutOption putOption(strike, maturity);
            double price = model.priceAnalytic(&putOption, false);
            double hedgeCost = putOption.hedgeCost(model, steps);
            std::cout << "Prix du put option : " << price << "\n";
            std::cout << "Coût de réplication : " << hedgeCost << "\n";

        } else if (choice >= 3 && choice <= 6) {
            // Barrier Options
            double barrier;
            std::cout << "Entrez le niveau de la barrière (B) : ";
            std::cin >> barrier;

            // Demander le type de l'option (Call ou Put)
            OptionType optionType;
            int optionTypeChoice;
            std::cout << "Type d'option : 1 pour Call, 2 pour Put : ";
            std::cin >> optionTypeChoice;
            if (optionTypeChoice == 1) {
                optionType = OptionType::Call;
            } else if (optionTypeChoice == 2) {
                optionType = OptionType::Put;
            } else {
                std::cout << "Choix invalide pour le type d'option.\n";
                continue;
            }

            BarrierType barrierType;
            switch (choice) {
                case 3:
                    barrierType = BarrierType::UpAndOut;
                    break;
                case 4:
                    barrierType = BarrierType::DownAndOut;
                    break;
                case 5:
                    barrierType = BarrierType::UpAndIn;
                    break;
                case 6:
                    barrierType = BarrierType::DownAndIn;
                    break;
            }

            BarrierOption barrierOption(strike, maturity, barrier, barrierType, optionType);
            double price = barrierOption.price(model, numPaths, steps);
            double hedgeCost = barrierOption.hedgeCost(model,steps);
            std::cout << "Prix de l'option barrière : " << price << "\n";
            std::cout << "Coût de réplication : " << hedgeCost << "\n";

        } else if (choice == 7 || choice == 8) {
            // Asian Options
            OptionType optionType = (choice == 7) ? OptionType::Call : OptionType::Put;

            AsianOption asianOption(strike, maturity, optionType);
            double price = asianOption.price(model, numPaths, steps);
            double hedgeCost = asianOption.hedgeCost(model, steps);
            std::cout << "Prix de l'option asiatique : " << price << "\n";
            std::cout << "Coût de réplication : " << hedgeCost << "\n";

        } else {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        }
    }

    return 0;
}
