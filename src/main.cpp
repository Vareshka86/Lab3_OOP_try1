/**
 * @file main.cpp
 * @brief Точка входа: меню калькулятора многочленов второй степени.
 * @details Программа хранит два многочлена — P и Q — и выполняет с ними
 * действия из меню. Лабораторная работа выполняется поэтапно:
 * - v0.1 — часть 1: класс Polynomial — конструкторы, деструктор, статические
 *   поля, вывод, значение в точке, поиск корней и счётчик поисков;
 * - v0.2 — часть 2: перегрузка унарных операций и арифметики;
 * - v1.0 — часть 3: перегрузка сравнений, демонстрация всех операций.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "input.h"
#include "Polynomial.h"

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // подключать только основную часть WinAPI
#include <windows.h>        // SetConsoleOutputCP, SetConsoleCP
#endif

/// Текущая версия программы (совпадает с меткой версии в git).
constexpr const char* PROGRAM_VERSION = "v0.1";

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/**
 * @brief Настраивает консоль Windows на кодировку UTF-8.
 * @details Исходные файлы сохранены в UTF-8. Без этой настройки
 * русский текст в консоли Windows выводится «кракозябрами».
 * В Linux/macOS консоль уже работает в UTF-8, поэтому там функция
 * ничего не делает (код внутри `#ifdef _WIN32` не компилируется).
 */
void setupConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

/**
 * @brief Запрашивает коэффициенты и создаёт многочлен.
 * @param name Имя многочлена для подсказки («P» или «Q»).
 * @return Новый многочлен, созданный параметризованным конструктором.
 */
Polynomial readPolynomial(const std::string& name)
{
    std::cout << "Многочлен " << name << " = ax² + bx + c\n";
    const double a = readDouble("  a = ");
    const double b = readDouble("  b = ");
    const double c = readDouble("  c = ");
    return Polynomial(a, b, c);
}

/**
 * @brief Находит и выводит корни многочлена.
 * @param name Имя многочлена.
 * @param p    Многочлен.
 */
void printRoots(const std::string& name, const Polynomial& p)
{
    double x1 = 0.0;
    double x2 = 0.0;
    const int count = p.findRoots(x1, x2);

    std::cout << "Корни уравнения " << p << " = 0 (многочлен " << name << "): ";
    if (count == Polynomial::INFINITE_ROOTS)
    {
        std::cout << "любое число (0 = 0)\n";
    }
    else if (count == 0)
    {
        std::cout << "действительных корней нет\n";
    }
    else if (count == 1)
    {
        std::cout << "x = " << x1 << '\n';
    }
    else
    {
        std::cout << "x1 = " << x1 << ", x2 = " << x2 << '\n';
    }
}

/**
 * @brief Выводит меню и текущие многочлены.
 * @param p Многочлен P.
 * @param q Многочлен Q.
 */
void printMenu(const Polynomial& p, const Polynomial& q)
{
    std::cout << "\n==================================================\n"
              << " Калькулятор многочленов 2-й степени (" << PROGRAM_VERSION << ")\n"
              << "==================================================\n"
              << "  P = " << p << '\n'
              << "  Q = " << q << '\n'
              << "  Многочленов в памяти: " << Polynomial::getExistingCount() << '\n'
              << "--------------------------------------------------\n"
              << "  1 - ввести P\n"
              << "  2 - ввести Q\n"
              << "  3 - обнулить P (конструктор по умолчанию)\n"
              << "  4 - скопировать P в Q (конструктор копирования)\n"
              << "  5 - значения P(x) и Q(x)\n"
              << "  6 - корни P\n"
              << "  7 - корни Q\n"
              << "  8 - сколько раз искали корни\n"
              << "  0 - выход\n";
}

} // namespace

/**
 * @brief Главная функция: меню калькулятора.
 * @details Исключения (например, закончился поток ввода) перехватываются здесь,
 * чтобы программа завершилась с понятным сообщением.
 * @return 0 — при нормальном завершении, 1 — при ошибке.
 */
int main()
{
    setupConsole();

    try
    {
        Polynomial p(1.0, -3.0, 2.0); // x² - 3x + 2: корни 1 и 2
        Polynomial q;                 // нулевой многочлен

        while (true)
        {
            printMenu(p, q);
            const int choice = readIntInRange("Ваш выбор: ", 0, 8);

            switch (choice)
            {
            case 1:
                p = readPolynomial("P");
                break;

            case 2:
                q = readPolynomial("Q");
                break;

            case 3:
                p = Polynomial(); // временный объект создаётся конструктором по умолчанию
                std::cout << "P = Polynomial();   // P = " << p << '\n';
                break;

            case 4:
                q = Polynomial(p); // временный объект создаётся конструктором копирования
                std::cout << "Q = Polynomial(P);   // Q = " << q << '\n';
                break;

            case 5:
            {
                const double x = readDouble("x = ");
                std::cout << "P(" << x << ") = " << p.valueAt(x) << '\n'
                          << "Q(" << x << ") = " << q.valueAt(x) << '\n';
                break;
            }

            case 6:
                printRoots("P", p);
                break;

            case 7:
                printRoots("Q", q);
                break;

            case 8:
                std::cout << "Поиск корней с запуска программы, число вызовов: "
                          << Polynomial::getRootSearchCount() << '\n';
                break;

            default: // 0 - выход
                std::cout << "Работа программы завершена.\n";
                return 0;
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "\nОшибка: " << error.what() << '\n';
        return 1;
    }
}
