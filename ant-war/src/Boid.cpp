#include "Boid.h"
#include <random>

const double Group::dist_max_sep_prey = 5;
const double Group::dist_max_coh_prey = 25.;
const double Group::dist_max_al_prey = 10.;
const double Group::separation_prey = 12.;
const double Group::coherence_prey = 12.;
const double Group::alignement_prey = 40.;
const double Group::max_speed_prey = 2.;
const double Group::max_speed_predator = 2.1;
const double Group::dist_max_escape = 5.;
const double Group::escape = 50.;
const double Group::hunt = 10.;

/* Pour générer des doubles aléatoires*/
double rand(double x, double y) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(x, y);
	return distribution(generator);
}

/* On attribue une couleur aux oiseaux pour qu'ils soient différents à l'affichage
   Leur position et leur vitesse sont aléatoires*/
Boid::Boid(double height_d, double width_d) {
	double pf = rand(0, width_d);
	double ps = rand(0., height_d);
	double sf = rand(-1., 1.);
	double ss = rand(-1., 1.);
	Pair<double> p(pf, ps);
	Pair<double> s(sf, ss);
	this->pos = p;
	this->speed = s;
	this->color_r = rand(0, 255);
	this->color_g = rand(0, 255);
	this->color_b = rand(0, 255);
}

Pair<double> Boid::get_pos() {
	return this->pos;
}

Pair<double> Boid::get_speed() {
	return this->speed;
}

void Boid::set_pos(Pair<double> p) {
	this->pos = p;
}

void Boid::set_speed(Pair<double> s) {
	this->speed = s;
}

int Boid::get_color_r() {
	return this->color_r;
}

int Boid::get_color_g() {
	return this->color_g;
}

int Boid::get_color_b() {
	return this->color_b;
}

Prey::Prey(double height_d, double width_d) {
	double pf = rand(0, width_d);
	double ps = rand(0., height_d);
	double sf = rand(-1., 1.);
	double ss = rand(-1., 1.);
	Pair<double> p (pf, ps);
	Pair<double> s (sf, ss);
	this->pos = p;
	this->speed = s;
	this->color_r = rand(100, 255);
	this->color_g = 0;
	this->color_b = 0;
}

Predator::Predator(double height_d, double width_d) {
	double pf = rand(0, width_d);
	double ps = rand(0., height_d);
	double sf = rand(-1., 1.);
	double ss = rand(-1., 1.);
	Pair<double> p(pf, ps);
	Pair<double> s(sf, ss);
	this->pos = p;
	this->speed = s;
	this->color_r = 0;
	this->color_g = 0;
	this->color_b = rand(100,255);
}

/* Un ensemble d'oiseux est un tableau de proies et de prédateurs ainsi que le nombre de proies et de prédateurs*/
Group::Group(int num_prey, int num_predator, int height_d, int width_d) : num_prey(num_prey), num_predator(num_predator) {
	this->tab_prey = new Prey[num_prey]; //création d'un tableau
	for (int i = 0; i < num_prey; i++) { //initialisation des proies
		tab_prey[i] = Prey(height_d, width_d);
	};
	this->tab_predator = new Predator[num_predator];
	for (int i = 0; i < num_predator; i++) {
		tab_predator[i] = Predator(height_d, width_d);
	};
}

Group::~Group() {
	delete[] tab_prey;
	delete[] tab_predator;
}

