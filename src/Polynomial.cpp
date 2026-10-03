/**
 * @file Polynomial.cpp
 * @brief Реализация класса Polynomial.
 * @author Vareshka86
 * @date 2026-10-03
 * @version 0.1
 */

#include "Polynomial.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

// Определения статических полей: они одни на всю программу, общие для всех многочленов.
// В C++14 статическое поле объявляется в классе, а определяется здесь, в .cpp.
int Polynomial::rootSearchCount_ = 0;
int Polynomial::existingCount_ = 0;

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/// Числа меньше этого по модулю считаются нулём: дробные вычисления дают
/// погрешность, и, например, дискриминант 1e-17 на самом деле равен 0.
const double EPSILON = 1e-12;

/**
 * @brief Заменяет «минус ноль» на обычный ноль.
 * @details В дробных числах есть -0.0 (например, -0.0 / 2): он равен нулю, но
 * печатается как «-0». Сравнение `v == 0.0` верно и для -0.0.
 * @param v Число.
 * @return То же число, но вместо -0.0 — 0.0.
 */
double withoutNegativeZero(double v)
{
    return v == 0.0 ? 0.0 : v;
}

/**
 * @brief Переводит число в строку без лишних нулей: 2 → «2», 2.5 → «2.5».
 * @param v Число.
 * @return Строка с числом.
 */
std::string formatNumber(double v)
{
    std::ostringstream out;
    out << withoutNegativeZero(v);
    return out.str();
}

/**
 * @brief Дописывает к строке многочлена одно слагаемое со знаком.
 * @param result Строка, к которой дописывается слагаемое.
 * @param coef   Коэффициент слагаемого; если он 0, слагаемое пропускается.
 * @param suffix «x²», «x» или пустая строка для свободного члена.
 */
void appendTerm(std::string& result, double coef, const std::string& suffix)
{
    if (coef == 0.0)
    {
        return;
    }

    const bool negative = coef < 0.0;
    const double absValue = std::fabs(coef);

    // Перед первым слагаемым знак пишется слитно и только «минус»,
    // между слагаемыми - через пробелы: « + » или « - »
    if (result.empty())
    {
        result += negative ? "-" : "";
    }
    else
    {
        result += negative ? " - " : " + ";
    }

    // Коэффициент 1 при x и x² не пишут: «x²», а не «1x²»
    if (!suffix.empty() && absValue == 1.0)
    {
        result += suffix;
    }
    else
    {
        result += formatNumber(absValue) + suffix;
    }
}

} // namespace

Polynomial::Polynomial()
    : a_(0.0), b_(0.0), c_(0.0)
{
    ++existingCount_;
}

Polynomial::Polynomial(double a, double b, double c)
    : a_(a), b_(b), c_(c)
{
    // Инвариант: все коэффициенты - конечные числа
    if (!std::isfinite(a_) || !std::isfinite(b_) || !std::isfinite(c_))
    {
        throw std::invalid_argument("коэффициенты многочлена должны быть конечными числами");
    }
    ++existingCount_; // только после проверки: если бросили исключение, объекта нет
}

Polynomial::Polynomial(const Polynomial& other)
    : a_(other.a_), b_(other.b_), c_(other.c_)
{
    ++existingCount_; // копия - ещё один существующий многочлен
}

Polynomial::~Polynomial()
{
    --existingCount_;
}

double Polynomial::getA() const
{
    return a_;
}

double Polynomial::getB() const
{
    return b_;
}

double Polynomial::getC() const
{
    return c_;
}

double Polynomial::valueAt(double x) const
{
    // Схема Горнера: ax² + bx + c = (a·x + b)·x + c
    return (a_ * x + b_) * x + c_;
}

int Polynomial::findRoots(double& x1, double& x2) const
{
    ++rootSearchCount_; // статическое поле можно менять и в const-методе

    // a = 0: уравнение не квадратное
    if (std::fabs(a_) < EPSILON)
    {
        if (std::fabs(b_) < EPSILON)
        {
            // 0x + c = 0: при c = 0 подходит любое x, иначе корней нет
            return std::fabs(c_) < EPSILON ? INFINITE_ROOTS : 0;
        }
        // bx + c = 0  =>  x = -c / b
        x1 = withoutNegativeZero(-c_ / b_);
        x2 = x1;
        return 1;
    }

    const double discriminant = b_ * b_ - 4.0 * a_ * c_;

    if (std::fabs(discriminant) < EPSILON)
    {
        x1 = withoutNegativeZero(-b_ / (2.0 * a_));
        x2 = x1;
        return 1;
    }
    if (discriminant < 0.0)
    {
        return 0; // действительных корней нет
    }

    const double root = std::sqrt(discriminant);
    const double first = (-b_ - root) / (2.0 * a_);
    const double second = (-b_ + root) / (2.0 * a_);

    // При a < 0 знаменатель отрицательный и «первый» корень окажется больше:
    // упорядочиваем, чтобы x1 всегда был меньшим
    x1 = withoutNegativeZero(first < second ? first : second);
    x2 = withoutNegativeZero(first < second ? second : first);
    return 2;
}

std::string Polynomial::toString() const
{
    std::string result;
    appendTerm(result, a_, "x²");
    appendTerm(result, b_, "x");
    appendTerm(result, c_, "");
    return result.empty() ? "0" : result;
}

int Polynomial::getRootSearchCount()
{
    return rootSearchCount_;
}

int Polynomial::getExistingCount()
{
    return existingCount_;
}

std::ostream& operator<<(std::ostream& out, const Polynomial& p)
{
    out << p.toString();
    return out;
}
