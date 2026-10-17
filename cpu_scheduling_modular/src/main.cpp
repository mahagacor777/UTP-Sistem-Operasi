#include <iostream>
#include <vector>
#include "../include/process.h"
#include "../include/npm_parser.h"
#include "../include/display.h"
#include "../include/schedulers.h"

using namespace std;

int main() {
    // 1. Tampilkan Banner Judul
    cout << "========================================================================================\n";
    cout << "    SIMULASI ALGORITMA PENJADWALAN PROSES CPU BERDASARKAN NPM MAHASISWA                \n";
    cout << "    Mata Kuliah: Sistem Operasi (Operating System) - Computer Science                  \n";
    cout << "========================================================================================\n\n";

    // 2. Input NPM dan Parsing Data Proses (Modul NPMParser)
    string npm = NPMParser::promptNPM();
    vector<Models::Process> processes = NPMParser::generateProcesses(npm);

    // 3. Tampilkan Data Inisialisasi Proses (Modul Display)
    Display::printProcessInitTable(npm, processes);

    // 4. Simulasi dan Tampilkan FCFS (Modul Schedulers & Display)
    Models::AlgorithmResult fcfs_res = Schedulers::runFCFS(processes);
    Display::printAlgorithmResult(fcfs_res);

    // 5. Simulasi dan Tampilkan SJF Non-Preemptive
    Models::AlgorithmResult sjf_res = Schedulers::runSJF(processes);
    Display::printAlgorithmResult(sjf_res);

    // 6. Simulasi dan Tampilkan HRRN Non-Preemptive
    Models::AlgorithmResult hrrn_res = Schedulers::runHRRN(processes);
    Display::printAlgorithmResult(hrrn_res);

    // 7. Tampilkan Tabel Perbandingan Seluruh Algoritma (Modul Display)
    Display::printComparisonTable(fcfs_res, sjf_res, hrrn_res);

    cout << "\nProgram selesai dieksekusi. Tekan Enter untuk keluar...\n";
    cin.ignore();
    cin.get();

    return 0;
}
