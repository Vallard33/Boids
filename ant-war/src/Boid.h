#ifndef Boid_h
#define Boid_h

#include "Pair.cpp"

class Boid {
protected :
	Pair<double> pos;
	Pair<double> speed;
	int color_r;
	int color_g;
	int color_b;
public:
	Boid(double height_d = 1280., double width_d = 720.);
	Pair<double> get_pos();
	Pair<double> get_speed();
	void set_pos(Pair<double> p);
	void set_speed(Pair<double> s);
	int get_color_r();
	int get_color_g();
	int get_color_b();
};

class Prey :public Boid {
public:
	Prey(double height_d = 1280., double width_d = 720.);
	void new_prey(Prey*, Boid*, int, int, int, double, double); //Ensemble des proies, ensemble des prédateurs, nombres de proies, nombre de prédateurs, indice de l'oiseau, taille de la fenêtre
};

class Predator :public Boid {
public:
	Predator(double height_d = 1280., double width_d = 720.);
	void new_predator(Boid*, Predator*, int, int, int, double, double); //Ensemble des proies, ensemble des prédateurs, nombres de proies, nombre de prédateurs, indice de l'oiseau, taille de la fenêtre
};

class Group {
	const int num_prey;
	const int num_predator;
	Prey* tab_prey;
	Predator* tab_predator;
public:
	Group(int num_prey = 20, int num_predator =2, int height = 600, int width = 800);
	~Group();
	void new_group(double, double);
	int get_num_prey();
	int get_num_predator();
	Prey* get_tab_prey();
	Predator* get_tab_predator();
	static const double dist_max_sep_prey;
	static const double dist_max_coh_prey;
	static const double dist_max_al_prey;
	static const double separation_prey;
	static const double coherence_prey;
	static const double alignement_prey;
	static const double max_speed_prey;
	static const double max_speed_predator;
	static const double dist_max_escape;
	static const double escape;
	static const double hunt;
};


#endif