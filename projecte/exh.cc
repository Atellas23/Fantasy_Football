#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cassert>

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
	int n1, n2, n3, total_points, max_player_price, budget, price;

  Alignment(int n1, int n2, int n3, int totP = 0, int prj = 0, int bg = 0, int price = 0):
    aln(vector<Player>(11, Player("", "", -1, "", -1))), n1(n1), n2(n2), n3(n3), total_points(totP), max_player_price(prj), budget(bg), price(price) {}

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
int n1, n2, n3, t, j;
clock_t start_time;

void write(string& filename, Alignment& A) {
	ofstream out;
	out.open(filename);
	out.setf(ios::fixed);
	out.precision(2);
	clock_t t = clock() - start_time;
	out << double(t) << endl
			<< "POR: " << A[0].name << endl
			<< "DEF: ";
	for (int i = 0; i < n1; ++i) out << (i == 0 ? "" : ";") << A[i+1].name;
	out << endl
			<< "MIG: ";
	for (int i = 0; i < n2; ++i) out << (i == 0 ? "" : ";") << A[i+n1].name;
	out << endl
			<< "DAV: ";
	for (int i = 0; i < n3; ++i) out << (i == 0 ? "" : ";") << A[i+n2].name;
	out << endl
			<< "Punts: " << A.total_points << endl
			<< "Preu: "  << A.price << endl;
	out.close();
}

void read_consult(string& filename) {
	ifstream in(filename);
	in >> n1 >> n2 >> n3 >> t >> j;
}

void read_database(string& filename) {
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
		if (preu <= j) { // tallem la base de dades pel preu
			switch(getpos(posicio)) {
				case 0: gk.push_back(jugador);
				case 1: df.push_back(jugador);
				case 2: md.push_back(jugador);
				case 3: dv.push_back(jugador);
			}
		}
  }
  in.close();
}


vector<int> vec(11);
vector<int> alineacio = {1, n1, n2, n3};
void rec(int& pos, int money_left, string& output_file_name, Alignment& currentTeam) {
	if (pos == 11) {
		if (currentTeam <)
	} else {

	}
}

int main(int argc, char** argv) {
	if (argc != 4) {
    cout << "Syntax: " << argv[0] << " data_base.txt [query_file_name] [output_file_name]" << endl;
    exit(1);
  }
	read_consult(argv[2]);
	read_database(argv[1]); // llegim només els jugadors amb preu <= j
	sort(database.begin(),database.end()); // ordenem els jugadors per punts
	start_time = clock();
	// rec();
}
