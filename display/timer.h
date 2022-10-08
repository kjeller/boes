#ifndef _1_S_IN_US
#define _1_S_IN_US 1000000
#endif
#ifndef _1_MS_IN_US
#define _1_MS_IN_US (_1_S_IN_US / 1000)
#endif

typedef enum
{
    Pause,
    Run,
    Reset,
    Init,
    Timeout,
} State;

class Timer {
  public:
    State state;
    unsigned long start_timestamp;
    unsigned long pause_timestamp;
    unsigned long time_diff;
    char sec_counter[4] = "000";
    char ms_counter[4] = "000";
    
    Timer();
    void start();
    void stop();
    void reset();
    void run();
};
