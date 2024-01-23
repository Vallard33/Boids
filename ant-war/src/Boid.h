#ifndef Boid_h
#define Boid_h

#include "Pair.cpp"

class Boid {
	Pair<double> pos;
	Pair<double> speed;
public:
	Boid(Pair<double> p = Pair<double>(0., 0.), Pair<double> s = Pair<double>(0.1, 0.1));
	Pair<double> Get_Pos();
	Pair<double> Get_Speed();
	void set_Pos(Pair<double> p);
	void set_Speed(Pair<double> s);
	void random(double, double);
	void new_boid(Boid*, int, int, double, double);
};

class Group {
	const int num;
	Boid* tab;
public:
	Group(int num = 20, int height = 600, int width = 800);
	~Group();
	void new_group(double, double);
	int Get_Num();
	Boid* Get_Tab();
	static const double dist_max_sep;
	static const double dist_max;
	static const double separation;
	static const double coherence;
	static const double alignement;
	static const double max_speed;
};

#endif