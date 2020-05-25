#include <iostream>
#include<vector>
#include<fstream>
#include<string>

enum class Pclass { First = 1, Second, Third };
enum class Sex { Male, Female };
enum Embarked { C, Q, S };

struct Passenger {
    int id;
    bool survival;
    Pclass pclass;
    std::string name;
    Sex sex;
    double age;
    short int sibsp,
        parch;
    std::string ticket;
    double fare;
    std::string cabin;
    Embarked embarked;
};

std::vector<Passenger> passengers;
std::vector<int> children;

std::string readuntil(std::ifstream& fin, char end = '\r') {
    std::string s = "";
    char buffer;
    while (fin.read(&buffer, 1)) {
        if (buffer != end) s += buffer;
        else break;
    }
    return s;
}

int end = -1; // сохраняет последнюю позицию в строке
int lastEndSize = 1; // сохраняет кол-во знаков

std::string readuntilstr(std::string str, std::string endStr) {
    int begin = end + lastEndSize;
    end = str.find(endStr, begin);
    lastEndSize = endStr.size();
    return str.substr(begin, end - begin);
}

std::string shtatw(Embarked max) {
    if (max == 0)  return "C";
    else if (max == 1) return "Q";
    else return "S";
}
int main()
{
    setlocale(LC_ALL, "Russian");
    std::ifstream fin ("train.csv");

    if (!fin)
    {
        std::cout << "Uh oh, something went wrong!";
        exit(1);
    }

    std::ofstream fout("вывод.txt");
    std::string line;
    readuntil(fin, '\r');

    while (true) {

        end = -1;

        line = readuntil(fin, '\r');
        if (line == "") break;
        Passenger p;
        p.id = std::stoi(readuntilstr(line, ","));
        p.survival = std::stoi(readuntilstr(line, ","));
        p.pclass = (Pclass)std::stoi(readuntilstr(line, ",\""));
        p.name = readuntilstr(line, "\",");
        p.sex = readuntilstr(line, ",") == "male" ? Sex::Male : Sex::Female;

        std::string ag = readuntilstr(line, ",");
        if (ag == "") p.age = -1;
        else p.age = std::stod(ag);

        p.sibsp = std::stoi(readuntilstr(line, ","));
        p.parch = std::stoi(readuntilstr(line, ","));
        p.ticket = readuntilstr(line, ",");
        p.fare = std::stod(readuntilstr(line, ","));
        p.cabin = readuntilstr(line, ",");

        std::string em = readuntilstr(line, ",");
        if (em == "C") p.embarked = Embarked::C;
        else if (em == "Q") p.embarked = Embarked::Q;
        else p.embarked = Embarked::S;

        passengers.push_back(p);
    }

    int calive = 0,
        fclass = 0,
        sclass = 0,
        thclass = 0,
        female = 0,
        male = 0;
    double average_age = 0;
    int female_all = 0,
        male_all = 0;
    double average_female = 0, // сред. жен. возраст
        average_male = 0; // сред. муж. возраст
    int shtat[3]{};
    int age_all = 0;
    Embarked shtat_max = C;

    for (int i = 0; i < passengers.size(); i++) {
        if (passengers[i].survival) {
            calive++;
            if (passengers[i].pclass == Pclass::First) fclass++;
            else if (passengers[i].pclass == Pclass::Second) sclass++;
            else thclass++;

            if (passengers[i].sex == Sex::Female) female++;
            else male++;
        };
        if (passengers[i].age != -1) {
            average_age += passengers[i].age;
            age_all++;
            if (passengers[i].sex == Sex::Female) {
                female_all++;
                average_female += passengers[i].age;
            }
            else {
                male_all++;
                average_male += passengers[i].age;
            }
        }

        shtat[passengers[i].embarked]++;

        if (passengers[i].age < 18 && passengers[i].age != -1) children.push_back(passengers[i].id);
    }
    average_age /= age_all;
    average_female /= female_all;
    average_male /= male_all;

    if (shtat[shtat_max] < shtat[Q]) shtat_max = Q;
    if (shtat[shtat_max] < shtat[S]) shtat_max = S;

    fout << "Общее число выживших:" << calive << '\n'
        << "Число выживших из 1 класса:" << fclass << '\n'
        << "Число выживших из 2 класса:" << sclass << '\n'
        << "Число выживших из 3 класса:" << thclass << '\n'
        << "Количество выживших женщин:" << female << '\n'
        << "Количество выживших мужчин:" << male << '\n'
        << "Средний возраст пассажира:" << average_age << '\n'
        << "Средний женский возраст:" << average_female << '\n'
        << "Средний мужской возраст:" << average_male << '\n'
        << "Штат, в котором село больше всего пассажиров:" << shtatw(shtat_max) << '\n'
        << "Список идентификаторов пассажиров младше 18:" << '\n';

    for (int i = 0; i < children.size(); i++) {
        if (i == children.size() - 1) {
            fout << children[i];
            break;
        }

        fout << children[i] << ",";
    }

    fout.close();
    fin.close();
}