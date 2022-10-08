#ifndef _TIMER_G
#define _TIMER_G
#define _1_S_IN_US 1000000
#define _1_MS_IN_US (_1_S_IN_US / 1000)

typedef enum
{
    Pause,
    Run,
    Reset,
    Init,
    Timeout_1,
    Timeout_2,
} State;

class Timer {
  public:
    State state;
    unsigned long start_timestamp;
    unsigned long pause_timestamp;
    unsigned long timeout_timestamp;
    unsigned long time_diff;
    char sec_counter[4] = "000";
    char ms_counter[4] = "000";
    
    Timer();
    void start();
    void stop();
    void reset();
    void run();
};

#endif

