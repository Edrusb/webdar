
    /// class req_ans is a pure virtual class that defines interface for requests and answers


typedef vector<std::string> request;
typedef vector<std::string> answer;

class req_ans
{
public:
    virtual answer get_answer(const request & req) = 0;
};
