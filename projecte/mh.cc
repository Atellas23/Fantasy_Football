#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cmath>

// Aplicarem la metaheurística GRASP al nostre problema.

// Importem la llibreria random i cstdlib, fonamentals per a
// l'esquema metaheurístic que hem decidit aplicar.
#include <random>
#include <cstdlib>

using namespace std;

// Definim els parametres d'entrada de les consultes com a variables globals.
int n1, n2, n3, t, j;
vector<int> num_pos;
clock_t start_time;

// Tambe definim el tamany total de la base de dades i la mitjana de preu
// com a variables globals.
int num_tot = 0;
double mu_tot = 0;

/* FUNCIO getpos
  - Donat un string amb la posicio d'un jugador, la retorna amb numero.
*/
int getpos(string& pos) {
	if (pos == "por") return 0;
	else if (pos == "def") return 1;
	else if (pos == "mig") return 2;
	else if (pos == "dav") return 3;
	// No hauria de passar, tot i així el posem.
	return -1;
}


/*    STRUCT PLAYER
- Conte la informacio basica del jugador (nom, club, posicio, preu i punts).
- Conte informacio afegida per realitzar tasques mes facilment (npos).
*/
struct Player {
	// PARAMETRES
	string name, pos, club;
	int npos, price, points;

	// CONSTRUCTOR
	Player(string name, string pos, int price, string club, int points):
    	name(name), pos(pos), club(club),	npos(getpos(pos)),
			price(price), points(points) {}

  /* OPERADOR <
		- Ordre entre els jugdors, utilitza els punts i el preu que tenen.
	*/
	bool operator< (const Player& J) {
		int a = j - price;
		int b = j - J.price;

		if (a != 0 and b != 0) {
			return (1.5*double(points)   - 0.8*1e8*double(1)/(a)) >
						 (1.5*double(J.points) - 0.8*1e8*double(1)/(b));

		} else if (a != 0) {
			return (1.5*double(points)   - 0.8*1e8*double(1)/(a)) >
						 (1.5*double(J.points) - 0.8*1e8*double(1)/(1 + b));

		} else if (b != 0) {
			return (1.5*double(points)   - 0.8*1e8*double(1)/(1 + a)) >
						 (1.5*double(J.points) - 0.8*1e8*double(1)/(b));

		} else {
			return (1.5*double(points)   - 0.8*1e8*double(1)/(1 + a)) >
						 (1.5*double(J.points) - 0.8*1e8*double(1)/(1 + b));

		}
	}
};

/* BASE DE DADES dels jugadors
	- Podria ser una struct per si mateixa pero només consisteix en 4
	  vectors de jugadors.

		Cada vector conte unicament els jugadors disponibles d'aquella posicio.
*/
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
	// PARAMETRES
	vector< vector<Player> > aln;
	int n1, n2, n3, total_points, total_price;

	// CONSTRUCTOR
	Alignment (int n1, int n2, int n3, int totP = 0, int pr = 0):
  	aln(vector< vector<Player> >(4)), n1(n1), n2(n2), n3(n3),
    total_points(totP), total_price(pr) {}
	// CONSTRUCTOR COPIA
	Alignment(const Alignment& source):
		aln(source.aln), n1(source.n1), n2(source.n2), n3(source.n3),
		total_points(source.total_points), total_price(source.total_price) {}

	// OPERADOR
	vector<Player>& operator[] (int idx) {return aln[idx];}

	/* METODE add_player
		- Requereix un jugador i la posicio en la que el vols possar.
		- Actualitza els punts i el preu de l'equip.
	*/
	void add_player (const Player& J, int x) {
		total_points += J.points;
		total_price  += J.price;
  	aln[x].push_back(J);
	}

	void change_player (const Player& J, int x, int i) {
		total_points -= aln[x][i].points;
		total_price -= aln[x][i].price;
		aln[x][i] = J;
		total_points += aln[x][i].points;
		total_price += aln[x][i].price;
	}
};

/* FUNCIO write
 - Requereix del nom del fitxer de sortida i l'alineacio a guardar.
 - Funcio que imprimeix en un fitxer una alineacio, en el format demanat a
	 l'enunciat del projecte.
 */
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
}

/* FUNCIO read_query
	- Requereix del fitxer de les dades d'entrada.
  - Llegeix i guarda en memoria els parametres d'entrada.
*/
void read_query(string& filename) {
	ifstream in(filename);
	in >> n1 >> n2 >> n3 >> t >> j;
	num_pos = {1, n1, n2, n3};
}

/* FUNCIO read_database
 	- Requereix del fitxer de la base de dades dels jugadors.
  - Llegeix del fitxer un llistat de jugadors i els col·loca
 	  a la base de dades de jugadors que consta de 4 vectors de jugadors.
*/
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
			++num_tot;
		}
  }
  in.close();
}

// ******** METAHEURISTICA: PRELIMINARS **************

vector<vector<int>> permutations;
vector<int> vec;
vector<bool> used;


// ********* GRASP: FASE 1 *******************

