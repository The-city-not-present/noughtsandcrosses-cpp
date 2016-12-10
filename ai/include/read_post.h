// included from main.cpp


namespace cgi_processing {

inline string getenv_safe(string arg) {
    const char* tmp = std::getenv(arg.c_str());
    return ( tmp==NULL ? string{} : string(tmp) );
};

std::string url_decode(std::string str)
{
	std::string temp;
	register int i;
	char tmp[5], tmpchar;
	strcpy(tmp,"0x");
	int size = str.size();
	for (i=0; i<size; i++) {
		if (str[i]=='%') {
			if (i+2<size) {
				tmp[2]=str[i+1];
				tmp[3] = str[i+2];
				tmp[4] = '\0';
				tmpchar = (char)strtol(tmp,NULL,0);
				temp+=tmpchar;
				i += 2;
				continue;
			} else {
				break;
			}
		} else if (str[i]=='+') {
			temp+=' ';
		} else {
			temp+=str[i];
		}
	}
	return temp;
}

std::string url_encode( std::string str ) {
    return str;
};

string read_post() {
    string contentlength_env = getenv_safe("CONTENT_LENGTH");
    unique_ptr<char> buffer;
    int content_length = atoi(contentlength_env.c_str());
    if( content_length==0 )
        return string{};
	try {
		buffer = unique_ptr<char>(new char[(content_length+1)*sizeof(char)]);
	} catch (std::bad_alloc& xa) {
		return string{};
	}
	if(fread(&*buffer, sizeof(char), content_length, stdin) != (unsigned int)content_length) {
		return string{};
	}
	*(&*buffer+content_length) = '\0';
    return string(url_decode(&*buffer));
};

} // end of namespace
