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




// ==Field_cell_type class

Field_cell_type::Field_cell_type() : val(0) {};
Field_cell_type::Field_cell_type( int _val ) : val(_val) {};
Field_cell_type::Field_cell_type( Val _val ) : val(_val) {};
//Field_cell_type::Field_cell_type( Field_cell_type& _val ) : val(_val.val) {};
Field_cell_type::operator int() const { return val; };
Field_cell_type::operator char () const { if((val&1)==0)return '_';return ((val&2)==0?'x':'o'); };
Field_cell_type::operator string() const { if((val&1)==0)return "undefined";return ((val&2)==0?"cross":"nought"); };
Field_cell_type::operator Val() const { return static_cast<Val>(val&3);/* младшие 2 бита*/ };
Field_cell_type::operator bool() const{ return (val&1)==1; };
bool Field_cell_type::operator == (Field_cell_type& v) const { return v.val==val; };
char Field_cell_type::get_bit( char i )const{return val & (1<<i); };
bool Field_cell_type::is_defined()const {    return (val&1)==1; }
bool Field_cell_type::is_undefined()const {  return (val&1)==0; }




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
    field = unique_ptr<Field<Field_cell_type>>( new Field<Field_cell_type>(ctx) );
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

Field_cell_type Lines::checkwin() {
     lines_direction dirs[4] { dir_rows, dir_columns, dir_diagonal_main, dir_diagonal_minor };
     for( auto i_dir : dirs )
        for( auto ii = field->line_iterator_begin_by_dir(i_dir); ii->is_not_equal(*field->line_iterator_end_by_dir(i_dir)); ii->next() ) {
            int t_0, t_1;
            for( auto j=ii->begin(); j!=ii->end(); ++j ) {
                if( (*j).is_undefined() ) { t_0 = t_1 = 0; continue; };
                if( (Val)(*j)==cross ) { t_0++; t_1=0; };
                if( (Val)(*j)==nought ) { t_1++; t_0=0; };
                if( t_0>=5 ) return Field_cell_type{cross};
                if( t_1>=5 ) return Field_cell_type{nought};
            };
        };
    return Field_cell_type{}; // equal to false - all bits are 0 and we treat this as undefined aka false; Thehe is a cast operator
};

