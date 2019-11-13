#include <iostream>
#include <vector>
#include <fstream>
#incldue <cassert>

using namespace std;

int count = 0;

int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else return 3;
}

int getId() {
	return count++;
}

struct Player {
	string name, id, pos, club;
	int npos, price, points;
	Player(string name, string pos, int price, string club, int point):
    name(name), id(getId()), pos(pos), npos(getpos(pos)), price(price), club(club), points(points) {}
};

vector<Player> database;

void write() {

}

void read_consult(string& filename) {

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
		Player jugador(nom,posicio,preu,club,punts);
		database.push_back(jugador);
  }
  in.close();
}


vector<int> vec(11);
vector<int> alineacio = {1, n1, n2, n3};
void rec(int pos, int dinerete) {
	if (pos == 11) {

	} else {

	}
}

int main(int argc, char** argv) {
	if (argc != 3) {
    cout << "Syntax: " << argv[0] << " data_base.txt query_file.txt" << endl;
    exit(1);
  }
	read_database(argv[1]);
	read_consult(argv[2]);
}
