#include "CPUPolicy.h"

#define KNOWN_POLICY(policy)   const char * const policy = #policy ;

// https://www.kernel.org/doc/Documentation/cpu-freq/user-guide.txt
namespace KnownCPUPolicy
{
    KNOWN_POLICY(affected_cpus)
    KNOWN_POLICY(cpuinfo_cur_freq)
    KNOWN_POLICY(cpuinfo_max_freq)
    KNOWN_POLICY(cpuinfo_min_freq)
    KNOWN_POLICY(cpuinfo_transition_latency)
    KNOWN_POLICY(related_cpus)
    KNOWN_POLICY(scaling_available_frequencies)
    KNOWN_POLICY(scaling_available_governors)
    KNOWN_POLICY(scaling_governor)
    KNOWN_POLICY(scaling_cur_freq)
    KNOWN_POLICY(scaling_max_freq)
    KNOWN_POLICY(scaling_min_freq)
    KNOWN_POLICY(scaling_setspeed)
    KNOWN_POLICY(bios_limit)
};

#undef KNOWN_POLICY

bool CPUPolicy::isEmpty()
{
    return name.isEmpty() && value.isEmpty();
}
