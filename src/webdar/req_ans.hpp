#ifndef REQ_ANS_HPP
#define REQ_ANS_HPP

    /// class req_ans is a pure virtual class that defines interface for requests and answers

    /// classes that implement this interface shall provide a HTML code (part of HTML body)

typedef vector<std::string> uri;

class request
{
public:
    request(const std::string & method, const uri & url, unsigned int version_maj, unsigned int version_min);

    void add_attribute(const std::string & key, const std::string & value);
    void add_body(const std::string & key);

    const std::string & get_method() const;
    const uri & get_uri() const;
    unsigned int get_verion_maj() const;
    unsigned int get_version_min() const;
    bool find_attribute(const std::string & key, std::string & value) const;
    const std::string & get_body() const;
};


class answer
{
public:
    answer(unsigned int version_maj, unsigned int version_min, unsigned int status_code, const std::string & reason_phrase);

    void add_attribute(const std::string & key, const std::string & value);
    void add_body(const std::string & key);

    void send(connexion & out);
};

#endif
