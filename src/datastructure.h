#ifndef MIDDLEBOX_PLACEMENT_SRC_DATASTRUCTURE_H_
#define MIDDLEBOX_PLACEMENT_SRC_DATASTRUCTURE_H_

#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <map>
#include <memory>
#include <stdlib.h>

#define INF 9999999
#define MAXN 1000
#define NIL -1

struct middlebox {
  std::string middlebox_name;
  int cpu_requirement;
  int processing_delay;
  int processing_capacity;
  double deployment_cost; // Hourly cost of running a middlebox.
  middlebox(const std::string &mb_name, const std::string &mb_cpu,
            const std::string &mb_delay, const std::string &mb_capacity,
            const std::string &mb_cost)
      : middlebox_name(mb_name), cpu_requirement(atoi(mb_cpu.c_str())),
        processing_delay(atoi(mb_delay.c_str())),
        processing_capacity(atoi(mb_capacity.c_str())),
        deployment_cost(atof(mb_cost.c_str())) {}
  std::string GetDebugString() {
    return "middlebox_name : " + middlebox_name + ", cpu_requirement : " +
           std::to_string(cpu_requirement) + ", processing_delay : " +
           std::to_string(processing_delay) + ", processing_capacity : " +
           std::to_string(processing_capacity) + ", deployment_cost : " +
           std::to_string(deployment_cost);
  }
};

struct traffic_class {
  std::string class_name;
  int min_bandwidth;    // In Mbps.
  int max_delay;        // In miliseconds.
  double delay_penalty; // Penalty for per 1 milisecond gurantee violation.
  // double bw_penalty;  // Not used now. For future use.
  traffic_class(const std::string &tc_name, const std::string &tc_min_bandwidth,
                const std::string &tc_max_delay,
                const std::string &tc_delay_penalty)
      : class_name(tc_name), min_bandwidth(atoi(tc_min_bandwidth.c_str())),
        max_delay(atoi(tc_max_delay.c_str())),
        delay_penalty(atof(tc_delay_penalty.c_str())) {}
  std::string GetDebugString() {
    return "class_name : " + class_name + ", min_bandwidth : " +
           std::to_string(min_bandwidth) + ", max_delay : " +
           std::to_string(max_delay) + ", delay_penalty : " +
           std::to_string(delay_penalty);
  }
};

struct traffic_request {
  int arrival_time;
  int source, destination;

  // Deprecated
  int sla_specification;

  // Minimum bandwidth demand in Kbps.
  int min_bandwidth;

  // Maximum delay according to the SLA.
  int max_delay;

  // Penalty for per 1ms guarantee violation.
  double delay_penalty;
  std::vector<int> middlebox_sequence;
  traffic_request(const std::string &tr_arrival_time,
                  const std::string &tr_source, const std::string &tr_dest,
                  const std::string &tr_min_bandwidth,
                  const std::string &tr_max_delay,
                  const std::string &tr_delay_penalty,
                  const std::vector<int> &tr_mbox_seq)
      : arrival_time(atoi(tr_arrival_time.c_str())),
        source(atoi(tr_source.c_str())), destination(atoi(tr_dest.c_str())),
        min_bandwidth(atoi(tr_min_bandwidth.c_str())),
        max_delay(atoi(tr_max_delay.c_str())),
        delay_penalty(atof(tr_delay_penalty.c_str())),
        middlebox_sequence(tr_mbox_seq) {}
  std::string GetDebugString() {
    std::string seq_string;
    for (auto value : middlebox_sequence) {
      seq_string += std::to_string(value) + " ";
    }
    return "source : " + std::to_string(source) + ", destination : " +
           std::to_string(destination) + ", min_bandwidth : " +
           std::to_string(min_bandwidth) + ", max_delay : " +
           std::to_string(max_delay) + ", delay_penalty : " +
           std::to_string(delay_penalty) + ", middlebox_sequence_length : " +
           std::to_string(middlebox_sequence.size()) +
           ", middlebox_sequence: " + seq_string;
  }
};

struct node {
  int node_id;
  int num_cores;
  int residual_cores;
  std::string GetDebugString() {
    return "node_id : " + std::to_string(node_id) + ", num_cores : " +
           std::to_string(num_cores);
  }
};

struct edge_endpoint {
  node *u;
  int bandwidth;
  int delay;
  int residual_bandwidth;
  edge_endpoint(node *u_ptr, int bw, int del)
      : u(u_ptr), bandwidth(bw), delay(del), residual_bandwidth(bw) {}
};

struct resource {
  std::vector<int> cpu_cores;
};

// Statistics for each traffic embedding.
struct traffic_statistics {
  // Arrival time of the traffic request.
  int arrival_time;

  // Energy cost of embedding the traffic request.
  double cost;

  traffic_statistics(int a_time, double c) : arrival_time(a_time), cost(c) {}
};

// Statistics for the whole solution.
struct solution_statistics {
  // Start time of the simulation in nano seconds.
  unsigned long long start_time;

  // End time of the simulation in nano seconds.
  unsigned long long end_time;

  // The number of accepted and rejected embeddings.
  int num_accepted, num_rejected;
  std::vector<traffic_statistics> t_stats;
};

// Global data structures;
extern std::vector<middlebox> middleboxes;
extern std::vector<traffic_class> traffic_classes;
extern std::vector<traffic_request> traffic_requests;
extern std::vector<node> nodes;
extern std::vector<std::vector<edge_endpoint> > graph;
extern std::map<std::pair<int, int>, std::unique_ptr<std::vector<int> > >
    path_cache;
extern solution_statistics stats;
extern double per_core_cost, per_bit_transit_cost;
extern double cost[MAXN][MAXN];
extern int pre[MAXN][MAXN];
extern int shortest_path[MAXN][MAXN], sp_pre[MAXN][MAXN];
#endif // MIDDLEBOX_PLACEMENT_SRC_DATASTRUCTURE_H_
