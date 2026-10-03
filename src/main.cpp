/**
 * @file main.cpp
 * @brief Точка входа: меню калькулятора многочленов второй степени.
 * @details Программа хранит два многочлена — P и Q — и выполняет с ними
 * действия из меню. Лабораторная работа выполняется поэтапно:
 * - v0.1 — часть 1: класс Polynomial — конструкторы, деструктор, статические
 *   поля, вывод, значение в точке, поиск корней и счётчик поисков;
 * - v0.2 — часть 2: перегрузка унарных операций и арифметики;
 * - v1.0 — часть 3: перегрузка сравнений, демонстрация всех операций.
 *
 * Меню использует все конструкторы и все перегруженные операции, как требует
 * задание; пункт 18 показывает их все по порядку.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 1.0
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
constexpr const char* PROGRAM_VERSION = "v1.0";

/// Наибольший номер пункта меню.
constexpr int MENU_LAST_ITEM = 18;

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
 * @brief Выводит результат одного сравнения: «P < Q : да».
 * @param expression Сравнение, как оно записано в коде.
 * @param result     Результат.
 */
void printComparison(const std::string& expression, bool result)
{
    std::cout << "  " << expression << " : " << (result ? "да" : "нет") << '\n';
}

/**
 * @brief Сравнивает два многочлена всеми шестью операциями в точке x₀.
 * @param leftName  Имя левого многочлена («P»).
 * @param left      Левый многочлен.
 * @param rightName Имя правого многочлена («Q»).
 * @param right     Правый многочлен.
 */
void compareAll(const std::string& leftName, const Polynomial& left,
                const std::string& rightName, const Polynomial& right)
{
    const double x = Polynomial::getComparePoint();
    std::cout << "Сравнение значений в точке x0 = " << x << ": " << leftName << "(x0) = "
              << left.valueAt(x) << ", " << rightName << "(x0) = " << right.valueAt(x) << '\n';
    const std::string l = leftName;
    const std::string r = rightName;
    printComparison(l + " <  " + r, left < right);
    printComparison(l + " >  " + r, left > right);
    printComparison(l + " <= " + r, left <= right);
    printComparison(l + " >= " + r, left >= right);
    printComparison(l + " == " + r, left == right);
    printComparison(l + " != " + r, left != right);
}

/**
 * @brief Демонстрация всех конструкторов и перегруженных операций по порядку.
 * @details Работает со своими многочленами, P и Q из меню не меняет. Каждая
 * строка — выражение на C++ и его результат.
 */
void demoAllOperations()
{
    std::cout << "\n--- 1. Конструкторы ---\n";
    const Polynomial zero;                  // по умолчанию
    const Polynomial a(1.0, -3.0, 2.0);     // с параметрами
    const Polynomial b(a);                  // копирования
    std::cout << "Polynomial zero;              // " << zero << '\n'
              << "Polynomial a(1, -3, 2);       // " << a << '\n'
              << "Polynomial b(a);              // " << b << '\n';

    std::cout << "\n--- 2. Унарные операции (на копии c = a) ---\n";
    Polynomial c(a);
    // Результат операции сохраняется отдельно: в C++14 порядок вычисления
    // частей одного выражения с << не гарантирован, и c могло бы вывестись
    // раньше, чем изменилось
    Polynomial result = ++c;
    std::cout << "++c вернул " << result << ",   c = " << c << '\n';
    result = c++;
    std::cout << "c++ вернул " << result << ",   c = " << c << '\n';
    result = --c;
    std::cout << "--c вернул " << result << ",   c = " << c << '\n';
    result = c--;
    std::cout << "c-- вернул " << result << ",   c = " << c << '\n';

    std::cout << "\n--- 3. Арифметическое присваивание (d = x² + x + 1) ---\n";
    Polynomial d(1.0, 1.0, 1.0);
    d += a;
    std::cout << "d += a;   // d = " << d << '\n';
    d -= a;
    std::cout << "d -= a;   // d = " << d << '\n';
    d *= 4.0;
    std::cout << "d *= 4;   // d = " << d << '\n';
    d /= 2.0;
    std::cout << "d /= 2;   // d = " << d << '\n';

    std::cout << "\n--- 4. Бинарные арифметические операции (через +=, -=, *=, /=) ---\n";
    std::cout << "a + d = " << (a + d) << '\n'
              << "a - d = " << (a - d) << '\n'
              << "a * 3 = " << (a * 3.0) << '\n'
              << "3 * a = " << (3.0 * a) << '\n'
              << "a / 2 = " << (a / 2.0) << '\n';

    std::cout << "\n--- 5. Сравнения в точке x0 = " << Polynomial::getComparePoint() << " ---\n";
    compareAll("a", a, "d", d);

    std::cout << "\n--- 6. Корни и статические поля ---\n";
    printRoots("a", a);
    std::cout << "Поиск корней, число вызовов с запуска: " << Polynomial::getRootSearchCount() << '\n'
              << "Многочленов в памяти сейчас (вместе с P, Q и объектами этой демонстрации): "
              << Polynomial::getExistingCount() << '\n';
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
              << "  Многочленов в памяти: " << Polynomial::getExistingCount()
              << ",   точка сравнения x0 = " << Polynomial::getComparePoint() << '\n'
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
              << " Сравнение:\n"
              << " 16 - задать точку сравнения x0\n"
              << " 17 - сравнить P и Q в точке x0 (<, >, <=, >=, ==, !=)\n"
              << " 18 - показать все конструкторы и операции по порядку\n"
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

    case 15:
        incrementDecrementMenu(p);
        break;

    case 16:
        Polynomial::setComparePoint(readDouble("x0 = "));
        std::cout << "Точка сравнения x0 = " << Polynomial::getComparePoint() << '\n';
        break;

    case 17:
        compareAll("P", p, "Q", q);
        break;

    default: // 18
        demoAllOperations();
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
