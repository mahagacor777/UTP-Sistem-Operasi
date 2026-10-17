#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>
#include "process.h"

namespace Display {

    void printHeader(const std::string& title);
    void printProcessInitTable(const std::string& npm, const std::vector<Models::Process>& proc_list);
    void printGanttChart(const std::vector<Models::GanttSegment>& gantt);
    void printHRRNStepTable(int step, int current_time, const std::vector<Models::CandidateRR>& candidates, int selected_idx);
    void printAlgorithmResult(const Models::AlgorithmResult& res);
    void printComparisonTable(const Models::AlgorithmResult& fcfs, const Models::AlgorithmResult& sjf, const Models::AlgorithmResult& hrrn);

} // namespace Display

#endif // DISPLAY_H
