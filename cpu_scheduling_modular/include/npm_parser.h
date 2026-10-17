#ifndef NPM_PARSER_H
#define NPM_PARSER_H

#include <string>
#include <vector>
#include "process.h"

namespace NPMParser {

    bool isValidNPM(const std::string& npm);
    std::string promptNPM();
    std::vector<Models::Process> generateProcesses(const std::string& npm);

} // namespace NPMParser

#endif // NPM_PARSER_H
