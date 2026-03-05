#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

class ElevatorProblem {

    private :

    std::random_device rd;
    std::mt19937 gen{rd()};
	std::uniform_int_distribution<int> distrib{1, 20};

    public :

    bool BreakDown() {
        if (distrib(gen) == 20) {
            return true;
        }

        else {
            return false;
        }
    }

};


class Elevator {

    private :

    std::string m_name_elev;
    int m_lv_min, m_lv_max;
    std::vector<int> history_floor;

    public :

    ElevatorProblem problem;

    Elevator(const std::string name_elev, const int lv_min, const int lv_max)
    : m_name_elev(name_elev), m_lv_min(lv_min), m_lv_max(lv_max) {}

    int CalculateGap(int a, int b) {
        int gap = std::max(a, b) - std::min(a, b);
        return gap;
    }

    void HistoryView() {

        for (int i = 1; i < history_floor.size(); i++) {
            std::cout << "Trajet " << i << " : " << history_floor[i - 1] << " -> " << history_floor[i] << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        MainElevator();
    }

    void MainElevator() {
        bool want_to_use = true;
        int starting_floor = 0;
        int arrival_floor = 0;
        
        std::cout << "De quel etage partez-vous ? : ";
        std::cin >> starting_floor;
        history_floor.push_back(starting_floor);

        while (want_to_use == true) {

            std::cout << "\nA quel etage voulez-vous aller ? (500 pour quitter, 400 pour l'historique) : ";
            std::cin >> arrival_floor;   

            if (arrival_floor == 500) {
                want_to_use = false;
                break;
            }

            else if (arrival_floor == 400) {
                HistoryView();
            }

            while (starting_floor == arrival_floor) {
                std::cout << "\nVeuillez entrer un etage different de l'actuel";
                std::cout << "\nA quel etage voulez-vous aller ? (500 pour quitter, 400 pour l'historique) : ";
                std::cin >> arrival_floor;  
            }

            history_floor.push_back(arrival_floor);

            std::cout << "Ok, c'est parti direction l'etage numero " << arrival_floor << ". Attention nous allons ";

            int gap_ = CalculateGap(starting_floor, arrival_floor);

            if (starting_floor < arrival_floor) {
                std::cout << "monter de " << gap_ << " etage(s) ..." << std::endl;
            }
            else {
                std::cout << "descendre de " << gap_ << " etage(s) ..." << std::endl;
            }

            if (problem.BreakDown() == true) {
                std::cout << "\n... Eh Bah NAN ! L'ascenseur '" << m_name_elev << "' a un petit soucis. Le depanneur va s'en charger mais il va falloir attendre. Please WAIT.";
                std::this_thread::sleep_for(std::chrono::seconds(8));
                continue;
            }


            if (starting_floor < arrival_floor) {
                for (int i = starting_floor; i <= arrival_floor; i++) {
                    std::cout << "Etage numero " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }
            }
            else {
                for (int i = starting_floor; i >= arrival_floor; i--) {
                    
                    std::cout << "Etage numero " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }
            }
            starting_floor = arrival_floor;
        }

    }
    
};

std::tuple<std::string, int, int> InitElevator() {
    std::string name;
    int min_, max_;

    std::cout << "Bienvenue dans le configurateur de votre ascenseur !";
    std::cout << "\nComment voulez-vous appeller votre ascenseur ? : ";
    std::getline(std::cin, name);
    std::cout << "\nQuel est l'etage minimum (> -100) de l'ascenseur '" << name << "' ? : ";
    std::cin >> min_;
    std::cout << "\nQuel est l'etage maximum (< 100) de l'ascenseur '" << name << "' ? : ";
    std::cin >> max_;
    std::cout << "\nVotre ascenseur a ete configure avec succes !" << std::endl;

    return std::make_tuple(name, min_, max_);
}


int main() {
    auto elevator_tuple = InitElevator();

    std::string name_elev_ = std::get<0>(elevator_tuple);
    int lv_min_ = std::get<1>(elevator_tuple);
    int lv_max_ = std::get<2>(elevator_tuple);

    Elevator elevator(name_elev_, lv_min_, lv_max_);
    elevator.MainElevator();

    return 0;
}