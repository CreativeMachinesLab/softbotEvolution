#ifndef HCUBE_VECTOR2_H_INCLUDED
#define HCUBE_VECTOR2_H_INCLUDED

namespace HCUBE
{
    template<class T>
    class Vector2
    {
    public:
        T x,y;

        Vector2()
                :
                x(0),
                y(0)
        {}

        Vector2(T _x,T _y)
                :
                x(_x),
                y(_y)
        {}

        bool operator<(const Vector2<T> &other) const
        {
            return this->getMagnitudeSquared()<other.getMagnitudeSquared();
        }

        Vector2<T> operator/(T divisor) const
        {
            return Vector2<T>(x/divisor,y/divisor);
        }

        void operator/=(T divisor)
        {
            x/=divisor;
            y/=divisor;
        }

        Vector2<T> operator-() const
        {
            Vector2<T> newVec(-x,-y);
            return newVec;
        }

        Vector2<T> operator-(const Vector2<T> &other) const
        {
            return Vector2<T>(this->x-other.x,this->y-other.y);
        }

        Vector2<T> operator+(const Vector2<T> &other) const
        {
            return Vector2<T>(this->x+other.x,this->y+other.y);
        }

        Vector2<T> operator*(const T &coeff) const
        {
            return Vector2<T>(x*coeff,y*coeff);
        }

        void operator+=(const Vector2<T> &other)
        {
            x+=other.x;
            y+=other.y;
        }

        void operator-=(const Vector2<T> &other)
        {
            x-=other.x;
            y-=other.y;
        }

        void operator*=(T val)
        {
            x*=val;
            y*=val;
        }

        T dot(const Vector2<T> &other) const
        {
            return T(this->x*other.x+this->y*other.y);
        }

        T cross(const Vector2<T> &other) const
        {
            return (x * other.y - y * other.x);
        }

        Vector2<T> rightHandNormal() const
        {
            return Vector2<T>(-y,x);
        }

        T getMagnitudeSquared() const
        {
            return dot(*this);
        }

        void normalize()
        {
            T mag = sqrt(getMagnitudeSquared());

            if(mag>1e-6)
            {
                x/=mag;
                y/=mag;
            }
        }

        Vector2<T> projectOn(const Vector2<T> &other) const
        {
            T dp = this->dot(other);

            Vector2<T> proj = other*(dp/other.getMagnitude());

            return proj;
        }

        T distanceSquared(const Vector2<T> &other) const
        {
            return (x-other.x)*(x-other.x)+(y-other.y)*(y-other.y);
        }
    };
}


#endif // HCUBE_VECTOR2_H_INCLUDED
