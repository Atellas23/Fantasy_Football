#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int pl_count = 0, al_count = 0;

int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else return 3;
}

int getPlayerId() {
	return pl_count++;
}

int getAlignmentId() {
  return al_count++;
}

struct Player {
	string name, pos, club;
	int id, npos, price, points;
	Player(string name, string pos, int price, string club, int points):
    name(name), pos(pos), club(club), id(getPlayerId()), npos(getpos(pos)), price(price), points(points) {}

	// OPERADORS
	bool operator< (const Player& j2) {
		if (points == j2.points) return price < j2.price;
		return points < j2.points;
	}

	bool operator<= (const Player& j2) {
		return points <= j2.points;
	}

	bool operator> (const Player& j2) {
		return points > j2.points;
	}

	bool operator>= (const Player& j2) {
		return points >= j2.points;
	}

	bool operator== (const Player& j2) {
		return points == j2.points;
	}
};

struct Alignment {
	vector<Player> aln;
	int id, n1, n2, n3, total_points, player_price, budget;

  Alignment(int n1, int n2, int n3, int tot = 0, int prj = 0, int bg = 0):
    aln(vector<Player>(11, Player("", "", -1, "", -1))), id(getAlignmentId()), n1(n1), n2(n2), n3(n3), total_points(tot), player_price(prj), budget(bg) {}

	// OPERADORS
	bool operator< (const Alignment& a2) {
		return total_points < a2.total_points;
	}
	bool operator> (const Alignment& a2) {
		return total_points > a2.total_points;
	}
	bool operator== (const Alignment& a2) {
		return total_points == a2.total_points;
	}
	Player& operator[] (int idx) {
		return aln[idx];
	}
  void addPlayer(const Player& J, int i) {
    aln[i] = J;
  }

};

vector<Player> gk; // porters (Goal-Keepers)
vector<Player> df; // DeFenses
vector<Player> md; // migcampistes
vector<Player> dv; // DaVanters

void read_database(char* filename) {
  ifstream in(filename);
  while (not in.eof()) {
    string nom, posicio, club;
    int punts, preu;
    getline(in,nom,';');    if (nom == "") break;
    getline(in,posicio,';');
    in >> preu;
    char aux; in >> aux;
    getline(in,club,';');
    in >> punts;
    string aux2;
    getline(in,aux2);
		Player jugador(nom, posicio, preu, club, punts);
		switch(getpos(posicio)) {
			case 0: gk.push_back(jugador);
			case 1: df.push_back(jugador);
			case 2: md.push_back(jugador);
			case 3: dv.push_back(jugador);
		}
  }
  in.close();
}

int main(int argc, char** argv) {
	read_database(argv[1]);
	/*sort(gk.begin(),gk.end());
	sort(df.begin(),df.end());
	sort(md.begin(),md.end());
	sort(dv.begin(),dv.end());*/
	/*
  Player J("Aleix", "mig", 1200, "FME", 1);
  Alignment dreamteam(2, 3, 5);
  dreamteam.addPlayer(J, 0);
  cout << dreamteam[0].name << endl;
	*/
}