/* Pour mettre à jour une proie, il faut tenir compte des autres proies et des prédateurs, et y appliqué les différentes règles de séparation, de cohésion et d'alignement. Les proies ont une vitesse maximale et lorsqu'elles arrivent à un bord elles repassent de l'autre côté. */
void Prey::new_prey(Prey* tab_prey, Boid* tab_predator, int num_prey, int num_predator, int i, double height_d, double width_d) {
	Pair<double> coh = Pair<double>(0., 0.);
	Pair<double> sep = Pair<double>(0., 0.);
	Pair<double> al = Pair<double>(0., 0.);
	Pair<double> pi = this->pos;
	Pair<double> si = this->speed;
	double neighbors_coh = 0.;
	double neighbors_al = 0.;
	for (int j = 0; j < num_prey; j++) {
		if (j != i) {
			Pair<double> pj = tab_prey[j].get_pos();
			Pair<double> sj = tab_prey[j].get_speed();
			double dist = distance(pi, pj);
			if (dist < Group::dist_max_sep_prey) {
				sep = sep + (pi - pj) * Group::separation_prey;
			}
			if (dist < Group::dist_max_coh_prey) {
				neighbors_coh += 1.;
				coh = coh + pj;
			}
			if (dist < Group::dist_max_al_prey) {
				neighbors_al += 1.;
				al = al + sj;

			}
		}
	}
	for (int j = num_prey; j < num_predator; j++) {
		Pair<double> pj = tab_predator[j].get_pos();
		double dist = distance(pi, pj);
		if (dist < Group::dist_max_escape) {
			sep = sep + (pi - pj) * Group::escape;
		}
	}
	if (neighbors_al > 0) {
		al = al / neighbors_al;
		si = si + (al - si) / Group::alignement_prey;
	};
	si = si + sep;
	if (neighbors_coh > 0) {
		coh = coh / neighbors_coh;
		si = si + (coh - pi) * Group::coherence_prey;
	};

	if (si.module() > Group::max_speed_prey) { si = si * (Group::max_speed_prey / si.module()); }; //vitesse maximale
	pi = pi + si;
	//Pour réapparaître de l'autre côté
	while (pi.get_first() > width_d) { pi.set_first(pi.get_first() - width_d); }
	while (pi.get_second() > height_d) { pi.set_second(pi.get_second() - height_d); }
	while (pi.get_first() < 0) { pi.set_first(pi.get_first() + width_d); }
	while (pi.get_second() < 0) { pi.set_second(pi.get_second() + height_d); }
	set_pos(pi);
	set_speed(si);
}

/* Les prédateurs vont vers la proie la plus proche d'eux mais ne se souviennent pas de la proie qu'ils chassent, ils ont aussi une vitesse limite*/
void Predator::new_predator(Boid* tab_prey, Predator* tab_predator, int num_prey, int num_predator, int i, double height_d, double width_d) {
	Pair<double> pi = this->pos;
	Pair<double> si = this->speed;
	int min = 0;
	double dist = distance(pi, tab_prey[0].get_pos());
	for (int j = 0; j < num_prey; j++) { // Pour chercher la proie la plus proche
		Pair<double> pj = tab_prey[j].get_pos();
		if (distance(pi, pj) < dist) {
			dist = distance(pi, pj);
			min = j;
		}
	};
	si = si + (tab_prey[min].get_pos() - pi) * Group::hunt;
	if (si.module() > Group::max_speed_predator) { si = si * (Group::max_speed_predator / si.module()); }; //vitesse maximale
	pi = pi + si;
	set_pos(pi);
	set_speed(si);
}

int Group::get_num_prey() {
	return this->num_prey;
}

int Group::get_num_predator() {
	return this->num_predator;
}

Prey* Group::get_tab_prey() {
	return this->tab_prey;
}

Predator* Group::get_tab_predator() {
	return this->tab_predator;
}

void Group::new_group(double height_d, double width_d) {
	int num_prey = this->num_prey;
	int num_predator = this->num_predator;
	for (int i = 0; i < num_prey; i++) {
		this->tab_prey[i].new_prey(this->tab_prey, this->tab_predator, num_prey, num_predator, i, height_d, width_d);
	}
	for (int i = 0; i < num_predator; i++) {
		this->tab_predator[i].new_predator(this->tab_prey, this->tab_predator, num_prey, num_predator, i, height_d, width_d);
	}
}