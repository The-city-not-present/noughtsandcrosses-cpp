#ifndef LINES_H
#define LINES_H

#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <memory>

using namespace std;




// == base definitions ==

class XY{
public:
    int x;
    int y;
    bool operator == ( XY other ) const { return ( (x==other.x)&&(y==other.y) ); };
    bool operator != ( XY other ) const { return ( (x!=other.x)||(y!=other.y) ); };
    bool operator >= ( XY other ) const { if(y>other.y)return true; if(y<other.y)return false; return x>=other.x; };
    bool operator > ( XY other ) const { if(y>other.y)return true; if(y<other.y)return false; return x>other.x; };
    bool operator <= ( XY other ) const { if(y>other.y)return false; if(y<other.y)return true; return x<=other.x; };
    bool operator < ( XY other ) const { if(y>other.y)return false; if(y<other.y)return true; return x<other.x; };
};
enum lines_direction {
    dir_rows,
    dir_columns,
    dir_diagonal_main,
    dir_diagonal_minor
};
typedef struct{ const lines_direction d; int u; int v; } UV;
typedef vector<XY> move_list;
enum Val : char { // младший бит - defined ли, второй бит - крестик (0) или нолик (1)
    undefined   = 0,
    cross       = 1,
    undefined2  = 2,
    nought      = 3
};

// == convert functions ==

XY lines_translate_uv_to_ij( UV );
XY lines_translate_ij_to_xy( XY, XY );
XY lines_translate_uv_to_xy( UV, XY );
UV lines_translate_ij_to_uv( lines_direction, XY );
XY lines_translate_xy_to_ij( XY, XY );
UV lines_translate_xy_to_uv( lines_direction, XY, XY );




// == Field_cell_type class ==

// TODO: компилятор где-то дает предупреждения об инлайнах; может, здесь? Лучше переписать?

// класс-тип, которого будут элементы в таблицах; позволяет читать отдельные биты

class Field_cell_type {
public:
    //uint32_t val; // чтоб 4 байта и беззнаковый
    char val; // байта хватит
    Field_cell_type();
    Field_cell_type( int );
    Field_cell_type( Val );
    //Field_cell_type( Field_cell_type& );
    operator int() const;
    operator char() const;
    operator string() const;
    operator bool() const;// то же, что is_defined()
    operator Val() const;
    bool operator == ( Field_cell_type& ) const;
    char get_bit( char i ) const;
    bool is_defined() const;
    bool is_undefined() const;
};




// Field family definitions

// TODO: define namespace field?

// constraints class

class Field_constraints {
private:
    void check() {
        x_min = ( x_min<=0  ? x_min : 0  );
        x_max = ( x_max>=-1 ? x_max : -1 );
        y_min = ( y_min<=0  ? y_min : 0  );
        y_max = ( y_max>=-1 ? y_max : -1 );
    };
public:
    int x_min; int x_max; int y_min; int y_max;
    Field_constraints() : x_min(0), x_max(-1), y_min(0), y_max(-1) {};
    Field_constraints( int _x_min, int _x_max, int _y_min, int _y_max ) : x_min(_x_min), x_max(_x_max), y_min(_y_min), y_max(_y_max) { check(); };
    bool in_range( XY r ) {
        return ( ( r.x>=x_min ) && ( r.x<=x_max ) && ( r.y>=y_min ) && (r.y<=y_max ) );
    };
};

// Field iterator class

template<typename type>
class Field_iterator : public iterator<random_access_iterator_tag,type> {
public:
    typedef type& reference;
    Field_iterator( type * );
    Field_iterator( const Field_iterator& );
    Field_iterator::reference operator * ();
    bool operator == ( Field_iterator const& ) const;
    bool operator != ( Field_iterator const& ) const;
    Field_iterator& operator ++ ();
    Field_iterator& operator -- ();
    type *p;
};



// "dir" means "by direction, in any direction"
// we should overload operators ++ and -- to make it move pointer to any direction

