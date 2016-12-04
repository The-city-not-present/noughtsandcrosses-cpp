#include <iostream>
#include <vector>
#include <cstdlib>

#include "lines.h"
#include "json_parse.h"
#include "ai/ai.h"
#include "read_post.h"

#include <sstream> // где-то для конверсии чисел в строки через stringstream
#include <string>

using namespace std;

#include "comment.hpp"




void test_field();
void test_ai( AI& );

class Finally_output {
public:
    vector<string>& operator << ( string arg ) {
        messages.push_back(arg);
        return messages;
    };
    vector<string> messages;
    ~Finally_output() {
        cout << "Content-type: text/html\n\n";
        for( auto &i : messages )
            cout << i << endl;
    };
};

int main( int argc, char** argv )
{
     {
       Finally_output output;
       try {
            string movelist_str( "" );//[{\"x\":3,\"y\":5},{\"x\":3,\"y\":6},{\"x\":-1,\"y\":6},{\"x\":-1,\"y\":2},{\"x\":-1,\"y\":1},{\"x\":1,\"y\":7}]" );
            if( argc>1 )
                movelist_str = argv[1];
            else
                movelist_str = cgi_processing::read_post();
            #ifdef DEBUG
           if( movelist_str=="" )
                movelist_str = "[{\"x\":10, \"y\":10}]";
            movelist_str = "[{\"x\":28,\"y\":11},{\"x\":28,\"y\":10},{\"x\":29,\"y\":10},{\"x\":29,\"y\":11},{\"x\":28,\"y\":12},{\"x\":30,\"y\":12},{\"x\":31,\"y\":13},{\"x\":26,\"y\":8},{\"x\":27,\"y\":9},{\"x\":29,\"y\":13},{\"x\":31,\"y\":11},{\"x\":30,\"y\":13},{\"x\":30,\"y\":11},{\"x\":31,\"y\":12},{\"x\":30,\"y\":9},{\"x\":27,\"y\":12}]";
            #endif
            JSON_parse data(movelist_str);
            move_list& moves = data.data;
            // ветка А
            AI ai(moves);
            // debug
            #ifdef DEBUG
            //#include "debug_code.hpp"
            #endif
            XY xy = ai.find_move();
            stringstream s;
            s << "{\"x\":" << xy.x << ",\"y\":" << xy.y << ", \"comment\":\""+comment::ai_comment(ai)+"\"}";
            output << s.str();

        } catch ( exception &e ) {
            output << "{\"error\":\""+cgi_processing::url_encode(e.what())+"\"}";
            //cout << "exception:  " << e.what() << endl;
            return 0;//0xff;
        };
    };
    return 0;
}

namespace test{
    string format( double a ) {
        int aa = 10000 * a;
        stringstream sss;
        sss << aa;
        string b = sss.str();
        while( b.size() < 4 )
            b = string{"0"+b};
        return string{"0,"+b};
    };
};

void test_ai( AI &ai ) {
    auto &ar = ai.start_position->estimates_field;
    for( XY p={ar.ctx.x_min,ar.ctx.y_min}; p.y<=ar.ctx.y_max; ++p.y ) {
        cout << endl << endl;
        for( p.x=ar.ctx.x_min; p.x<=ar.ctx.x_max; ++p.x )
            cout << test::format(ar[p].s0) << "    ";
    };
};

