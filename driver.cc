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
      //cout << new_new_command << endl;
      system(new_new_command.c_str());
    }
  }
  /*
  vector<int> points1, points2;
  ifstream in1("FITXER.txt"), in2("FITXER2.txt");
  int p;
  while (not in1.eof()) {
    in1 >> p;
    points1.push_back(p);
    in2 >> p;
    points2.push_back(p);
  }
  in1.close(); in2.close();
  int div = 0;
  for (int k = 0; k < (int)points1.size(); ++k) div += points1[k] - points2[k];
  cout << "Testing on comparator:\n";
  system("grep COMP projecte/greedy.cc");
  cout << endl;
  system("pr -m -t FITXER.txt FITXER2.txt");
  cout << "\nThe data in \"FITXER.txt\" is " << div << " points better (in total) than the data in \"FITXER2.txt\".\n";

  int easy_div = 0, med_div = 0, hard_div = 0;
  for (int i = 0; i < 7; ++i) {
    easy_div += points1[i] - points2[i];
    med_div += points1[i + 7] - points2[i + 7];
    hard_div += points1[i + 14] - points2[i + 14];
  }
  cout << "And it is " << easy_div << " points better considering only the easy queries." << endl;
  cout << "And it is " << med_div  << " points better considering only the medium queries." << endl;
  cout << "And it is " << hard_div << " points better considering only the hard queries." << endl;

  */
}
