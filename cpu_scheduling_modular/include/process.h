#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

namespace Models {

    struct Process {
        int id;             // ID numerik proses: 1, 2, ..., N
        std::string name;   // Label proses: "P1", "P2", ...
        int at;             // Arrival Time (urutan digit NPM dari depan ke belakang)
        int bt;             // Burst Time (urutan digit NPM dari belakang ke depan, 0 -> 10)
        int raw_bt;         // Nilai digit asli sebelum penyesuaian jika 0

        // Atribut hasil evaluasi penjadwalan
        int st;             // Start Time (Waktu mulai eksekusi di CPU)
        int ct;             // Completion Time (Waktu selesai eksekusi)
        int tat;            // Turnaround Time = CT - AT
        int wt;             // Waiting Time = TAT - BT
    };

    struct GanttSegment {
        std::string name;   // Label proses (misal "P1") atau "IDLE"
        int start_time;     // Waktu mulai segmen
        int end_time;       // Waktu akhir segmen
    };

    struct CandidateRR {
        int idx;            // Indeks pada daftar proses
        std::string name;   // Label proses (contoh "P1")
        int at;             // Arrival Time
        int bt;             // Burst Time
        int wt;             // Waktu tunggu saat pemilihan (current_time - AT)
        double rr;          // Response Ratio = (WT + BT) / BT
    };

    struct AlgorithmResult {
        std::string algo_name;              // Nama algoritma penjadwalan
        std::vector<Process> processes;     // Data proses akhir (terurut P1..PN)
        std::vector<GanttSegment> gantt_chart; // Segmen visual Gantt Chart
        std::vector<std::string> execution_order; // Urutan eksekusi proses
        double avg_wt;                      // Rata-rata Waiting Time (AWT)
        double avg_tat;                     // Rata-rata Turnaround Time (ATAT)
    };

} // namespace Models

#endif // PROCESS_H
