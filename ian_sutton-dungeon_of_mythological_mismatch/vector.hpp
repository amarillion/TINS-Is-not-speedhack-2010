#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector {
    public:
        Vector();
        Vector(const float x, const float y);
        Vector(const Vector& v);

        Vector& operator =(const Vector& v);
        Vector& operator +=(const Vector& v);
        Vector& operator -=(const Vector& v);
        Vector& operator *=(const float f);
        bool operator ==(const Vector& v) const;

        const Vector operator+(const Vector &v) const;
        const Vector operator-(const Vector &v) const;
        const Vector operator*(const float v) const;

        float MagSq();
        float Mag();
        const float DistanceSq(const Vector& v) const;
        const float Distance(const Vector& v) const;

        float AngleTo(const Vector& v);
        float AngleFrom(const Vector& v);

        const float X() const;
        const float Y() const;
        const float XInt() const;
        const float YInt() const;
        void X(float x);
        void Y(float y);
        void XY(float x, float y);

    protected:
        float x, y;
        float mag_sq, mag;
        bool need_calc_mag_sq, need_calc_mag;
};

extern const Vector v_home;
extern const Vector v_up;
extern const Vector v_down;
extern const Vector v_left;
extern const Vector v_right;


#endif
