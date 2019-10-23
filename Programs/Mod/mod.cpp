#include <iostream>
#include "mod.h"

Mod::Mod(long t): x(t % modulus) {
    if (x < 0) x += modulus;
}
Mod::Mod(const Mod& m): x(m.x) {}
Mod& Mod::operator=(const Mod& m) {
    x = m.x;
    return *this;
}
Mod& Mod::operator+=(const Mod& m) {
    if (x > 0 && m.x > 0 && (x+m.x) < 0) {
        x = (((x % modulus) - modulus) + (m.x % modulus)) % modulus;
    } else if (x < 0 && m.x < 0 && (x+m.x) > 0) {
        x = (((x % modulus) + modulus) + (m.x % modulus)) % modulus;
    } else x = (x + m.x) % modulus;
    return *this;
}
Mod& Mod::operator-=(const Mod& m) {
    return *this += -m;
}
Mod& Mod::operator*=(const Mod& m) {
    long cur = m.x;
    Mod sum(0);
    Mod n(x);
    while (cur != 0) {
        if (cur & 1) sum += n;
        cur >>= 1;
        n += n;
    }
    *this = sum;
    return *this;
}
Mod& Mod::operator/=(const Mod& m) {
    Mod new_this(x);
    new_this *= m.pwr(-1);
    *this = new_this;
    return *this;
}
Mod Mod::operator-() const {
    return Mod(-x);
}
Mod Mod::pwr(long e) const {
    if (x == 0) return Mod(0);
    if (e < 0) {
        return inv(x).pwr(-e);
    } else if (e == 0) {
        return Mod(1);
    } else {
        if (x > 0) {
            Mod xm(x);
            if (e % 2 == 0) {
                xm *= xm;
                return xm.pwr(e/2);
            } else {
                Mod f = (xm*xm).pwr((e-1)/2);
                return f*xm;
            }
        } else {
            int p = -1;
            if (e % 2 == 0) p = 1;
            return Mod(p * Mod(-x).pwr(e).val());
        }
    }
}
long Mod::val() const { return x; }
void Mod::set_modulus(long m) {
    if (m < 2) {
        cerr << "Modulus less than 2\n";
        exit(-1);
    }
    modulus = m;
}

Mod Mod::inv(long r0) {
    long m = modulus;
    r0 %= m;
    if (r0 > 0 && r0+m < 0) return inv(r0 - m);
    else r0 = (r0 + m) % m;
    long orig = m, x1 = 1, x2 = 0;
    bool neg = false;
    while (m % r0 != 0) {
        long q = m / r0;
        long t = x1;
        x1 = q * x1 + x2;
        x2 = t;
        t = r0;
        r0 = m % r0;
        m = t;
        neg = !neg;
    }
    if ((m / r0 * x1 + x2) != orig) {
        cerr << "Inverse does not exist\n";
        exit(-1);
    }
    if (neg) return (-x1 % orig + orig) % orig;
    else return Mod(x1);
}

Mod operator+(const Mod& a, const Mod& b) {
    Mod sum = a;
    sum += b;
    return sum;
}
Mod operator+(long t, const Mod& m) {
    return m+Mod(t);
}
Mod operator-(const Mod& a, const Mod& b) {
    Mod dif = a;
    dif -= b;
    return dif;
}
Mod operator-(long t, const Mod& m) {
    return m - Mod(t);
}
Mod operator*(const Mod& a, const Mod& b) {
    Mod prod = a;
    prod *= b;
    return prod;
}
Mod operator*(long t, const Mod& m) {
    return m*Mod(t);
}
Mod operator/(const Mod& a, const Mod& b) {
    Mod quo = a;
    quo /= b;
    return quo;
}
Mod operator/(long t, const Mod& m) {
    return m/Mod(t);
}
bool operator==(const Mod& a, const Mod& b) {
    return ((a.val() % Mod::get_modulus()) == (b.val() % Mod::get_modulus()));
}
bool operator==(long t, const Mod& m) {
    return ((m.val() % Mod::get_modulus()) == (t % Mod::get_modulus()));
}
bool operator!=(const Mod& a, const Mod& b) {
    return !(a == b);
}
bool operator!=(long t, const Mod& m) {
    return !(t == m);
}
istream& operator>>(istream& is, Mod& m) {
    long x;
    is >> x;
    m = Mod(x);
    return is;
}
ostream& operator<<(ostream& os, const Mod& m) {
    return os << m.val() << std::endl;
} 
long Mod::modulus = 17;

int main() {
    Mod::set_modulus(9223372036854775783);
    Mod a(0),b(0);
    cout << "Enter modulus:\n";
    long m;
    cin >> m;
    a.set_modulus(m);
    cout << "Enter a,b:\n";
    cin >> a >> b;
    cout << "inverse of a: " << a.pwr(-1);
    cout << "confirm: " << a*a.pwr(-1);
    cout << "inverse of b: " << b.pwr(-1);
    cout << "confirm: " << b*b.pwr(-1);
    cout << "a + b: " << a+b;
    cout << "a - b: " << a-b;
    cout << "a * b: " << a*b;
    cout << "a / b: " << a/b;
    cout << "a ^ b: " << a.pwr(b.val());
    return 0;
}