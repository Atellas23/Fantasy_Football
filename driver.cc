#include <bits/stdc++.h>
using namespace std;

bool exists(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int main() {
  if (exists("FITXER.txt")) system("mv FITXER.txt FITXER2.txt");
  system("make compile name=projecte/greedy");
  string database = "projecte/data_base.txt";
  string output = "projecte/output.txt";
  vector<string> queries = {
    "projecte/public_benchs/easy-",
    "projecte/public_benchs/med-",
    "projecte/public_benchs/hard-"
  };
  //cout << database + queries[1] << endl;
  for (int i = 0; i < 3; ++i) {
    string command = "./projecte/greedy.out ";
    command += database + " ";
    string new_command = command + queries[i];
    for (int j = 1; j < 8; ++j) {
      string new_new_command = new_command + to_string(j) + ".txt " + output;
      cout << new_new_command << endl;
      system(new_new_command.c_str());
    }
  }
  system("pr -m -t FITXER.txt FITXER2.txt");
  /*
  vector<int> points1, points2;
  ifstream in("FITXER.txt");
  int p, tot1 = 0, tot2 = 0;
  while (not in.eof()) {
    in >> p;
    points1.push_back(p);
    tot1 += p;
  }
  in.close();
  in.open("FITXER2.txt");
  while (not in.eof()) {
    in >> p;
    points2.push_back(p);
    tot2 += p;
  }
  double div = 0;
  for (int k = 0; k < (int)points1.size(); ++k) div += (double(points1[k])/tot1)*log((double(points1[k])/tot1)/(double(points2[k])/tot2));
  cout << "The Kullback-Leibler Divergence D(\"FITXER.TXT\"||\"FITXER2.TXT\") = " << div << ".\n";
  */
}
