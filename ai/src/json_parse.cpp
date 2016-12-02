#include "json_parse.h"
#include <regex>


JSON_parse::JSON_parse() : data({}) {
}

JSON_parse::JSON_parse( string arg ) : data({}) {
    vector<string> temp;
    try {
        regex re( "(?:\\{(.*?)\\},?)+?" );
        sregex_iterator next( arg.begin(), arg.end(), re );
        sregex_iterator end;
        while( next!=end ) {
            smatch match = *next;
            temp.push_back( match.str(1) );
            next++;
         }
    } catch( regex_error& w) {
        throw runtime_error( "invalid argument format" );
    };
    regex re( "\\s*\\\"x\\\"\\s*\\:\\s*(-?\\d+)\\s*,\\s*\\\"y\\\"\\s*\\:\\s*(-?\\d+)\\s*" );
    for( auto i=temp.begin(); i!=temp.end(); ++i ) {
        try {
            smatch match;
            XY z;
            if( !( regex_search( (*i), match, re ) && match.size() > 1 ) )
                continue;
            z.x = atoi( static_cast<string>(match.str(1)).c_str() );
            z.y = atoi( static_cast<string>(match.str(2)).c_str() );
            data.push_back(z);
        } catch ( regex_error& e ) {
            throw runtime_error( "invalid argument format" );
        };
    }
}

