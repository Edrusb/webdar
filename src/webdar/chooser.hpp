    /// class chooser propose choice to the user
    /// to bind to an existing session or create a new one
    /// each object runs in a separated thread to interact with the user.

class chooser
{
public:
    chooser();

    bool is_running() const;

	/// run a seperated thread and return quickly or throw exception
	/// \note an exception is thrown if status is running
    void go_ask(connexion *ptr, bool can_create_new);

private:
    connexion *ptr;

    running_as_thread();
};