template<typename type>
class Field_iterator_dir : public Field_iterator<type> {
public:
    Field_iterator_dir( type *, int );
    /*explicit */Field_iterator_dir( const Field_iterator_dir& o ) : Field_iterator<type>(o), shift_offset(o.shift_offset) {};
    //Field_iterator_dir<type>& operator = (

    friend int Field_iterator_dir_distance( Field_iterator_dir<type>&, Field_iterator_dir<type>& );

    Field_iterator_dir( const Field_iterator<type>& ) = delete; // нельзя приводить начальника к ребtнку
    Field_iterator_dir& operator ++ ();
    Field_iterator_dir& operator -- ();
protected:
    int shift_offset;
};




// iterator_line

template<typename type>
class Field_line_iterator : public iterator<random_access_iterator_tag,type> {
public:
    type *p_start, *p_end, *p_neutral;
    XY point_start, point_end, point_pivot;
    int w, h;
    Field_constraints& ctx;

    Field_line_iterator( pair<type*,XY>, pair<type*,XY>, pair<type*,XY>, int, int, Field_constraints& ); // yeah, a long list((   ужос

    //const int operator * () const = delete; // не надо такого; а что мы получим разыменовыванием? Объект, которого нет; ничего не получим
    bool operator == ( Field_line_iterator const& ) const;
    bool operator != ( Field_line_iterator const& ) const;
    virtual Field_line_iterator& operator ++ () = 0; // should be overriden
    virtual Field_line_iterator& operator -- () = 0;
    virtual Field_iterator_dir<type> begin()    = 0;
    virtual Field_iterator_dir<type> end()      = 0;
    virtual Field_iterator_dir<type> neutral()  = 0;

    // some dublicates so that calls via operator -> are available
    virtual bool is_equal( Field_line_iterator const& ) = 0;
    virtual bool is_not_equal( Field_line_iterator const& ) = 0;
    virtual Field_line_iterator& next() = 0; // should be overriden
    virtual Field_line_iterator& prev() = 0;
    // да, громоздкая структура будет
};



template<typename type>
class Field_line_iterator_dir_rows : public Field_line_iterator<type> {
public:
    Field_line_iterator_dir_rows( pair<type*,XY> p1, pair<type*,XY> p2, pair<type*,XY> p3, int _w, int _h, Field_constraints& _ctx ) :
        Field_line_iterator<type>(p1,p2,p3,_w,_h,_ctx)
    {};
    Field_line_iterator_dir_rows& operator ++ ();
    Field_line_iterator_dir_rows& operator -- ();
    Field_iterator_dir<type> begin();
    Field_iterator_dir<type> end();
    Field_iterator_dir<type> neutral();
    // and dublicates
    Field_line_iterator_dir_rows& next();// = operator ++;
    Field_line_iterator_dir_rows& prev();// = operator --;
    bool is_equal( Field_line_iterator<type> const& );// = operator ==;
    bool is_not_equal( Field_line_iterator<type> const& );// = operator !=;
    // на самом деле ненужный хлам, но что-то с++ тяжело позволяет реализовать абстрактные классы без этой херни
};

template<typename type>
class Field_line_iterator_dir_columns : public Field_line_iterator<type> {
public:
    Field_line_iterator_dir_columns( pair<type*,XY> p1, pair<type*,XY> p2, pair<type*,XY> p3, int _w, int _h, Field_constraints& _ctx ) :
        Field_line_iterator<type>(p1,p2,p3,_w,_h,_ctx)
    {};
    Field_line_iterator_dir_columns& operator ++ ();
    Field_line_iterator_dir_columns& operator -- ();
    Field_iterator_dir<type> begin();
    Field_iterator_dir<type> end();
    Field_iterator_dir<type> neutral();
    // and dublicates
    Field_line_iterator_dir_columns& next();// = operator ++;
    Field_line_iterator_dir_columns& prev();// = operator --;
    bool is_equal( Field_line_iterator<type> const& );// = operator ==;
    bool is_not_equal( Field_line_iterator<type> const& );// = operator !=;
};

