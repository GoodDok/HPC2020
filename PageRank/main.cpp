#include "pagerank.h"
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <functional>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <omp.h>


using namespace std;

struct timing_result {
  double min, avg, max;
};

ostream &operator<<(ostream &strm, const timing_result result) {
  strm << "Min calc time in seconds = " << result.min << endl;
  strm << "Avg calc time in seconds = " << result.avg << endl;
  strm << "Max calc time in seconds = " << result.max << endl;
  return strm;
}

timing_result measure_timing(const function<void(void)> &f, int iterations) {
  struct timeval start{}, end{};
  double r_time;
  double min_runtime = 1e20, max_runtime = 0, sum_runtime = 0;
  for (int i = 0; i < iterations; ++i) {
    gettimeofday(&start, nullptr);
    f();
    gettimeofday(&end, nullptr);
    r_time = (double) (end.tv_sec - start.tv_sec) + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    if (r_time < min_runtime) {
      min_runtime = r_time;
    }
    if (r_time > max_runtime) {
      max_runtime = r_time;
    }
    sum_runtime += r_time;
  }
  return timing_result{min_runtime, sum_runtime / iterations, max_runtime};
}

void count_number_of_paths(int n, int iterations) {

  Matrix2D first = Matrix2D(n, n);
  Matrix2D second = Matrix2D(n, n);

  function<void(void)> custom_mult = [&first, &second]() { first * second; };
  auto timing = measure_timing(custom_mult, iterations);
  cout << "Custom implementation: " << endl << timing << endl;
}

long c(int n, int k) {
  if (n < k) {
    return 0;
  }
  long answer = 1;
  for (int i = n - k + 1; i <= n; i++) {
    answer *= i;
  }
  for (int i = 1; i <= k; i++) {
    answer /= i;
  }
  return answer;
}

struct football_match {
  string team_a;
  string team_b;
  int team_a_score;
  int team_b_score;
};

ostream &operator<<(ostream &strm, const football_match result) {
  strm << "Team A = " << result.team_a << endl;
  strm << "Team B = " << result.team_b << endl;
  strm << "Team A score = " << result.team_a_score << endl;
  strm << "Team B score = " << result.team_b_score << endl;
  return strm;
}

vector<football_match> get_matches_from_csv(string csv_path) {
    ifstream results_ifs(csv_path);
    vector<football_match> result;
    string line;
    string cell;

    while (getline(results_ifs, line)) {
       string team_a;
       string team_b;
       int team_a_score;
       int team_b_score;
       istringstream iss(line);
       for (int i = 0; i < 7; ++i) {
         getline(iss, cell, ',');
         if (i == 3) {
           team_a = cell;
         }
         if (i == 4) {
           team_b = cell;
         }
         if (i == 6) {
           team_a_score = stoi(cell.substr(0, 1));
           team_b_score = stoi(cell.substr(4, 1));
         }
       }
       result.push_back(football_match{team_a, team_b, team_a_score, team_b_score});
    }
    results_ifs.close();

    return result;
}

bool compare_pairs(const pair<string, double>& a, const pair<string, double>& b) { 
    return a.second > b.second; 
}

Matrix2D generate_graph_matrix(int N) {
  Matrix2D matrix = Matrix2D(N, N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      // generate 0, 1 or 2 with equal probability
      matrix.set(i, j, (int)(1.0 * rand() / RAND_MAX * 3));
    }
  }
  return matrix;
}


int main(int argc, char *argv[]) {
  cout << "1. Generate graph and test counter of number of paths" << endl;
  int N = 10;

  Matrix2D graph = Matrix2D(N, N); 
  for (int i = 0; i < N; ++i) {
    for (int j = i+1; j < N; ++j) {
      graph.set(i, j, 1);
    }
  }
  
 
  for (int pow = 1; pow < N; ++pow) {
    cout << "power = " << pow << endl;
    Matrix2D graphPowered = graph^pow;
    cout << graphPowered << endl;
    assert(graphPowered.get(0, N-1) == c(N-2, pow-1));
  }
   

  cout << "2. Generate small random (or select some graph) and implement "
       << "pagerank procedure. Compare with naive ranking" << endl;
  
  vector<football_match> results = get_matches_from_csv("epl-2019-GMTStandardTime.csv");
  unordered_map<string, int> team_idx;
  int teams_count = 20;
  int curr_idx = 0;
  Matrix2D teams_matrix = Matrix2D(teams_count, teams_count);
  
  for (auto i = results.begin(); i != results.end(); ++i) {
     string team_a = (*i).team_a;
     string team_b = (*i).team_b;
     if (team_idx.find(team_a) == team_idx.end()) {
       team_idx[team_a] = curr_idx++;
     }
     if (team_idx.find(team_b) == team_idx.end()) {
       team_idx[team_b] = curr_idx++;
     }
     teams_matrix.set(team_idx[team_a], team_idx[team_b], 
                      teams_matrix.get(team_idx[team_a], team_idx[team_b]) + (*i).team_a_score);
     teams_matrix.set(team_idx[team_b], team_idx[team_a], 
                      teams_matrix.get(team_idx[team_b], team_idx[team_a]) + (*i).team_b_score);
  }
  cout << "Total number of teams: " << curr_idx << endl;
  assert(teams_count == curr_idx);
  
  Matrix2D ranks_vector = find_pagerank(teams_matrix);
  
  vector<pair<string, double>> team_ranks; 
  for (auto it = team_idx.cbegin(); it != team_idx.cend(); ++it) {
    team_ranks.push_back(pair<string, double>{it->first, ranks_vector.get(it->second, 0)});
  }
  sort(team_ranks.begin(), team_ranks.end(), compare_pairs);
  for (auto i = team_ranks.begin(); i != team_ranks.end(); ++i) {
    cout << (*i).first << " --> " << (*i).second << endl;
  }


  cout << endl << "3. Try tricks for evaluation of performance" << endl;
  
  Matrix2D random_graph = generate_graph_matrix(4096);
  function<void(void)> custom_mult = [random_graph]() { 
     Matrix2D ranks = find_pagerank(random_graph); 
     cout << ranks.get_total_sum() << endl;
  };
 
  omp_set_num_threads(1); 
  auto timing = measure_timing(custom_mult, 5);
  cout << "Random graph non-parallelized: " << endl << timing << endl;
  
  omp_set_num_threads(4); 
  timing = measure_timing(custom_mult, 5);
  cout << "Random graph parallelized: " << endl << timing << endl;
   
  return 0;
}
