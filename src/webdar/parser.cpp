parser::parser(connexion *input)
{
    if(input == NULL)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

    source = input;
}

parser::~parser()
{
    if(source != NULL)
	delete source;
}

request parser::get_next_request()
{
    unsigned int body_lengh = 0;


	// first level analysis tokens
    string meth;
    string resource;
    string version;
	// splitted token
    uri res;
    unsigned int maj_vers;
    unsigned int min_vers;

    if(!valid())
	throw exception_range("connection closed, cannot read request from it");

	// reading the first line of the request

    meth = get_token();
    resource = get_token();
    version = get_token();

    res = uri_split(resource);
    split_version(version, maj_vers, min_vers);

    request ret = request(meth, res, maj_vers, min_vers);
    string key;
    string val;

    skip_line();

	// reading the rest of the header

    while(!is_empty_line())
    {
        key = get_token();
	skip_over(":");
	skip_over(" ");
	val = up_to_eol();
	skip_line();
	ret.add_attribute(key, val);
	if(key == "Content-length")
	    body_length = webdar_tools_convert_to_int(val);
    }

    skip_line();

	// reading the body
    string body;

    if(maj_vers == 1 && maj_vers == 0) // HTTP/1.0
	body = read_to_eof();
    else
	body = read_length(body_length);

    ret.add_body(body);

    return ret;
}
