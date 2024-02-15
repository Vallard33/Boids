#ifndef Pair_h
#define Pair_h
#include <math.h>

template <class T>

class Pair {
private:
	T fst;
	T snd;
public:
	Pair(T f = 0., T s = 0.);
	const T get_first();
	const T get_second();
	void set_first(T);
	void set_second(T);
	T module();
	template <class T> friend const Pair<T> operator+(Pair<T> const&, Pair<T> const&);
	template <class T> friend const Pair<T> operator-(Pair<T> const&, Pair<T> const&);
	template <class T> friend const Pair<T> operator*(Pair<T> const&, T const&);
	template <class T> friend const Pair<T> operator/(Pair<T> const&, T const&);
	template <class T> friend const Pair<T> operator+(T const&, Pair<T> const&);
	template <class T> friend const Pair<T> operator/(T const&, Pair<T> const&);
};



template <class T> T distance(Pair<T>, Pair<T>);


template <class T> Pair<T>::Pair(T f, T s) {
	this->fst = f;
	this->snd = s;
};

template <class T> inline const T Pair<T>::get_first() {
	return this->fst;
};

template <class T> inline const T Pair<T>::get_second() {
	return this->snd;
};

template <class T> void Pair<T>::set_first(T f) {
	this->fst = f;
}

template <class T> void Pair<T>::set_second(T s) {
	this->snd = s;
}

template <class T> T distance(Pair<T> p1, Pair<T> p2) {
	const T a = p1.get_first() - p2.get_first();
	const T b = p1.get_second() - p2.get_second();
	return sqrt(pow(a, 2.) + pow(b, 2.));
};

template <class T> T Pair<T>::module() {
	const T a = this->fst;
	const T b = this->snd;
	return sqrt(pow(a, 2.) + pow(b, 2.));
}

template <class T> const Pair<T> operator+(Pair<T> const& a, Pair<T> const& b) {
	return Pair<T>(a.fst + b.fst, a.snd + b.snd);
};

template <class T> const Pair<T> operator-(Pair<T> const& a, Pair<T> const& b) {
	return Pair<T>(a.fst - b.fst, a.snd - b.snd);
};

template <class T> const Pair<T> operator*(Pair<T> const& a, T const& b) {
	return Pair<T>(a.fst * b, a.snd * b);
};

template <class T> const Pair<T> operator/(Pair<T> const& a, T const& b) {
	return Pair<T>(a.fst / b, a.snd / b);
};

template <class T> const Pair<T> operator+(T const& a, Pair<T> const& b) {
	return Pair<T>(a + b.fst, a + b.snd);
};

template <class T> const Pair<T> operator/(T const& a, Pair<T> const& b) {
	return Pair<T>(a / b.fst, a / b.snd);
};

#endif

