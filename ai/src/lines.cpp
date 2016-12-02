#include "lines.h"
#include <vector>

using namespace std;




// == convert functions ==
// преобразует координаты для разных систем координат
// ij - декартовы координаты с нулем в том месте, где первый ход
// xy - декартовы координаты с нулем в левом верхнем углу доски, то есть координаты, которые выдал интерфейс пользователя
XY lines_translate_uv_to_ij( UV r ) {
    lines_direction d = r.d;
    int& u = r.u;
    int& v = r.v;
    if( d==dir_rows ) return { v, u };// горизонталь
    if( d==dir_columns ) return { u, v };// вертикаль
    // 2, 4 - по чёрным, 3, 5 - по белым
    XY pivot = { 0, 0 };
    int dx = 1;
    int dy = ( (d==dir_diagonal_main ) ? 1 : -1 );
    pivot.x += u;
    return XY{ pivot.x + dx*v, pivot.y+dy*v };
};


XY lines_translate_ij_to_xy( XY ij, XY refmove ) {
    return { ij.x+refmove.x, ij.y+refmove.y };
};


XY lines_translate_uv_to_xy( UV r, XY refmove ) {
    XY t = lines_translate_uv_to_ij( r );
    return { t.x+refmove.x, t.y+refmove.y };
};


UV lines_translate_ij_to_uv( lines_direction d, XY ij ) {
    int& i = ij.x;
    int& j = ij.y;
    int u, v;
    if( d==dir_rows ) return { d, j, i };
    if( d==dir_columns ) return { d, i, j };
    // now d>=2
    int dy = ( (d==dir_diagonal_main) ? 1 : -1 );
    XY pivot = { 0, 0 };
    u = i - j*dy - pivot.x;
    v = j*dy; // на самом деле можно и умножить, ибо деление на (-1) есть суть умножение
    return { d, u, v };
};


XY lines_translate_xy_to_ij( XY r, XY refmove ) {
    return { r.x-refmove.x, r.y-refmove.y };
};


UV lines_translate_xy_to_uv( lines_direction d, XY xy, XY refmove ) {
    return lines_translate_ij_to_uv( d, { xy.x-refmove.x, xy.y-refmove.y } );
};




// ==Lines_val class

Lines_val::Lines_val() : val(0) {};
Lines_val::Lines_val( int _val ) : val(_val) {};
Lines_val::Lines_val( Val _val ) : val(_val) {};
//Lines_val::Lines_val( Lines_val& _val ) : val(_val.val) {};
Lines_val::operator int() const { return val; };
Lines_val::operator char () const { if((val&1)==0)return '_';return ((val&2)==0?'x':'o'); };
Lines_val::operator string() const { if((val&1)==0)return "undefined";return ((val&2)==0?"cross":"nought"); };
Lines_val::operator Val() const { return static_cast<Val>(val&3);/* младшие 2 бита*/ };
Lines_val::operator bool() const{ return (val&1)==1; };
bool Lines_val::operator == (Lines_val& v) const { return v.val==val; };
char Lines_val::get_bit( char i )const{return val & (1<<i); };
bool Lines_val::is_defined()const {    return (val&1)==1; }
bool Lines_val::is_undefined()const {  return (val&1)==0; }




// == Lines class ==

// === конструкторы ===

Lines::Lines( move_list& arg ) : refmove({20,15})
{
    Field_constraints ctx;
    if( arg.size()>0 ) {
        refmove = arg[0];
        for( const auto& i : arg ) {
            if( i.x-refmove.x<ctx.x_min ) ctx.x_min = i.x-refmove.x;
            if( i.x-refmove.x>ctx.x_max ) ctx.x_max = i.x-refmove.x;
            if( i.y-refmove.y<ctx.y_min ) ctx.y_min = i.y-refmove.y;
            if( i.y-refmove.y>ctx.y_max ) ctx.y_max = i.y-refmove.y;
        };
        /*ctx.x_min -= 4;
        ctx.x_max += 4;
        ctx.y_min -= 4;
        ctx.y_max += 4;*/
    };
    field = unique_ptr<Field<Lines_val>>( new Field<Lines_val>(ctx) );
    int t = 0;
    for( const auto& i : arg )
        (*field)[XY({i.x-refmove.x,i.y-refmove.y})] = ((t++)&1) * 2 + 1;
    field->moves_count = arg.size();
}

/*Lines::Lines()
{
}

Lines::Lines( Lines& arg ) {
};*/

Lines_val Lines::checkwin() {
     lines_direction dirs[4] { dir_rows, dir_columns, dir_diagonal_main, dir_diagonal_minor };
     for( auto i_dir : dirs )
        for( auto ii = field->line_iterator_begin_by_dir(i_dir); ii->is_not_equal(*field->line_iterator_end_by_dir(i_dir)); ii->next() ) {
            int t_0, t_1;
            for( auto j=ii->begin(); j!=ii->end(); ++j ) {
                if( (*j).is_undefined() ) { t_0 = t_1 = 0; continue; };
                if( (Val)(*j)==cross ) { t_0++; t_1=0; };
                if( (Val)(*j)==nought ) { t_1++; t_0=0; };
                if( t_0>=5 ) return Lines_val{cross};
                if( t_1>=5 ) return Lines_val{nought};
            };
        };
    return Lines_val{}; // equal to false - all bits are 0 and we treat this as undefined aka false; Thehe is a cast operator
};

