#ifndef EVENTS_HPP
#define EVENTS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <set>
#include <map>

    // webdar headers


class events_referred;

    /// object of class events_referrer are able to be informed
    /// when object they refer do dye, calling the breaking_link method
class events_referrer
{
public:
    virtual ~events_referrer() {};

protected:
	/// method that let the referred to inform the referrer
	/// that it shall break the link with it
    virtual void breaking_link(void *ptr) = 0;

	/// method to use on an events_referred to be notifed
	/// by a call to beaking_link when that object is about to dye
	/// and has no more to be referred
    void add_me_to_referred(events_referred *obj);
    friend class events_referred;
};


    /// class of object that are referred by others
    /// and that informe these referrers about their imminent death
    /// invoking the events_referrer::breaking_link() method with "this"
    /// as argument
class events_referred
{
public:
    virtual ~events_referred() { break_all_links(); };

private:
    std::set<events_referrer *> links;

	/// add the object given as argument to the list of
	/// referrer to inform before this object is destroyed
    void add_referrer(events_referrer *obj) { links.insert(obj); };

    void break_all_links();
    friend void events_referrer::add_me_to_referred(events_referred *obj);
};

    /// triggered by events_trigger to proceed to a given action
    /// when a specific events occurred, by use of the on_events() method
    /// an events_actor must previously be bound to an events_trigger
    /// by the events_trigger::bind() method
class events_actor : public events_referred
{
public:
    virtual void on_events(const std::string & events_name) = 0;
};


class events_trigger : public events_referrer
{
public:
    void bind(const std::string & events_name, events_actor *obj);

protected:
	// inherited from events_referrer
    void breaking_link(const void *ptr);

    void trigger(const std::string & events_name) const;

private:
    std::map<std::string, events_actor *> associated;

};



#endif
