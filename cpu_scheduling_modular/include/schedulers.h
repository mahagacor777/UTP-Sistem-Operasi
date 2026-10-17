#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include <vector>
#include <string>
#include "process.h"

namespace Schedulers {

    Models::AlgorithmResult runFCFS(std::vector<Models::Process> proc_list);
    Models::AlgorithmResult runSJF(std::vector<Models::Process> proc_list);
    Models::AlgorithmResult runHRRN(std::vector<Models::Process> proc_list);

} // namespace Schedulers

#endif // SCHEDULERS_H
