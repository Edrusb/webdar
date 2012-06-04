


class thread
{
public:
    thread() {};
    virtual ~thread();

    void run();
    bool is_running(pthread_t & id);

protected:
    virtual void inherited_run() = 0;

private:
    pthread_mutex_t field_control;
    bool running;
    pthread_t tid;

    static void *run_obj(void *);
};