/* FUNCIO determine_candidate_list_length
	- No requereix de cap parametre

	- Retorna un vector de 4 elements amb el nombre de jugadors
		que tindra la llista de candidats.

*/
vector<int> determine_candidate_list_length() {
	vector<int> v;

	// alfa pels porters
	v.push_back(1+PlayerDatabase[0].size()/50);
	// alfa pels defenses
	v.push_back(n1+PlayerDatabase[1].size()/50);
	// alfa pels migcampistes
	v.push_back(n2+PlayerDatabase[2].size()/50);
	// alfa pels davanters
	v.push_back(n3+PlayerDatabase[3].size()/50);
	return v;
}

/* FUNCIO restricted_candidate_list
	- Requereix de la posicio dels jugadors x, i alpha
		que es el nombre maxim d'elements que tindra la llista
		de candidats.

	- Retorna una llista de jugadors el preu dels quals es
	  inferior al pressupost actual restant.

*/
vector<int> restricted_candidate_list(int x, int alpha) {
	vector<int> v;
	int i = 0;
	while (alpha > 0 and i < (int)PlayerDatabase[x].size()) {
		if (PlayerDatabase[x][i].price <= t) {
			v.push_back(i);
			--alpha;
		}
		++i;
	}

	return v;
}

/* FUNCIO select_element_at_random
	- Requeix de la posicio dels jugadors x i la llista
	  restringida de jugadors candidats, RCL.

	- Escull aleatoriament un jugador de RCL, amb probabilitat
  	punts/suma(punts de tots els jugadors).
*/

Player select_element_at_random(int x, vector<int>& RCL) {
	default_random_engine gen;
	vector<double> weights;
	// Omplim el vector weights amb els scores
	// donats a cada membre de la RCL.
	for (int i = 0; i < (int)RCL.size(); ++i) {
		weights.push_back(PlayerDatabase[x][RCL[i]].points);
	}
	// Fabriquem la funcio de distribucio.
	discrete_distribution<int> d(weights.begin(), weights.end());
	/*
	Cal notar que la distribucio discrete_distribution<int> retorna un
	nombre entre 0 i n-1, on n es el nombre d'elements en el rang
	(weights.begin(), weights.end()), amb l'element i tenint probabilitat
	weights[i]/suma(weights).
	*/
	// Escollim un jugador a l'altzar
	int chosen_at_random = d(gen);

	// Actualitzem el pressupost.
	t -= PlayerDatabase[x][RCL[chosen_at_random]].price;

	// Retorna el jugador.
	return PlayerDatabase[x][RCL[chosen_at_random]];
}

/* FUNCIO construct_greedy_randomized_solution
	- Requeix de l'index de la permutacio que fara servir.

	- Es la funcio principal de la FASE 1. Construeix una
	  Alineacio inicial que aproxima la solucio.

*/
Alignment construct_greedy_randomized_solution(int id_perm) {
  Alignment s(n1, n2, n3);
  vector<int> alpha = determine_candidate_list_length();
	for (int x: permutations[id_perm]) {
		for (int i = 0; i < num_pos[x]; ++i) {
			vector<int> RCL = restricted_candidate_list(x, alpha[x]);
			Player P = select_element_at_random(x, RCL);
			s.add_player(P, P.npos);
		}
	}
	return s;
}

// ************ CERCA LOCAL ****************

/* FUNCIO Basic_local_search
	- Requereix d'una solucio inicial s per millorar.

	- Quan troba una millora, la aplica i para.
*/
bool Basic_local_search(Alignment& s) {
	int r = rand() % 24; // Nombre aleatori del 0 al 23.

	// Primer permuta per cada posicio.
	for (int x: permutations[r]) {
		// Segon permuta pels primers jugadors de la base de dades.
		for (int i = 0; i < (int)PlayerDatabase[x].size()/100; ++i) {
			Player P = PlayerDatabase[x][i];
			for (int j = 0; j < num_pos[x]; ++j) {
				if (((P.points >= s[x][j].points and P.price < s[x][j].price + t) or
				    (P.points > s[x][j].points and P.price <= s[x][j].price + t))
						and (P.name != s[x][j].name)) {
						t -= P.price;
						t += s[x][j].price;
						s.change_player(P, x, j);
						return true;
				}
			}
		}
	}

	return false;
}


