#include "CPUPolicy.h"


#define KNOWN_POLICY(policy)   const char * const policy = #policy ;

namespace KnownCPUPolicy
{
    KNOWN_POLICY(cpuinfo_max_freq)
    KNOWN_POLICY(cpuinfo_min_freq)
    KNOWN_POLICY(scaling_available_governors)
    KNOWN_POLICY(scaling_governor)
    KNOWN_POLICY(scaling_cur_freq)
    KNOWN_POLICY(scaling_max_freq)
    KNOWN_POLICY(scaling_min_freq)
};

#undef KNOWN_POLICY

bool CPUPolicy::isEmpty()
{
    return name.isEmpty() && value.isEmpty();
}
