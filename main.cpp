#include <iostream> //Библиотека для ввода и вывода данных в консоль (cout, cin, endl)
#include <iomanip> //Библиотека для управления форматированием ввода-вывода (fixed и setprecision)
#include <chrono> //Библиотека для работы с временем и подсчёта интервалов времени (duration, high_resolution_clock, now, count)
#include <cmath> //Библиотека для работы с математическими функциями (sin, cos, pow)

//Прототипы функций
double Fun();
double MDP();
double MZS();
double MPP();

//Функция для подсчёта данной функции y = x^2 * cos(x) + 2sin(x)
double Fun(double x) {
    return pow(x, 2) * cos(x) + 2.0 * sin(x);
}

//Функция метода деления отрезка пополам
double MDP(double a, double b, double e) {
    double x1, x2, y1, y2;

    do {
        x1 = (a + b - e) / 2.0;
        x2 = (a + b + e) / 2.0;
        y1 = Fun(x1);
        y2 = Fun(x2);
        if (y1 > y2) {
            a = x1;
        }
        else {
            b = x2;
        }

    } while ((b - a) > 2.0 * e);
    return (a + b) / 2.0;

}

//Функция метода золотого сечения
double MZS(double a, double b, double e) {
    double x1, x2, y1, y2, psi = 0.381966011;

    x1 = a + psi * (b - a);
    x2 = b - psi * (b - a);
    y1 = Fun(x1);
    y2 = Fun(x2);
    do {
        if (y1 > y2) {
            a = x1;
            x1 = x2;
            y1 = y2;
            x2 = b - psi * (b - a);
            y2 = Fun(x2);
        }
        else {
            b = x2;
            x2 = x1;
            y2 = y1;
            x1 = a + psi * (b - a);
            y1 = Fun(x1);
        }
    } while ((b - a) > e);
    return (a + b) / 2.0;

}

//Функция метода последовательного перебора
double MPP(double x0, double h, double e) {
    do {
        if (Fun(x0) < Fun(x0 + h)) {
            x0 -= h;
            h /= 4;
            while (Fun(x0) < Fun(x0 + h)) {
                x0 -= h;
            }
        }
        if (!(Fun(x0) < Fun(x0 + h))){
            x0 += h;
            h /= 2;
            while (!(Fun(x0) < Fun(x0 + h))) {
                x0 += h;
            }
        }
    } while (h > e / 4);

    return x0;
}


int main() {
    setlocale(LC_ALL, "ru"); //Установка русской кодировки

    //Обявление переменных
    double a, b, x0, h, e;
    double mdp[3]{}, mzs[3]{}, mpp[3]{}; //Массивы, хранящие минимумы каждого метода
    std::chrono::duration<double> smdp[3]{}, smzs[3]{}, smpp[3]{}; //Массивы хранящие классы, нужные для вывода времени
    std::cout
            << "Finding the minima of the function by MDP, MZS, MPP"
            << std::endl;

    //Ввод a и b (пределы интервала)
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter b (b > a): ";
    std::cin >> b;
    if (a > b) { //Если пользователь ввёл отрицательный интервал, то программа меняет а и b
        double temp = a;
        a = b;
        b = temp;
        std::cout
                << "Replacement"
                << std::endl;
    }
    double step = (b - a) / 3; //Нахождение шага, нужного для разделения интервала на три равные части

    //Ввод шага h для метода последовательного перебора
    do {
        std::cout << "Enter h (0 <= h <= 1): ";
        std::cin >> h;
        if (h <= 0 and h >= 1) {
            std::cout
                    << "Error! Repeat entering!"
                    << std::endl;
        }
    } while (h <= 0 and h >= 1);


    //Ввод е (точности вычислений)
    do {
        std::cout << "Enter e: ";
        std::cin >> e;
        if (e <= pow(10, -14) or e >= pow(10, 1)) {
            std::cout
                    << "Error! Repeat entering!"
                    << std::endl;
        }
    } while (e <= pow(10, -14) or e >= pow(10, 1));

    //Вывод исходных данных
    std::cout
            << std::endl
            << "a = " << a
            << " b = " << b
            << " h = " << h
            << " e = " << e
            << std::endl << std::endl;

    b = a + step; //Нахождение первого интервала
    for (int i = 0; i < 3; i++) { //Вычисление минимума функции каждым методом на всех интервалах

        //Создание и инициализация переменной start, хранящая момент времени в начале выполнения метода
        auto start = std::chrono::high_resolution_clock::now();
        //Вызов MDP
        mdp[i] = MDP(a, b, e);
        //Создание и инициализация переменной end, хранящая момент времени в конце выполнения метода
        auto end = std::chrono::high_resolution_clock::now();
        //Вычисление разницы во времени между началом выполнения метода и завершением выполнения метода
        smdp[i] = end - start;

        start = std::chrono::high_resolution_clock::now();
        //Вызов MZS
        mzs[i] = MZS(a, b, e);
        end = std::chrono::high_resolution_clock::now();
        smzs[i] = end - start;

        x0 = (a + b) / 2; //Установка х0 по середине интервала
        start = std::chrono::high_resolution_clock::now();
        //Вызов MPP
        mpp[i] = MPP(x0, h, e);
        end = std::chrono::high_resolution_clock::now();
        smpp[i] = end - start;

        //Переход к следующему интервалу
        a += step;
        b += step;
    }

    //Возращение к первому интервалу
    a -= 3 * step;
    b -= 3 * step;

    //Вывод результатов выполнения программы
    std::cout
            << "\033[1;35m"
            << "MDP\t\t\tMZS\t\t\tMPP"
            << std::endl << std::endl;

    for (int i = 0; i < 3; i++, a += step, b += step) {

        std::cout
                << std::fixed << std::setprecision(5)
                << "\033[1;32m"
                << "x" << i + 1 << " = "<< mdp[i] << "\t\t"
                << "x" << i + 1 << " = " << mzs[i] << "\t\t"
                << "x" << i + 1 << " = " << mpp[i]
                << std::endl;

        std::cout
                << "\033[1;31m"
                << "y" << i + 1 << " = " << Fun(mdp[i]) << "\t\t"
                << "y" << i + 1 << " = " << Fun(mzs[i]) << "\t\t"
                << "y" << i + 1 << " = " << Fun(mpp[i])
                << std::endl;

        x0 = (a + b) / 2;

        std::cout
                << std::fixed << std::setprecision(0)
                << "\033[1;33m"
                << "a = " << a << "\t\t\t"
                << "a = " << a << "\t\t\t"
                << "x0 = " << x0
                << std::endl;

        std::cout
                << "b = " << b << "\t\t\t"
                << "b = " << b << "\t\t\t"
                << "       "
                << std::endl;

        std::cout
                << std::fixed << std::setprecision(1)
                << "\033[1;34m"
                << "time = " << smdp[i].count() * pow(10,6) << " mks" << "\t\t"
                << "time = " << smzs[i].count() * pow(10, 6) << " mks" << "\t\t"
                << "time = " << smpp[i].count() * pow(10, 6) << " mks"
                << std::endl << std::endl
                << "\033[1;37m";

    }
    return 0;
}