/* FUNCIO improve
	- Requereix d'una solucio inicial s per millorar.

	- Es tracte d'una cerca local (semblant a Simulated Annealing)
	  la qual canvia l'alineacio donada per, o bé, una
		alineacio estrictament millor, o bé, amb una petita probabilitat,
		amb una alineacio una mica pitjor, per aixi poder-se escapar dels
		optims locals i poder aproximar-se a un optim global.

*/
bool improve (Alignment& s) {
	int r = rand() % 24; // Nombre aleatori del 0 al 23

	// Primer itera per cada posicio.
	for (int x: permutations[r]) {
		// Segon itera sobre els primers jugadors de la base de dades.
		for (int i = 0; i < (int)PlayerDatabase[x].size()/100; ++i) {
			Player P = PlayerDatabase[x][i];
			for (int j = 0; j < num_pos[x]; ++j) {
				if (((P.points >= s[x][j].points and P.price < s[x][j].price + t) or
				    (P.points > s[x][j].points and P.price <= s[x][j].price + t))
						and (P.name != s[x][j].name)) {
						t -= P.price;
						t += s[x][j].price;
						s.change_player(P, x, j);
						return true;
				}
				// Per escapar dels minims locals,
				// posem que es pot empitjorar una
				// solucio el 5.00% de les vegades.
				if (P.price <= s[x][j].price*0.9 and P.points >= s[x][j].points*0.9) {
					int prob = rand() % 1000;
					if (prob > 949) {
						t -= P.price;
						t += s[x][j].price;
						s.change_player(P, x, j);
						return true;
					}
				}
			}
		}
	}

	return false;
}


/* FUNCIO local_search
	- Requereix una primera aproximacio de solucio per millorar.

	- Crida un nombre fitat de vegades a la funcio BasicLocalSearch
	  o improve per millorar la aproximacio donada.
*/
void local_search(Alignment& s) {
	Alignment best = s;

	/*
	for (int i = 0; Basic_local_search(s) and i < 1000; ++i) {
		if (s.total_points > best.total_points) best = s;
	}
	*/

	for (int i = 0; improve(s) and i < 500; ++i) {
		if (s.total_points > best.total_points) best = s;
	}

	s = best;
}

// ************ GRASP: FASE 2 *********************

/* FUNCIO rec
	- Requereix d'un index posicio i d'un identificador id, ambdos enters.
	- pos: guarda fins a quina posicio esta construida una permutacio nova a vec.
	- id: guarda per quina permutacio anem.

	- Aquesta funcio emplena amb les 24 permutacions de {0,1,2,3}
	  la matriu permutations.

	- Nota: es un backtracking normal.
*/
void rec(int pos, int& id) {
	if (pos == 4) {
		permutations[id] = vec;
		++id;
		return;
	}

	for (int i = 0; i < 4; ++i) {
		if (not used[i]) {
			used[i] = true;
			vec[pos] = i;
			rec(pos + 1, id);
			used[i] = false;
		}
	}
}

/* FUNCIO fill_permutations
	- No requereix de cap parametre.
	- Aquesta funcio fa els preparatius per emplenar la matriu permutations amb
	  les permutacions de {0,1,2,3}. Posteriorment crida a la funcio rec que
		les calcula i les guarda.
*/
void fill_permutations() {
	// Matriu permutations: a cada fila guarda una permutacio de {0,1,2,3}.
	permutations = vector<vector<int>> (24, vector<int> (4));
	// Vector auxiliar vec: guarda cada permutacio mentre es construeix.
	vec = vector<int> (4);
	// Vector auxiliar used: guarda quins nombres hi ha al vector vec.
	used = vector<bool> (4, false);
	// Enter id: guarda quantes permutacions s'ha trobat en cada moment.
	int id = 0;

	// Crida a la funcio que emplena la matriu de pemutacions.
	rec(0, id);
}

/* FUNCIO metaheuristic
	- Requereix l'alineacio de la qual es vol maximitzar el nombre de punts.

	- Aquesta funcio segueix un algoritme GRASP de la seguent manera:
		- Es genera totes les permutacions de {0,1,2,3}: aquestes seran l'ordre
		  que seguira la funcio construct_greedy_randomized_solution per
			realitzar la construccio d'una primera aproximacio.
		- Aleshores per cadascuna de les permutacions, genera la primera
		  aproximacio ja esmentada, a continuacio la millora amb un algoritme
			de cerca local.
		- Es guarda sempre la millor en el parametre d'entrada.

	- Intencio:
		- D'aquesta manera, l'algoritme genera 24 solucions força diferents
		  on cada una prioritza mes unes posicions que altres. Aixi quan la
			funcio local_search aconsegueixi un minim local, aquests estaran
			distribuits i hi ha mes probabilitat d'obtenir solucions millors.

*/
void metaheuristic(Alignment& bestTeam) {
	// Alineacio auxiliar.
	Alignment s(n1, n2, n3);

	// Crida a la funcio que emplena la matriu de permutacions.
	fill_permutations();

	// Ordenem cadascuna de les bases de jugadors.
	for (int k = 0; k < 4; ++k)
		sort(PlayerDatabase[k].begin(), PlayerDatabase[k].end());

	// Ens guardem el pressupost, per anar-lo reestablint cada iteracio.
	int budget = t;

	// Itera per cada permutacio de {0,1,2,3}.
  for (int i = 0; i < 24; ++i) {
		t = budget; // Reestableix el pressupost al inicial.
    s = construct_greedy_randomized_solution(i); // Genera primera aproximacio.
    local_search(s); // Millora l'aproximacio i la guarda si es la millor.
    if (s.total_points > bestTeam.total_points) bestTeam = s;
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
	metaheuristic(bestTeam);

	// Escrivim la solucio en el fitxer de sortida.
  write(output_file_name, bestTeam);

}