template<typename type>
class Field_line_iterator_dir_diagonal_main : public Field_line_iterator<type> {
public:
    Field_line_iterator_dir_diagonal_main( pair<type*,XY> p1, pair<type*,XY> p2, pair<type*,XY> p3, int _w, int _h, Field_constraints& _ctx ) :
        Field_line_iterator<type>(p1,p2,p3,_w,_h,_ctx)
    {};
    Field_line_iterator_dir_diagonal_main& operator ++ ();
    Field_line_iterator_dir_diagonal_main& operator -- ();
    Field_iterator_dir<type> begin();
    Field_iterator_dir<type> end();
    Field_iterator_dir<type> neutral();
    // and dublicates
    Field_line_iterator_dir_diagonal_main& next();// = operator ++;
    Field_line_iterator_dir_diagonal_main& prev();// = operator --;
    bool is_equal( Field_line_iterator<type> const& );// = operator ==;
    bool is_not_equal( Field_line_iterator<type> const& );// = operator !=;
};

template<typename type>
class Field_line_iterator_dir_diagonal_minor : public Field_line_iterator<type> {
public:
    Field_line_iterator_dir_diagonal_minor( pair<type*,XY> p1, pair<type*,XY> p2, pair<type*,XY> p3, int _w, int _h, Field_constraints& _ctx ) :
        Field_line_iterator<type>(p1,p2,p3,_w,_h,_ctx)
    {};
    Field_line_iterator_dir_diagonal_minor& operator ++ ();
    Field_line_iterator_dir_diagonal_minor& operator -- ();
    Field_iterator_dir<type> begin();
    Field_iterator_dir<type> end();
    Field_iterator_dir<type> neutral();
    // and dublicates
    Field_line_iterator_dir_diagonal_minor& next();// = operator ++;
    Field_line_iterator_dir_diagonal_minor& prev();// = operator --;
    bool is_equal( Field_line_iterator<type> const& );// = operator ==;
    bool is_not_equal( Field_line_iterator<type> const& );// = operator !=;
};



// now we ended up with iterators

// == Field class ==

template<typename type> class Field{
public:
    int moves_count;
    Field_constraints ctx;

    Field();
    Field( Field_constraints );
    Field( int, int, int, int );
    Field<type> operator= ( Field<type>& ) = delete;
    virtual ~Field();

    type& operator[] (XY);

    Field_iterator<type> begin();
    Field_iterator<type> end();

    // directional iterators
    Field_line_iterator_dir_rows<type> begin_dir_rows();
    Field_line_iterator_dir_rows<type> end_dir_rows();
    Field_line_iterator_dir_columns<type> begin_dir_columns();
    Field_line_iterator_dir_columns<type> end_dir_columns();
    Field_line_iterator_dir_diagonal_main<type> begin_dir_diagonal_main();
    Field_line_iterator_dir_diagonal_main<type> end_dir_diagonal_main();
    Field_line_iterator_dir_diagonal_minor<type> begin_dir_diagonal_minor();
    Field_line_iterator_dir_diagonal_minor<type> end_dir_diagonal_minor();
    // мы можем только вернуть указатель на обстраактный класс
    // поэтому займемся конструированием указателя
    // иначе вся концепция ни к чему
    //      комментарии на английском, для пафоса:
    // the same, but returning pointer via unique_ptr
    // and all in one function depending on parameter dir of type lines_direction
    // enabling to loop over all the directions
    unique_ptr<Field_line_iterator<type>> line_iterator_begin_by_dir(lines_direction);
    unique_ptr<Field_line_iterator<type>> line_iterator_end_by_dir(lines_direction);
protected:
    unsigned int w; unsigned int h;
    unique_ptr<type[]> data;
    void Create();
};



#include "lines_field.hpp"




// == Lines class ==

class Lines {
    public:
        unique_ptr<Field<Field_cell_type>> field;
        XY refmove;

        Lines( Lines& ) = delete;
        Lines( move_list& );
        Lines() = delete;
        virtual ~Lines() {};
        Lines operator = ( Lines& ) = delete;

        Field_cell_type checkwin();
};




#endif // LINES_H
