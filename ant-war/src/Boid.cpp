#include "Boid.h"
#include <random>

const double Group::dist_max_sep = 0.002;
const double Group::dist_max = 0.8;
const double Group::separation = 0.004;
const double Group::coherence = 0.5;
const double Group::alignement = 0.5;
const double Group::max_speed = 0.8;

Boid::Boid(Pair<double> p, Pair<double> s) {
	this->pos = p;
	this->speed = s;
}

Pair<double> Boid::Get_Pos() {
	return this->pos;
}

Pair<double> Boid::Get_Speed() {
	return this->speed;
}

void Boid::set_Pos(Pair<double> p) {
	this->pos = p;
}

void Boid::set_Speed(Pair<double> s) {
	this->speed = s;
}

double rand(double x, double y) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(x, y);
	return distribution(generator);
}

void Boid::random(double height_d, double width_d) {
	double pf = rand(0, width_d);
	double ps = rand(0., height_d);
	double sf = rand(-0.1, 0.1);
	double ss = rand(-0.1, 0.1);
	Pair<double> p (pf, ps);
	Pair<double> s (sf, ss);
	this->set_Pos(p);
	this->set_Speed(s);

}

void Boid::new_boid(Boid* tab, int size, int i, double height_d, double width_d) {
	Pair<double> coh = Pair<double>(0., 0.);
	Pair<double> sep = Pair<double>(0., 0.);
	Pair<double> al = Pair<double>(0., 0.);
	Pair<double> pi = this->pos;
	Pair<double> si = this->speed;
	double neighbors = 0.;
	for (int j = 0; j < size; j++) {
		if (j != i) {
			Pair<double> pj = tab[j].pos;
			Pair<double> sj = tab[j].speed;
			double dist = Distance(pi, pj);
			if (dist < Group::dist_max_sep) {
				sep = sep + pi - pj;
			}
			if (dist < Group::dist_max) {
				neighbors += 1.;
				al = al + sj;
				coh = coh + pj;
			}
		}
	}
	si = si + (1. / (1. + sep)) * Group::separation;
	if (neighbors > 0) {
		al = al / neighbors;
		si = si + (al - si) / Group::alignement;
		coh = coh / neighbors;
		si = si + (coh - pi) * Group::coherence;
	};
	if (si.Module() > Group::max_speed) { si = si * (Group::max_speed / si.Module()); };
	pi = pi + si;
	while (pi.get_First() > width_d) { pi.set_First(pi.get_First() - width_d);}
	while (pi.get_Second() > height_d) { pi.set_Second(pi.get_Second() - height_d); }
	while (pi.get_First() < 0) { pi.set_First(pi.get_First() + width_d); }
	while (pi.get_Second() < 0 ) { pi.set_Second(pi.get_Second() + height_d); }
	set_Pos(pi);
	set_Speed(si);
}

Group::Group(int n, int height_d, int width_d) : num(n) {
	this->tab = new Boid[n];
	for (int i = 0; i < n; i++) {
		this->tab[i].random(height_d, width_d);
	}
}

Group::~Group() {
	delete[] tab;
}

int Group::Get_Num() {
	return this->num;
}

Boid* Group::Get_Tab() {
	return this->tab;
}

void Group::new_group(double height_d, double width_d) {
	for (int i = 0; i < this->num; i++) {
		this->tab[i].new_boid(this->tab, this->num, i, height_d, width_d);
	}
}