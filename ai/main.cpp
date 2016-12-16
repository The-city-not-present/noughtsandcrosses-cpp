#include <iostream>
#include <vector>
#include <cstdlib>

#include "lines.h"
#include "json_parse.h"
#include "json_nlohmann/json.hpp"
#include "ai/ai.h"
#include "read_post.h"
#include "logname.h"
#include <chrono>
#include <ctime>

#include <sstream> // где-то для конверсии чисел в строки через stringstream
#include <string>

using namespace std;
using json = nlohmann::json;

#include "comment.hpp"
#include <cstdio>



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
        string s = "Content-type: application/json\n\n";
        for( auto &i : messages )
            s += i;
        log_harddrive << "timestamp: "<<std::time(nullptr)<<endl<<"program reached the end" << endl;
        cout.sync_with_stdio(true);
        cout << s;
    };
};

int main( int argc, char** argv )
{
     {
       Finally_output output;
       try {
            string input( "" );
            if( argc>1 )
                input = argv[1];
            else
                input = cgi_processing::read_post();
            #ifdef DEBUG
            input = "{\"moves\":[{\"x\":20,\"y\":15},{\"x\":16,\"y\":15},{\"x\":20,\"y\":12},{\"x\":20,\"y\":13},{\"x\":19,\"y\":14},{\"x\":18,\"y\":13},{\"x\":19,\"y\":13},{\"x\":19,\"y\":12},{\"x\":17,\"y\":14},{\"x\":18,\"y\":14},{\"x\":21,\"y\":14},{\"x\":18,\"y\":16},{\"x\":18,\"y\":17},{\"x\":19,\"y\":16},{\"x\":18,\"y\":12},{\"x\":17,\"y\":16},{\"x\":20,\"y\":16},{\"x\":17,\"y\":15},{\"x\":20,\"y\":14},{\"x\":20,\"y\":18},{\"x\":19,\"y\":17},{\"x\":23,\"y\":16},{\"x\":16,\"y\":16},{\"x\":22,\"y\":16},{\"x\":21,\"y\":15}]}";
            #endif
            log_harddrive << endl<<endl<<endl<<endl<<"timestamp: " << std::time(0) << endl << "program started with arguments:" << endl << input << endl << /*std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ) << endl << */endl;
            json j = json::parse( input );
            move_list moves;
            log_harddrive << "parsed moves array: ";
            for( json::iterator i =j["moves"].begin(); i!=j["moves"].end(); ++i ) {
                moves.push_back( XY{ (*i)["x"], (*i)["y"] } );
            };
            for( auto &i : moves )
                log_harddrive << " ( "<<i.x<<", "<<i.y<<" ), ";
            log_harddrive << endl;
            AI ai(moves);

            if( j.find("options")!=j.end() ) {
                if( j["options"].find("ai_evaluate_limit")!=j["options"].end() )
                    ai.opts["ai_evaluate_limit"] = j["options"]["ai_evaluate_limit"];
                if( j["options"].find("ai_evaluate_max_iteraions_count")!=j["options"].end() )
                    ai.opts["ai_evaluate_max_iteraions_count"] = j["options"]["ai_evaluate_max_iteraions_count"];
                if( j["options"].find("ai_estimates")!=j["options"].end() ) {
                    ai.opts["ai_estimates_5_me"]    = j["options"]["ai_estimates"]["e5_me"];
                    ai.opts["ai_estimates_5_notme"] = j["options"]["ai_estimates"]["e5_notme"];
                    ai.opts["ai_estimates_4_me"]    = j["options"]["ai_estimates"]["e4_me"];
                    ai.opts["ai_estimates_4_notme"] = j["options"]["ai_estimates"]["e4_notme"];
                    ai.opts["ai_estimates_3_me"]    = j["options"]["ai_estimates"]["e3_me"];
                    ai.opts["ai_estimates_3_notme"] = j["options"]["ai_estimates"]["e3_notme"];
                    ai.opts["ai_estimates_2_me"]    = j["options"]["ai_estimates"]["e2_me"];
                    ai.opts["ai_estimates_2_notme"] = j["options"]["ai_estimates"]["e2_notme"];
                    ai.opts["ai_estimates_1_me"]    = j["options"]["ai_estimates"]["e1_me"];
                    ai.opts["ai_estimates_1_notme"] = j["options"]["ai_estimates"]["e1_notme"];
                    ai.opts["ai_estimates_0_me"]    = j["options"]["ai_estimates"]["e0_me"];
                    ai.opts["ai_estimates_0_notme"] = j["options"]["ai_estimates"]["e0_notme"];
                    ai.opts["ai_estimates_x_me"]    = j["options"]["ai_estimates"]["ex_me"];
                    ai.opts["ai_estimates_x_notme"] = j["options"]["ai_estimates"]["ex_notme"];
               };
            };

            log_harddrive << "find_move()" << endl;
            XY xy = ai.find_move();
            log_harddrive << endl<<"found best move [ "<<xy.x<<", "<<xy.y<<" ]" << endl<<endl;
            // debug
            #ifdef DEBUG
            #include "debug_code.hpp"
            #endif
            stringstream s;
            s << "{\"x\":" << xy.x << ",\"y\":" << xy.y << ", \"comment\":\""+comment::ai_comment(ai)+"\"}";
            output << s.str();

        } catch ( exception &e ) {
            log_harddrive << "ERROR: " << e.what() << endl;
            output << "{\"error\":\""+cgi_processing::url_encode(e.what())+"\"}";
            //cout << "exception:  " << e.what() << endl;
            return 0;//0xff;
        };
    };
    return 0;
}

namespace test{
    string format( long double a ) {
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

