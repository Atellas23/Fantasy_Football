#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std;

int player_count = 0;

int getPlayerId() {
	return player_count++;
}

int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else return 3;
}

struct Player {
	string name, pos, club;
	int id, npos, price, points;
	Player(string name, string pos, int price, string club, int points):
    name(name), pos(pos), club(club), id(getPlayerId()), npos(getpos(pos)), price(price), points(points) {}

	// OPERADORS
	bool operator< (const Player& j2) {
		//if (points == j2.points) return price < j2.price;
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

vector<Player> database;
/*
vector<Player> gk; // porters (Goal-Keepers)
vector<Player> df; // DeFenses
vector<Player> md; // migcampistes
vector<Player> dv; // DaVanters
*/
int n1, n2, n3, t, j;
clock_t start_time;

bool positionOrder(const Player& a, const Player& b) {
	return a.npos < b.npos;
}

void write(string& filename, Alignment& A) {
	ofstream out;
	out.open(filename);
	out.setf(ios::fixed);
	out.precision(2);
	clock_t t = clock() - start_time;
	sort(A.aln.begin(),A.aln.end(),positionOrder);
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
			database.push_back(jugador);
			/*
			switch(getpos(posicio)) {
				case 0: gk.push_back(jugador);
				case 1: df.push_back(jugador);
				case 2: md.push_back(jugador);
				case 3: dv.push_back(jugador);
			}
			*/
		}
  }
  in.close();
}

vector<int> vec(11);
vector<int> alineacio = {1, n1, n2, n3};
vector<bool> used;

void rec(int pos, int money_left, string& output_file_name, Alignment& currentTeam, Alignment& bestTeam) {
	if (pos == 11) {
		if (currentTeam == bestTeam ? currentTeam.price < bestTeam.price : currentTeam < bestTeam) {
			bestTeam = currentTeam;
			write(output_file_name, currentTeam);
		}
	} else {
		if (money_left < j) return;
		for (Player& p: database) {
			if (not used[p.id]) {
				used[p.id] = true;
				if (alineacio[p.npos] > 0) {
					--alineacio[p.npos];
					currentTeam.addPlayer(p,pos);
					rec(pos + 1, money_left - p.price, output_file_name, currentTeam, bestTeam);
					++alineacio[p.npos];
				}
				used[p.id] = false;
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc != 4) {
    cout << "Syntax: " << argv[0] << " data_base.txt [query_file_name] [output_file_name]" << endl;
    exit(1);
  }
	string input_file_name = argv[1], query_file_name = argv[2], output_file_name = argv[3];
	read_consult(query_file_name);
	read_database(input_file_name); // llegim només els jugadors amb preu <= j
	used = vector<bool>(player_count, false);
	sort(database.begin(), database.end()); // ordenem els jugadors per punts
	start_time = clock();
	Alignment team(n1, n2, n3, 0, j, t, 0);
	Alignment bestTeam(n1, n2, n3, 0, j, t, 0);
	// rec(0,t,output_file_name,team,bestTeam);
}
