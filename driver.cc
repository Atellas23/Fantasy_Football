#include <bits/stdc++.h>
using namespace std;

/*bool exists(const string& name) {
    ifstream f(name.c_str());
    return f.good();
*/

int main() {
  system("make compile name=projecte/mh");
  string database = "projecte/diff_benchs/data_base";
  string output = "projecte/mh_outputs/output";
  string query = "projecte/diff_benchs/query";
  system(" > projecte/mh_outputs/summary_so_far.txt");
  for (int i = 1; i < 12; ++i) {
    string command = "./projecte/mh.out ";
    string where = output + to_string(i) + ".txt";
    command += database + to_string(i) + ".txt "
    + query + to_string(i) + ".txt "
    + where;
    system(command.c_str());
    system(("echo projecte/query" + to_string(i) + ".txt >> projecte/mh_outputs/summary_so_far.txt").c_str());
    string command2 = "grep \"Punts:\" " + where + " >> projecte/mh_outputs/summary_so_far.txt";
    system(command2.c_str());
    //system("echo \n >> summary_so_far.txt");
  }
  system("pr -m -t projecte/mh_outputs/outputs_max.txt projecte/mh_outputs/summary_so_far.txt");
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
