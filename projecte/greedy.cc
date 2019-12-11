#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std;

// Comptador global, perquè cada jugador tingui un identificador unic.
int player_count = 0;
int getPlayerId() {return player_count++;}

// Donat un string amb la posicio d'un jugador, la retorna amb numero.
int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else return 3;
}

/*    STRUCT PLAYER
- Conte la informacio basica del jugador (nom, club, posicio, preu i punts).
- Conte informacio afegida per realitzar tasques mes facilment (id, npos).
*/
struct Player {
	string name, pos, club;
	int id, npos, price, points;
	Player(string name, string pos, int price, string club, int points, int iden = getPlayerId()):
    	name(name), pos(pos), club(club), id(iden),
    	npos(getpos(pos)), price(price), points(points) {}

    /* OPERADOR <
	- Només compara si un jugador es pitjor que un altre, definit com que te
	menys punts i alhora es mes car.

	ATENCIO: no es simetric, ni defineix un ordre total entre els jugadors.
	*/
	bool operator< (const Player& J) {
    if (points == J.points) return price < J.price;
  	return points > J.points;
	}
};


/* STRUCT ALIGNEMENT
- Conte informacio sobre una alineacio concreta
sempre hi ha un porter
n1: nombre de defenses
n2: nombre de migcampistes
n3: nombre de davanters

CONDICIO: n1 + n2 + n3 = 10

- Tambe conte la suma de punts i la suma de preus dels jugadors

El constructor inicialitza amb jugadors inventats
*/
struct Alignment {
	vector<Player> aln;
	int n1, n2, n3, total_points, total_price;

  	Alignment (int n1, int n2, int n3, int totP = -11, int pr = -11):
    	aln(vector<Player> (11, Player("", "", -1, "", -1))),
    	n1(n1), n2(n2), n3(n3), total_points(totP), total_price(pr) {}

	// OPERADORS
	bool operator<  (const Alignment& a2) {return total_points <  a2.total_points;}
	bool operator>  (const Alignment& a2) {return total_points >  a2.total_points;}
	bool operator== (const Alignment& a2) {return total_points == a2.total_points;}

	Player& operator[] (int idx) {return aln[idx];}

  	void addPlayer (const Player& J, int i) {
  		total_points += J.points - aln[i].points;
  		total_price  += J.price  - aln[i].price;
    	aln[i] = J;
  	}

  	bool has (const Player& J) {
  		switch (J.npos) {
  			case 0: return J.id == aln[0].id;
  			case 1: for (int i = 1; i <= n1; ++i) if (J.id == aln[i].id) return true;
		  	case 2: for (int i = n1+1; i <= n1+n2; ++i) if (J.id == aln[i].id) return true;
		  	case 3: for (int i = n1+n2+1; i <= 10; ++i) if (J.id == aln[i].id) return true;
  		}
  		return false;
  	}
};

vector<vector<Player>> PlayerDatabase(4);

int n1, n2, n3, t, j;
clock_t start_time;

// Funcio que imprimeix en un fitxer una alineacio
void write(string& filename, Alignment& A) {
	ofstream out;
	out.open(filename);
	out.setf(ios::fixed);
	out.precision(1);
	clock_t t = clock() - start_time;

	out << double(t)/CLOCKS_PER_SEC << endl
			<< "POR: " << A[0].name << endl
			<< "DEF: ";
	for (int i = 0; i < n1; ++i) out << (i == 0 ? "" : ";") << A[i+1].name;
	out << endl
			<< "MIG: ";
	for (int i = 0; i < n2; ++i) out << (i == 0 ? "" : ";") << A[i+1+n1].name;
	out << endl
			<< "DAV: ";
	for (int i = 0; i < n3; ++i) out << (i == 0 ? "" : ";") << A[i+1+n1+n2].name;
	out << endl
			<< "Punts: " << A.total_points << endl
			<< "Preu: "  << A.total_price << endl;
	out.close();
}

// Lectura dels parametres d'entrada
void read_query(string& filename) {
	ifstream in(filename);
	in >> n1 >> n2 >> n3 >> t >> j;
}


// Funcio que llegeix d'un fitxer un llistat de jugadors
// i els col·loca ordenats per posicio i per punts a la base
// de dades de jugadors que consta de 4 vectors de jugadors.
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
	  if (preu <= j) {PlayerDatabase[jugador.npos].push_back(jugador);}
  }
  in.close();
  for (int k = 0; k < 4; ++k) sort(PlayerDatabase[k].begin(), PlayerDatabase[k].end(), order);

}

// ************ FUNCIO MAIN **************

int main(int argc, char** argv) {
  // En cas de no proporcionar-se tots els fitxers d'entrada salta un error.
	if (argc != 4) {
	    cout << "Syntax: " << argv[0] << " [data_base_file_name] [query_file_name] [output_file_name]" << endl;
	    exit(1);
	}

  // Llegim les dades d'entrada a traves dels fitxers proporcionats.
	string input_file_name = argv[1], query_file_name = argv[2], output_file_name = argv[3];
	read_query(query_file_name);
	read_database(input_file_name);

	start_time = clock(); // Iniciem el cronometre.

  // Deduim quina es la millor alineacio que podem trobar.
	Alignment bestTeam(n1, n2, n3);
  Greedy();

  // Escribim la solucio
  write(output_file_name, bestTeam);
}
