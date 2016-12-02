#include <iostream>
#include <vector>
#include <cstdlib>

#include "lines.h"
#include "json_parse.h"
#include "ai/ai.h"
#include "read_post.h"

#include <sstream> // где-то для конверсии чисел в строки через stringstream

using namespace std;




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
            #endif
            JSON_parse data(movelist_str);
            move_list& moves = data.data;
            // ветка А
            AI ai(moves);
            // debug
            #ifdef DEBUG
            #include "debug_code.hpp"
            #endif
            XY xy = ai.find_move();
            stringstream s;
            s << "{\"x\":" << xy.x << ",\"y\":" << xy.y << "}";
            output << s.str();

        } catch ( exception &e ) {
            output << "{\"error\":\""+cgi_processing::url_encode(e.what())+"\"}";
            cerr << "exception:  " << e.what() << endl;
            return 0xff;
        };
    };
    return 0;
}

void test_field() {
    const int bounds_limit = 4;
    Field<int> f{ -bounds_limit, bounds_limit, -bounds_limit, bounds_limit };
    f[{0,0}] = 10;
    XY i{1,1};
    int val = 10;
    for( int bounds=1; bounds<=bounds_limit; ++bounds ) {
        while( i.x<bounds ) {
            f[i] = val;
            ++i.x; ++val;
        };
        while( i.y>-bounds ){
            f[i] = val;
            --i.y; ++val;
        };
        while( i.x>-bounds ){
            f[i] = val;
            --i.x; ++val;
        };
        while( i.y<bounds ){
            f[i] = val;
            ++i.y; ++val;
        };
        while( i.x<bounds ){
            f[i] = val;
            ++i.x; ++val;
        };
    };
    f[i] = val;


    // test direction conversions
    const lines_direction dir = dir_diagonal_main;
    vector<string> msgs;
    {
        UV c { dir, 0, 0 };
        while( f.ctx.in_range(lines_translate_uv_to_ij(c)) )
            --c.v;
        for( ++c.v; f.ctx.in_range(lines_translate_uv_to_ij(c)); ++c.v ) {
            XY point = lines_translate_uv_to_ij(c);
            f[point] = 90;
            UV temp = lines_translate_ij_to_uv(dir, point);
            if( (temp.u!=c.u) || (temp.v!=c.v ) ) {
                std::stringstream s;
                s<<"failed: ( "<<(int)dir<<", "<<c.u<<", "<<c.v<<" ) -> ( "<<point.x<<", "<<point.y<<" ) -> ( "<<(int)dir<<", "<<temp.u<<", "<<temp.v<<" )";
                msgs.push_back(s.str());
            };
        };
    };
    {
        UV c = { dir, 1, 0 };
        while( f.ctx.in_range(lines_translate_uv_to_ij(c)) )
            --c.v;
        for( ++c.v; f.ctx.in_range(lines_translate_uv_to_ij(c)); ++c.v ){
            XY point = lines_translate_uv_to_ij(c);
            f[point] = 91;
            UV temp = lines_translate_ij_to_uv(dir, point);
            if( (temp.u!=c.u) || (temp.v!=c.v ) ) {
                std::stringstream s;
                s<<"failed: ( "<<(int)dir<<", "<<c.u<<", "<<c.v<<" ) -> ( "<<point.x<<", "<<point.y<<" ) -> ( "<<(int)dir<<", "<<temp.u<<", "<<temp.v<<" )";
                msgs.push_back(s.str());
            };
        };
    };
    {
        UV c { dir, 2, 0 };
        while( f.ctx.in_range(lines_translate_uv_to_ij(c)) )
            --c.v;
        for( ++c.v; f.ctx.in_range(lines_translate_uv_to_ij(c)); ++c.v ) {
            XY point = lines_translate_uv_to_ij(c);
            f[point] = 92;
            UV temp = lines_translate_ij_to_uv(dir, point);
            if( (temp.u!=c.u) || (temp.v!=c.v ) ) {
                std::stringstream s;
                s<<"failed: ( "<<(int)dir<<", "<<c.u<<", "<<c.v<<" ) -> ( "<<point.x<<", "<<point.y<<" ) -> ( "<<(int)dir<<", "<<temp.u<<", "<<temp.v<<" )";
                msgs.push_back(s.str());
            };
        };
    };
    cout << ((msgs.size()==0)?"everything is ok":"messages are:") << endl;
    for( auto i : msgs )
        cout << i << endl;
    cout << endl << endl << endl;
    for( auto ii = f.begin_dir_rows(); ii!=f.end_dir_rows(); ++ii ) {
        for( auto j=ii.begin(); j!=ii.end(); ++j )
            cout << *j << " ";
        cout << endl << endl;
    };

     cout << endl << endl << "now we iterate over dir_diagonal_minor:" << endl;
    for( auto ii = f.begin_dir_diagonal_minor(); ii!=f.end_dir_diagonal_minor(); ++ii ) {
        cout << "pivot [ "<<ii.point_pivot.x<<", "<<ii.point_pivot.y<<" ] :    ";
        for( auto j=ii.begin(); j!=ii.end(); ++j )
            cout << *j << " ";
        cout << endl << endl;
    };
     cout << endl << endl << "test loop over all the directions:" << endl;
     lines_direction dirs[4] { dir_rows, dir_columns, dir_diagonal_main, dir_diagonal_minor };
     for( auto i_dir : dirs )
        for( auto ii = f.line_iterator_begin_by_dir(i_dir); ii->is_not_equal(*f.line_iterator_end_by_dir(i_dir)); ii->next() ) {
            cout << "pivot [ "<<ii->point_pivot.x<<", "<<ii->point_pivot.y<<" ] :    ";
            for( auto j=ii->begin(); j!=ii->end(); ++j )
                cout << *j << " ";
            cout << endl << endl;
        };
};

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

