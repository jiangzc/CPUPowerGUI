#ifndef CPUPOLICY_H
#define CPUPOLICY_H
#include <QString>

#define KNOWN_POLICY(policy)  extern const char * const policy ;

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

class CPUPolicy
{
public:
    QString name;
    QString value;
    bool isReadable = false;
    bool isWriteable = false;
    bool isEmpty();
};
#endif // CPUPOLICY_H
