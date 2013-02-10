#ifndef REFERENCE_HPP
#define REFERENCE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <list>

    // webdar headers

class reference
{
public:
    reference() { rd = peers.begin(); };
    virtual ~reference() { clear_all_peers(); };

protected:

	/// method used to create a relation between two objects
	///
	/// \note the relation is symetrical (a.peer_with(&b) is the same
	/// as b.peer_with(&a). The relation ends when one or the other object
	/// is destroyed. The other object is then informed of this event

    void peer_with(reference *obj);

	/// break the peering with the object given as argument
    void break_peer_from(reference *obj);

	/// to be informed when a peer has broke an established peering
    virtual void broken_peering_from(reference *obj) {};

	/// whether the current object has peering
    bool is_empty() const { return peers.empty(); };

	/// the number of peers
    unsigned int size() const { return peers.size(); };

	/// reset the peers reading
    void reset_read_peers() { rd = peers.begin(); };

	/// provide the next peer
	///
	/// \param[in] the address of the next peer upon successful call
	/// \return true if a next peer exists, false else in that case
	/// the argument is undefined
    bool read_next_peer(reference * & peer);

	/// break all peering from all peers
    void clear_all_peers();

private:
    std::list<reference *> peers;
    std::list<reference *>::iterator rd;
};

#endif
