#include <iostream>
#include <vector>
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
	int id, n1, n2, n3, total_points, price;

  Alignment(int n1, int n2, int n3, int tot = 0, int pr = 0):
    aln(vector<Player>(11, Player("", "", -1, "", -1))), id(getAlignmentId()), n1(n1), n2(n2), n3(n3), total_points(tot), price(pr) {}


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

int main() {
  Player J("Aleix", "mig", 1200, "FME", 1);
  Alignment dreamteam(2, 3, 5);
  dreamteam.addPlayer(J, 0);
  cout << dreamteam[0].name << endl;
}
