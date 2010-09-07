#include <cmath>
#include "vector.hpp"

const Vector v_home(0.0, 0.0);
const Vector v_up(0.0, -1.0);
const Vector v_down(0.0, 1.0);
const Vector v_left(-1.0, 0.0);
const Vector v_right(1.0, 0.0);

Vector::Vector()
{
    x = 0.0;
    y = 0.0;
    mag_sq = 0.0;
    mag = 0.0;
    need_calc_mag_sq = false;
    need_calc_mag = false;
}

Vector::Vector(const float x, const float y)
{
    this->x = x;
    this->y = y;
    mag_sq = 0.0;
    mag = 0.0;
    need_calc_mag_sq = true;
    need_calc_mag = true;
}

Vector::Vector(const Vector& v)
{
    x = v.x;
    y = v.y;
    mag_sq = v.mag_sq;
    mag = v.mag;
    need_calc_mag_sq = v.need_calc_mag_sq;
    need_calc_mag = v.need_calc_mag;
}

Vector& Vector::operator =(const Vector& v)
{
    x = v.x;
    y = v.y;
    mag_sq = v.mag_sq;
    mag = v.mag;
    need_calc_mag_sq = v.need_calc_mag_sq;
    need_calc_mag = v.need_calc_mag;
    return *this;
}

Vector& Vector::operator +=(const Vector& v)
{
    XY(x + v.x, y + v.y);
    return *this;
}

Vector& Vector::operator -=(const Vector& v)
{
    XY(x - v.x, y - v.y);
    return *this;
}

Vector& Vector::operator *=(const float f)
{
    XY(x * f, y * f);
    return *this;
}

const Vector Vector::operator+(const Vector &v) const
{
    return Vector(*this) += v;
}

const Vector Vector::operator-(const Vector &v) const
{
    return Vector(*this) -= v;
}

const Vector Vector::operator*(const float f) const
{
    return Vector(*this) *= f;
}


bool Vector::operator ==(const Vector& v) const
{
    if (fabs(this->x - v.x) < 0.001 && fabs(this->y - v.y < 0.001))
        return true;
    return false;
}



float Vector::MagSq()
{
    if (need_calc_mag_sq) {
        mag_sq = x*x + y*y;
        need_calc_mag_sq = false;
    }
    return mag_sq;
}

float Vector::Mag()
{
    if (need_calc_mag) {
        mag = sqrt(MagSq());
        need_calc_mag = false;
    }
    return mag;
}

const float Vector::DistanceSq(const Vector& v) const
{
    return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);
}

const float Vector::Distance(const Vector& v) const
{
    return sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y));
}

float Vector::AngleTo(const Vector& v)
{
    return atan2(v.y - y, v.x - x);
}

float Vector::AngleFrom(const Vector& v)
{
    return atan2(y - v.y, x - v.x);
}

const float Vector::X() const
{
    return x;
}

const float Vector::Y() const
{
    return y;
}

const float Vector::XInt() const
{
    return static_cast<int>(x>0?x+0.5:x-0.5);
}

const float Vector::YInt() const
{
    return static_cast<int>(y>0?y+0.5:y-0.5);
}

void Vector::X(float x)
{
    this->x = x;
    need_calc_mag_sq = true;
    need_calc_mag = true;
}

void Vector::Y(float y)
{
    this->y = y;
    need_calc_mag_sq = true;
    need_calc_mag = true;
}

void Vector::XY(float x, float y)
{
    this->x = x;
    this->y = y;
    need_calc_mag_sq = true;
    need_calc_mag = true;
}
