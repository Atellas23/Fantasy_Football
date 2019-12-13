#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <cmath>

using namespace std;

int n1, n2, n3, t, j;
clock_t start_time;

int num_tot = 0;
double mu_tot = 0;

// Comptador global, perque cada jugador tingui un identificador unic.
int player_count = 0;
int getPlayerId() {return player_count++;}

// Donat un string amb la posicio d'un jugador, la retorna amb numero.
int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else if (pos == "dav") return 3;
	return -1;
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
	- Defineix un ordre entre els jugadors, tenint en compte els seus punts
	  i el preu que tenen. A més, fa servir el parametre j, que representa
		el cost màxim que pot tenir un jugador, com també utilitza la mitjana
		total dels punts del jugadors, per tenir un varem de quina comparacio
		es mes convenient utilitzar.
	ATENCIO
	Els coeficients utilitzats en aquesta ordenacio dels jugadors estan ajustats
	per la base de dades inicial donada.
	*/
	bool operator< (const Player& J) {
		if (mu_tot < 1e6) {
			if (points == J.points) return price < J.price;
			if (price == 0) return false;
			if (J.price == 0) return true;
	  	return double(points*points)    /pow(log(price), 16) >
						 double(J.points*J.points)/pow(log(J.price), 16);
		}
		return (1.5*double(points)   - 0.8*1e8*double(1)/(j - price)) >
					 (1.5*double(J.points) - 0.8*1e8*double(1)/(j - J.price));
	}
};

vector<vector<Player>> PlayerDatabase(4);

/* STRUCT ALIGNMENT
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
	vector< vector<Player> > aln;
	int n1, n2, n3, total_points, total_price;

	Alignment (int n1, int n2, int n3, int totP = 0, int pr = 0):
  	aln(vector< vector<Player> >(4)), n1(n1), n2(n2), n3(n3),
    total_points(totP), total_price(pr) {}

	// OPERADORS
	bool operator<  (const Alignment& a2) {return total_points <  a2.total_points;}
	bool operator>  (const Alignment& a2) {return total_points >  a2.total_points;}
	bool operator== (const Alignment& a2) {return total_points == a2.total_points;}

	vector<Player>& operator[] (int idx) {return aln[idx];}

	void addPlayer (const Player& J, int i) {
		total_points += J.points;
		total_price  += J.price;
  	aln[i].push_back(J);
	}
};


// Funcio que imprimeix en un fitxer una alineacio
void write(string& filename, Alignment& A) {
	ofstream out;
	out.open(filename);
	out.setf(ios::fixed);
	out.precision(1);
	clock_t t = clock() - start_time;

	out << double(t)/CLOCKS_PER_SEC << endl
			<< "POR: " << A[0][0].name << endl
			<< "DEF: ";
	for (int i = 0; i < n1; ++i) out << (i == 0 ? "" : ";") << A[1][i].name;
	out << endl
			<< "MIG: ";
	for (int i = 0; i < n2; ++i) out << (i == 0 ? "" : ";") << A[2][i].name;
	out << endl
			<< "DAV: ";
	for (int i = 0; i < n3; ++i) out << (i == 0 ? "" : ";") << A[3][i].name;
	out << endl
			<< "Punts: " << A.total_points << endl
			<< "Preu: "  << A.total_price << endl;
	out.close();

	out.open("FITXER.txt", ofstream::app);
	out.setf(ios::fixed);
	out.precision(1);
	out << A.total_points << endl;
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
    getline(in, nom, ';');    if (nom == "") break;
    getline(in, posicio, ';');
    in >> preu;
    char aux; in >> aux;
    getline(in, club, ';');
    in >> punts;
    string aux2;
    getline(in, aux2);
		if (preu <= j) {
			Player jugador(nom, posicio, preu, club, punts);
			PlayerDatabase[jugador.npos].push_back(jugador);
		}
  }
  in.close();
}

// Aquesta funcio deixa a ord la permutacio de {0,1,2,3} que respecta l'ordre
// del vector pond.
void ordre(vector<double>& pond, vector<int>& ord) {
	ord = {0, 1, 2, 3};
	for (int i = 0; i < (int)pond.size(); ++i) {
		int max_idx = i;
		for (int j = i; j < (int)pond.size(); ++j) {
			if (pond[j] > pond[max_idx]) max_idx = j;
		}
		swap(ord[i], ord[max_idx]);
		swap(pond[i], pond[max_idx]);
	}
}


void Greedy(Alignment& S) {

	for (int x = 0; x < 4; ++x) {
		for (Player jugador: PlayerDatabase[x]) {
			mu_tot += jugador.price;
		}
		num_tot += PlayerDatabase[x].size();
	}
	mu_tot /= num_tot;

	// El vector pond calcula una mitjana ponderada de les bases de dades per
	// decidir per quina posicio començarem a omplir l'alineacio
	vector<double> pond(4, 0);
  for (int k = 0; k < 4; ++k) {
		double a = 0.2, b = 1;
    sort(PlayerDatabase[k].begin(), PlayerDatabase[k].end());
		for (int i = 0; i < (int)PlayerDatabase[k].size(); ++i) {
			pond[k] += PlayerDatabase[k][i].points*b;
			b *= a;
		}
	}

	vector<int> ord(pond.size());
	ordre(pond, ord);


	vector<int> n = {1, n1, n2, n3};
  for (int database_idx: ord) {
		int pos = 0;
    for (int i = 0; i < n[database_idx] and pos < (int)PlayerDatabase[database_idx].size();) {
      if (PlayerDatabase[database_idx][pos].price <= t) {
        S.addPlayer(PlayerDatabase[database_idx][pos], database_idx);
        t -= PlayerDatabase[database_idx][pos].price;
				++i;
      }
			++pos;
    }
  }
}

// ************ FUNCIO MAIN **************

int main(int argc, char** argv) {
  // En cas de no proporcionar-se tots els fitxers d'entrada salta un error.
	if (argc != 4) {
	    cout << "Syntax: " << argv[0] <<
      " [data_base_file_name] [query_file_name] [output_file_name]" << endl;
	    exit(1);
	}

  // Llegim les dades d'entrada a traves dels fitxers proporcionats.
	string input_file_name = argv[1], query_file_name = argv[2],
          output_file_name = argv[3];
	read_query(query_file_name);
	read_database(input_file_name);

	start_time = clock(); // Iniciem el cronometre.

  // Deduim quina es la millor alineacio que podem trobar.
	Alignment bestTeam(n1, n2, n3);
  Greedy(bestTeam);
  // Escrivim la solucio en el fitxer de sortida.
  write(output_file_name, bestTeam);
}
