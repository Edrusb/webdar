#ifndef REQ_ANS_HPP
#define REQ_ANS_HPP

    /// class req_ans is a pure virtual class that defines interface for requests and answers

typedef vector<std::string> request;
typedef vector<std::string> answer;

class av
{
public:
    av(const std::string & attribut);
    av(const std::string & attribut, const std::string & value);

private:
    std::string attribut;
    std::string value;
};

class req_ans
{
public:
    virtual answer get_answer(const std::string & request_prefix, const request & req, const std::vector<av> & request_attributs) = 0;
};

#endif
