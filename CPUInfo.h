#ifndef CPUINFO_H
#define CPUINFO_H
#include <QObject>
#include <QString>
#include <QDir>

class CPUCore : public QObject
{
    Q_OBJECT
public:
    CPUCore(const QDir &cpuDir);
    short core_id;
    Q_PROPERTY(unsigned int cpuinfo_max_freq MEMBER m_cpuinfo_max_freq READ cpuinfo_max_freq)
    Q_PROPERTY(unsigned int cpuinfo_min_freq MEMBER m_cpuinfo_min_freq READ cpuinfo_min_freq)
    Q_PROPERTY(unsigned int scaling_max_freq MEMBER m_scaling_max_freq READ scaling_max_freq WRITE set_scaling_max_freq)
    Q_PROPERTY(unsigned int scaling_min_freq MEMBER m_scaling_min_freq READ scaling_min_freq WRITE set_scaling_min_freq)
    Q_PROPERTY(unsigned int scaling_cur_freq MEMBER m_scaling_cur_freq READ scaling_cur_freq)
    Q_PROPERTY(QString scaling_available_governors MEMBER m_scaling_available_governors READ scaling_available_governors)
    Q_PROPERTY(QString scaling_governor MEMBER m_scaling_governor READ scaling_governor WRITE set_scaling_governor)
    Q_PROPERTY(QString scaling_driver MEMBER m_scaling_driver READ scaling_driver)
    // property begin
    unsigned int cpuinfo_max_freq() const;
    unsigned int cpuinfo_min_freq() const;
    unsigned int scaling_max_freq() const;
    unsigned int scaling_min_freq() const;
    unsigned int scaling_cur_freq() const;
    QString scaling_available_governors() const;
    QString scaling_governor() const;
    QString scaling_driver() const;
    // property end
    bool update();




public slots:
    // property begin
    void set_scaling_max_freq(unsigned int scaling_max_freq);
    void set_scaling_min_freq(unsigned int scaling_min_freq);
    void set_scaling_governor(QString scaling_governor);
    // property end
  bool writePropertyToFile(QString key);
private:
    short m_core_id = 0;
    unsigned int m_cpuinfo_max_freq = 0;
    unsigned int m_cpuinfo_min_freq = 0;
    unsigned int m_scaling_max_freq = 0;
    unsigned int m_scaling_min_freq = 0;
    unsigned int m_scaling_cur_freq = 0;
    QString m_scaling_available_governors;
    QString m_scaling_governor;
    QString m_scaling_driver;
    QDir m_cpuDir;


};

class CPUInfo
{
public:
    CPUInfo();
};

#endif // CPUINFO_H
