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
 * @version 0.2
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
constexpr const char* PROGRAM_VERSION = "v0.2";

/// Наибольший номер пункта меню.
constexpr int MENU_LAST_ITEM = 15;

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
 * @brief Показывает работу инкремента и декремента P во всех четырёх формах.
 * @details Для постфиксной формы видно, что выражение возвращает прежнее
 * значение, а для префиксной — уже новое.
 * @param p Многочлен P (меняется).
 */
void incrementDecrementMenu(Polynomial& p)
{
    std::cout << "  1 - ++P   (префиксный инкремент)\n"
              << "  2 - P++   (постфиксный инкремент)\n"
              << "  3 - --P   (префиксный декремент)\n"
              << "  4 - P--   (постфиксный декремент)\n";
    const int choice = readIntInRange("Ваш выбор: ", 1, 4);

    std::cout << "До:     P = " << p << '\n';
    switch (choice)
    {
    case 1:
    {
        const Polynomial result = ++p;
        std::cout << "++P вернул: " << result << "   (новое значение)\n";
        break;
    }
    case 2:
    {
        const Polynomial result = p++;
        std::cout << "P++ вернул: " << result << "   (прежнее значение)\n";
        break;
    }
    case 3:
    {
        const Polynomial result = --p;
        std::cout << "--P вернул: " << result << "   (новое значение)\n";
        break;
    }
    default:
    {
        const Polynomial result = p--;
        std::cout << "P-- вернул: " << result << "   (прежнее значение)\n";
        break;
    }
    }
    std::cout << "После:  P = " << p << '\n';
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
              << " Арифметика:\n"
              << "  9 - P + Q и P - Q\n"
              << " 10 - P * k, k * P и P / k\n"
              << " 11 - P += Q\n"
              << " 12 - P -= Q\n"
              << " 13 - P *= k\n"
              << " 14 - P /= k\n"
              << " 15 - инкремент и декремент P (++P, P++, --P, P--)\n"
              << "  0 - выход\n";
}

/**
 * @brief Выполняет выбранный пункт меню.
 * @param choice Номер пункта (1 … MENU_LAST_ITEM).
 * @param p      Многочлен P.
 * @param q      Многочлен Q.
 * @exception std::invalid_argument, std::overflow_error Ошибки операций
 *            (например, деление на ноль); перехватываются в main.
 */
void runMenuItem(int choice, Polynomial& p, Polynomial& q)
{
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

    case 9:
        std::cout << "P + Q = " << (p + q) << '\n'
                  << "P - Q = " << (p - q) << '\n'
                  << "(P и Q не изменились)\n";
        break;

    case 10:
    {
        const double k = readDouble("k = ");
        std::cout << "P * k = " << (p * k) << '\n'
                  << "k * P = " << (k * p) << '\n';
        std::cout << "P / k = " << (p / k) << '\n'; // при k = 0 - исключение «деление на ноль»
        break;
    }

    case 11:
        p += q;
        std::cout << "P += Q;   // P = " << p << '\n';
        break;

    case 12:
        p -= q;
        std::cout << "P -= Q;   // P = " << p << '\n';
        break;

    case 13:
    {
        const double k = readDouble("k = ");
        p *= k;
        std::cout << "P *= " << k << ";   // P = " << p << '\n';
        break;
    }

    case 14:
    {
        const double k = readDouble("k = ");
        p /= k;
        std::cout << "P /= " << k << ";   // P = " << p << '\n';
        break;
    }

    default: // 15
        incrementDecrementMenu(p);
        break;
    }
}

} // namespace

/**
 * @brief Главная функция: меню калькулятора.
 * @details Ошибки отдельных операций (деление на ноль, переполнение) выводятся,
 * и программа продолжает работу: многочлены при этом не меняются. Если
 * закончился поток ввода, программа завершается с сообщением.
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
            const int choice = readIntInRange("Ваш выбор: ", 0, MENU_LAST_ITEM);
            if (choice == 0)
            {
                std::cout << "Работа программы завершена.\n";
                return 0;
            }

            try
            {
                runMenuItem(choice, p, q);
            }
            catch (const std::invalid_argument& error)
            {
                std::cout << "Ошибка: " << error.what() << ". Многочлены не изменились.\n";
            }
            catch (const std::overflow_error& error)
            {
                std::cout << "Ошибка: " << error.what() << ". Многочлены не изменились.\n";
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "\nОшибка: " << error.what() << '\n';
        return 1;
    }
}
