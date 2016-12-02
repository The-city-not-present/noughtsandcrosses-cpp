// included from main.cpp
// I am too lazy to write include guard

/*****************************************************************************
The MIT License

some code is written by Guy Rutenberg
Copyright (c) 2007

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/

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
		buffer = unique_ptr<char>(new char[content_length*sizeof(char)]);
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
