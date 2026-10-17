#include "../include/npm_parser.h"
#include <iostream>
#include <cctype>

using namespace std;

namespace NPMParser {

    bool isValidNPM(const string& npm) {
        if (npm.empty()) return false;
        for (char c : npm) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    string promptNPM() {
        string npm;
        while (true) {
            cout << "Masukkan Nomor Pokok Mahasiswa (NPM): ";
            if (!(cin >> npm)) exit(0);

            if (isValidNPM(npm)) {
                return npm;
            }
            cout << ">> Error: Input tidak valid! Masukkan hanya karakter angka (contoh: 2210511001).\n\n";
        }
    }

    vector<Models::Process> generateProcesses(const string& npm) {
        int n = npm.length();
        vector<Models::Process> processes(n);

        for (int i = 0; i < n; ++i) {
            processes[i].id = i + 1;
            processes[i].name = "P" + to_string(i + 1);
            
            // Aturan 1: Arrival Time (AT) dari digit depan ke belakang
            processes[i].at = npm[i] - '0';
            
            // Aturan 2: Burst Time (BT) dari digit belakang ke depan
            processes[i].raw_bt = npm[n - 1 - i] - '0';
            
            // Aturan 3: Jika BT == 0, ubah menjadi 10
            processes[i].bt = (processes[i].raw_bt == 0) ? 10 : processes[i].raw_bt;
            
            processes[i].st = 0;
            processes[i].ct = 0;
            processes[i].tat = 0;
            processes[i].wt = 0;
        }
        return processes;
    }

} // namespace NPMParser
