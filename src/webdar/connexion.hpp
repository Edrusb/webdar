
#include <libdar/generic_file.hpp>


    /// class connexion - holds TCP sessions from clients

    /// this class provide read/write access to a TCP connexion
    /// it may support future evolution by mean of inherited class
    /// that will provide for example ssl ciphering for example
class connexion : public generic_file
{
public:
    enum status
    {
	connected,   //< both read and write are allowed
	no_connected //< session is closed both directions
    };

	/// create a new object based on a existing socket filedescriptor
    connexion(int fd);

	/// destructor is virtual to permit further evolution by mean of inherited classes
    virtual ~connexion();


    status get_status() const;


	// skip() and others should be forbidden (same as anonymous pipe)

protected:
	// to be added from inherited class


private:
    int filedesc; // socket file descriptor

};
