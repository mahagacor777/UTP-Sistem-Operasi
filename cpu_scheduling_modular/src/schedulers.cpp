#include "../include/schedulers.h"
#include "../include/display.h"
#include <algorithm>
#include <cmath>

using namespace std;

namespace Schedulers {

    static Models::AlgorithmResult finalizeResult(const string& algo_name, vector<Models::Process> proc_list, 
                                                 const vector<Models::GanttSegment>& gantt, const vector<string>& order) {
        Models::AlgorithmResult res;
        res.algo_name = algo_name;
        res.gantt_chart = gantt;
        res.execution_order = order;

        sort(proc_list.begin(), proc_list.end(), [](const Models::Process& a, const Models::Process& b) {
            return a.id < b.id;
        });

        double total_wt = 0, total_tat = 0;
        for (const auto& p : proc_list) {
            total_wt += p.wt;
            total_tat += p.tat;
        }

        res.processes = proc_list;
        res.avg_wt = total_wt / (double)proc_list.size();
        res.avg_tat = total_tat / (double)proc_list.size();
        return res;
    }

    // 1. FCFS
    Models::AlgorithmResult runFCFS(vector<Models::Process> proc_list) {
        int n = proc_list.size();
        vector<Models::GanttSegment> gantt;
        vector<string> order;

        sort(proc_list.begin(), proc_list.end(), [](const Models::Process& a, const Models::Process& b) {
            if (a.at != b.at) return a.at < b.at;
            return a.id < b.id;
        });

        int current_time = 0;
        for (int i = 0; i < n; ++i) {
            if (current_time < proc_list[i].at) {
                gantt.push_back({"IDLE", current_time, proc_list[i].at});
                current_time = proc_list[i].at;
            }

            proc_list[i].st = current_time;
            current_time += proc_list[i].bt;
            proc_list[i].ct = current_time;
            proc_list[i].tat = proc_list[i].ct - proc_list[i].at;
            proc_list[i].wt = proc_list[i].tat - proc_list[i].bt;

            order.push_back(proc_list[i].name);
            gantt.push_back({proc_list[i].name, proc_list[i].st, proc_list[i].ct});
        }

        return finalizeResult("First Come First Served (FCFS)", proc_list, gantt, order);
    }

    // 2. SJF Non-Preemptive
    Models::AlgorithmResult runSJF(vector<Models::Process> proc_list) {
        int n = proc_list.size();
        vector<bool> completed(n, false);
        int completed_count = 0;
        int current_time = 0;

        vector<Models::GanttSegment> gantt;
        vector<string> order;

        while (completed_count < n) {
            int selected_idx = -1;
            int min_bt = 1e9;

            for (int i = 0; i < n; ++i) {
                if (!completed[i] && proc_list[i].at <= current_time) {
                    if (proc_list[i].bt < min_bt) {
                        min_bt = proc_list[i].bt;
                        selected_idx = i;
                    } else if (proc_list[i].bt == min_bt) {
                        if (proc_list[i].at < proc_list[selected_idx].at) {
                            selected_idx = i;
                        } else if (proc_list[i].at == proc_list[selected_idx].at) {
                            if (proc_list[i].id < proc_list[selected_idx].id) {
                                selected_idx = i;
                            }
                        }
                    }
                }
            }

            if (selected_idx == -1) {
                int next_arrival = 1e9;
                for (int i = 0; i < n; ++i) {
                    if (!completed[i] && proc_list[i].at < next_arrival) {
                        next_arrival = proc_list[i].at;
                    }
                }
                gantt.push_back({"IDLE", current_time, next_arrival});
                current_time = next_arrival;
                continue;
            }

            proc_list[selected_idx].st = current_time;
            current_time += proc_list[selected_idx].bt;
            proc_list[selected_idx].ct = current_time;
            proc_list[selected_idx].tat = proc_list[selected_idx].ct - proc_list[selected_idx].at;
            proc_list[selected_idx].wt = proc_list[selected_idx].tat - proc_list[selected_idx].bt;

            completed[selected_idx] = true;
            completed_count++;

            order.push_back(proc_list[selected_idx].name);
            gantt.push_back({proc_list[selected_idx].name, proc_list[selected_idx].st, proc_list[selected_idx].ct});
        }

        return finalizeResult("Shortest Job First (SJF) Non-Preemptive", proc_list, gantt, order);
    }

    // 3. HRRN Non-Preemptive
    Models::AlgorithmResult runHRRN(vector<Models::Process> proc_list) {
        int n = proc_list.size();
        vector<bool> completed(n, false);
        int completed_count = 0;
        int current_time = 0;
        int step = 1;

        vector<Models::GanttSegment> gantt;
        vector<string> order;

        Display::printHeader("PERHITUNGAN RESPONSE RATIO SETIAP TAHAP PEMILIHAN PROSES (HRRN)\n Rumus: Response Ratio = (WT + BT) / BT, dengan WT = Waktu Sekarang - Arrival Time (AT)");

        while (completed_count < n) {
            vector<int> ready_indices;
            for (int i = 0; i < n; ++i) {
                if (!completed[i] && proc_list[i].at <= current_time) {
                    ready_indices.push_back(i);
                }
            }

            if (ready_indices.empty()) {
                int next_arrival = 1e9;
                for (int i = 0; i < n; ++i) {
                    if (!completed[i] && proc_list[i].at < next_arrival) {
                        next_arrival = proc_list[i].at;
                    }
                }
                gantt.push_back({"IDLE", current_time, next_arrival});
                current_time = next_arrival;
                continue;
            }

            int selected_idx = -1;
            double max_rr = -1.0;
            vector<Models::CandidateRR> candidates;

            for (int idx : ready_indices) {
                int wt_now = current_time - proc_list[idx].at;
                double rr = (double)(wt_now + proc_list[idx].bt) / (double)proc_list[idx].bt;
                candidates.push_back({idx, proc_list[idx].name, proc_list[idx].at, proc_list[idx].bt, wt_now, rr});

                bool choose_this = false;
                if (selected_idx == -1) {
                    choose_this = true;
                } else if (rr > max_rr + 1e-9) {
                    choose_this = true;
                } else if (fabs(rr - max_rr) <= 1e-9) {
                    if (proc_list[idx].at < proc_list[selected_idx].at) {
                        choose_this = true;
                    } else if (proc_list[idx].at == proc_list[selected_idx].at) {
                        if (proc_list[idx].id < proc_list[selected_idx].id) {
                            choose_this = true;
                        }
                    }
                }

                if (choose_this) {
                    max_rr = rr;
                    selected_idx = idx;
                }
            }

            Display::printHRRNStepTable(step, current_time, candidates, selected_idx);

            proc_list[selected_idx].st = current_time;
            current_time += proc_list[selected_idx].bt;
            proc_list[selected_idx].ct = current_time;
            proc_list[selected_idx].tat = proc_list[selected_idx].ct - proc_list[selected_idx].at;
            proc_list[selected_idx].wt = proc_list[selected_idx].tat - proc_list[selected_idx].bt;

            completed[selected_idx] = true;
            completed_count++;
            step++;

            order.push_back(proc_list[selected_idx].name);
            gantt.push_back({proc_list[selected_idx].name, proc_list[selected_idx].st, proc_list[selected_idx].ct});
        }

        return finalizeResult("Highest Response Ratio Next (HRRN) Non-Preemptive", proc_list, gantt, order);
    }

} // namespace Schedulers
