#include "../include/display.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

namespace Display {

    void printHeader(const string& title) {
        cout << "\n========================================================================================\n";
        cout << " " << title << "\n";
        cout << "========================================================================================\n";
    }

    void printProcessInitTable(const string& npm, const vector<Models::Process>& proc_list) {
        cout << "\n[INFORMASI DATA PROSES DARI NPM]\n";
        cout << "NPM Mahasiswa : " << npm << "\n";
        cout << "Jumlah Digit  : " << npm.length() << " digit (" << npm.length() << " Proses: P1 s.d P" << npm.length() << ")\n\n";

        cout << "+------------+----------------+--------------+------------------+-----------------+\n";
        cout << "| Process ID | Digit NPM (AT) | Arrival (AT) | Digit Asli (BT)  | Burst Final(BT) |\n";
        cout << "+------------+----------------+--------------+------------------+-----------------+\n";
        for (const auto& p : proc_list) {
            cout << "| " << setw(10) << left << p.name
                 << " | " << setw(14) << right << p.at
                 << " | " << setw(12) << right << p.at
                 << " | " << setw(16) << right << p.raw_bt
                 << " | " << setw(15) << right << p.bt 
                 << (p.raw_bt == 0 ? " (diubah)" : "         ") << " |\n";
        }
        cout << "+------------+----------------+--------------+------------------+-----------------+\n";
        cout << "*Keterangan: Sesuai aturan, nilai Burst Time 0 diubah menjadi 10.\n";
    }

    void printGanttChart(const vector<Models::GanttSegment>& gantt) {
        if (gantt.empty()) return;

        cout << "\n--- GANTT CHART ---\n";
        
        for (const auto& seg : gantt) cout << "+---------";
        cout << "+\n";

        for (const auto& seg : gantt) cout << "| " << setw(7) << left << seg.name;
        cout << "|\n";

        for (const auto& seg : gantt) cout << "+---------";
        cout << "+\n";

        cout << setw(10) << left << gantt[0].start_time;
        for (const auto& seg : gantt) cout << setw(10) << left << seg.end_time;
        cout << "\n\n";
    }

    void printHRRNStepTable(int step, int current_time, const vector<Models::CandidateRR>& candidates, int selected_idx) {
        cout << "\n>>> [Tahap Pemilihan ke-" << step << "] Saat Waktu (t) = " << current_time << "\n";
        cout << "+------------+--------------+------------+-----------------+---------------------------+----------------+\n";
        cout << "| Process ID | Arrival (AT) | Burst (BT) | Waktu Tunggu WT | Rumus: (WT + BT) / BT     | Response Ratio |\n";
        cout << "+------------+--------------+------------+-----------------+---------------------------+----------------+\n";

        string chosen_name = "";
        double chosen_rr = 0.0;

        for (const auto& c : candidates) {
            string formula_str = "(" + to_string(c.wt) + " + " + to_string(c.bt) + ") / " + to_string(c.bt);
            bool is_picked = (c.idx == selected_idx);
            if (is_picked) {
                chosen_name = c.name;
                chosen_rr = c.rr;
            }

            cout << "| " << setw(10) << left << c.name
                 << " | " << setw(12) << right << c.at
                 << " | " << setw(10) << right << c.bt
                 << " | " << setw(15) << right << c.wt
                 << " | " << setw(25) << left << formula_str
                 << " | " << setw(11) << right << fixed << setprecision(4) << c.rr
                 << (is_picked ? " * " : "   ") << "|\n";
        }
        cout << "+------------+--------------+------------+-----------------+---------------------------+----------------+\n";
        cout << ">> Hasil: " << chosen_name << " terpilih karena memiliki Response Ratio tertinggi (" 
             << fixed << setprecision(4) << chosen_rr << ")\n";
    }

