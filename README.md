МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  

Федеральное государственное автономное образовательное учреждение высшего образования  

"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  

ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  

Кафедра компьютерной инженерии и моделирования
<br/><br/>
### Отчёт по лабораторной работе № 5<br/> по дисциплине "Программирование"
<br/>
​Cтудента 1 курса группы ПИ-б-о-192(1)<br/>
Гусько Елизавета Викторовна<br/>
направления подготовки 09.03.04 "Программная инженерия"  
<br/>


<br/>
<table>

<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>

<td>(оценка)</td>

<td>Чабанов В.В.</td>

</tr>

</table>

<br/><br/>

​

Симферополь, 2020

<br/>

# Лабораторная работа №5

# Работа с текстовыми файлами

### ***Цель***: 
1. Научиться работать с текстовыми файлами;
2. Закрепить навыки работы со структурами.

<br/>

### Ход работы:

1. Скачать необходимые файлы из репозитория: train.csv , data_dictionary.txt
2. По файлу data_dictionary.txt реализуем структуру.

``` c++
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
```
[Файл с данными](вывод.txt)

<table>
<tr><td> общее число выживших: 
<td> 342 
<tr><td> Число выживших из 1 класса: 
<td> 136 
<tr><td> Число выживших из 2 класса:
<td> 87 
<tr><td> Число выживших из 3 класса: 
<td> 119 
<tr><td> Количество выживших женщин: 
<td> 233 
<tr><td> Количество выживших мужчин: 
<td> 109 
<tr><td> Средний возраст пассажира: 
<td> 29.6793 
<tr><td> Средний женский возраст: 
<td> 27.9042 
<tr><td> Средний мужской возраст: 
<td> 30.702 
<tr><td> Штат, в котором село больше всего пассажиров: 
<td> S 
<tr><td> Список идентификаторов пассажиров младше 18:
<td> 8,10,11,15,17,23,25,40,44,51,59,60,64,69,72,79,85,87,112,115,120,126,139,148,157,164,165,166,172,173,183,184,185,194,206,209,221,234,238,262,267,279,283,298,306,308,330,334,341,349,353,375,382,387,390,408,420,434,436,446,447,449,470,480,481,490,501,505,531,533,536,542,543,550,551,575,619,635,643,645,684,687,690,692,721,722,732,747,751,752,756,765,778,781,782,788,789,792,803,804,814,820,825,828,831,832,842,845,851,853,854,870,876 
</tr>

</table>



### Вывод: в ходе выполнения лабораторной работы я получила навыки работы с csv-файлами и текстовыми файлами.