    void printAlgorithmResult(const Models::AlgorithmResult& res) {
        printHeader("HASIL PERHITUNGAN: " + res.algo_name);

        cout << "Urutan Eksekusi : ";
        for (size_t i = 0; i < res.execution_order.size(); ++i) {
            cout << res.execution_order[i];
            if (i + 1 < res.execution_order.size()) cout << " -> ";
        }
        cout << "\n";

        printGanttChart(res.gantt_chart);

        cout << "+------------+--------------+------------+-----------------+-------------------+--------------+\n";
        cout << "| Process ID | Arrival (AT) | Burst (BT) | Completion (CT) | Turn Around (TAT) | Waiting (WT) |\n";
        cout << "+------------+--------------+------------+-----------------+-------------------+--------------+\n";

        for (const auto& p : res.processes) {
            cout << "| " << setw(10) << left << p.name
                 << " | " << setw(12) << right << p.at
                 << " | " << setw(10) << right << p.bt
                 << " | " << setw(15) << right << p.ct
                 << " | " << setw(17) << right << p.tat
                 << " | " << setw(12) << right << p.wt
                 << " |\n";
        }
        cout << "+------------+--------------+------------+-----------------+-------------------+--------------+\n";
        cout << "| RATA-RATA  |              |            |                 | "
             << setw(17) << right << fixed << setprecision(2) << res.avg_tat
             << " | " << setw(12) << right << fixed << setprecision(2) << res.avg_wt
             << " |\n";
        cout << "+------------+--------------+------------+-----------------+-------------------+--------------+\n";
        cout << "Rata-rata Turn Around Time (ATAT) = " << fixed << setprecision(2) << res.avg_tat << "\n";
        cout << "Rata-rata Waiting Time     (AWT)  = " << fixed << setprecision(2) << res.avg_wt << "\n\n";
    }

    void printComparisonTable(const Models::AlgorithmResult& fcfs, const Models::AlgorithmResult& sjf, const Models::AlgorithmResult& hrrn) {
        printHeader("TABEL PERBANDINGAN KINERJA ALGORITMA PENJADWALAN");
        
        cout << "+-------------------------------------------+-------------------------+-------------------------+\n";
        cout << "| Algoritma Penjadwalan                     | Rata-rata Waiting Time  | Rata-rata Turnaround    |\n";
        cout << "+-------------------------------------------+-------------------------+-------------------------+\n";
        cout << "| " << setw(41) << left << fcfs.algo_name 
             << " | " << setw(23) << right << fixed << setprecision(2) << fcfs.avg_wt
             << " | " << setw(23) << right << fixed << setprecision(2) << fcfs.avg_tat << " |\n";
        cout << "| " << setw(41) << left << sjf.algo_name 
             << " | " << setw(23) << right << fixed << setprecision(2) << sjf.avg_wt
             << " | " << setw(23) << right << fixed << setprecision(2) << sjf.avg_tat << " |\n";
        cout << "| " << setw(41) << left << hrrn.algo_name 
             << " | " << setw(23) << right << fixed << setprecision(2) << hrrn.avg_wt
             << " | " << setw(23) << right << fixed << setprecision(2) << hrrn.avg_tat << " |\n";
        cout << "+-------------------------------------------+-------------------------+-------------------------+\n\n";

        cout << ">>> KESIMPULAN PERBANDINGAN:\n";
        double min_wt = min({fcfs.avg_wt, sjf.avg_wt, hrrn.avg_wt});
        cout << "- Rata-rata Waiting Time Terendah : ";
        if (fabs(sjf.avg_wt - min_wt) < 1e-6) cout << "SJF Non-Preemptive (" << fixed << setprecision(2) << sjf.avg_wt << ")\n";
        else if (fabs(hrrn.avg_wt - min_wt) < 1e-6) cout << "HRRN Non-Preemptive (" << fixed << setprecision(2) << hrrn.avg_wt << ")\n";
        else cout << "FCFS (" << fixed << setprecision(2) << fcfs.avg_wt << ")\n";

        double min_tat = min({fcfs.avg_tat, sjf.avg_tat, hrrn.avg_tat});
        cout << "- Rata-rata Turnaround Terendah   : ";
        if (fabs(sjf.avg_tat - min_tat) < 1e-6) cout << "SJF Non-Preemptive (" << fixed << setprecision(2) << sjf.avg_tat << ")\n";
        else if (fabs(hrrn.avg_tat - min_tat) < 1e-6) cout << "HRRN Non-Preemptive (" << fixed << setprecision(2) << hrrn.avg_tat << ")\n";
        else cout << "FCFS (" << fixed << setprecision(2) << fcfs.avg_tat << ")\n";

        cout << "\nCatatan Teoretis:\n";
        cout << "1. SJF Non-Preemptive secara teoritis menghasilkan AWT terkecil jika tidak ada proses baru yang datang mengganggu.\n";
        cout << "2. HRRN Non-Preemptive mencegah starvation dengan rumus aging: Response Ratio = (WT + BT) / BT.\n";
        cout << "3. FCFS bekerja sederhana sesuai waktu kedatangan, namun berisiko mengalami Convoy Effect.\n";
        cout << "========================================================================================\n";
    }

} // namespace Display
