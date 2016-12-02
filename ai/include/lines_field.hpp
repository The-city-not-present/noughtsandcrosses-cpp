// The file should be only included once in lines.h
// It contains implementation for template class Field<typename>
// and connected classes, such as many iterator classes

#include <cstring>

// == Field class ==

// создание, конструкторы

template<typename type>
Field<type>::Field() : moves_count(0) {
    Create();
};

template<typename type>
Field<type>::Field( Field_constraints constraints ) : ctx(constraints), moves_count(0) {
    Create();
};

template<typename type>
Field<type>::Field( int a, int b, int c, int d ) : moves_count(0) {
    ctx = Field_constraints( a, b, c, d );
    Create();
};

template<typename type>
void Field<type>::Create() {
    w = ctx.x_max - ctx.x_min + 1;
    h = ctx.y_max - ctx.y_min + 1;
    data = unique_ptr<type[]>( new type[w*h] );
    std::memset( data.get(), 0, w*h*sizeof(type) );
};

template<typename type>
Field<type>::~Field() {
    data.reset(); // вроде необязательно
};

// методы и операции


template<typename type>
type& Field<type>::operator[] ( XY index ) {
    #ifndef NDEBUG
    if( (index.x<ctx.x_min) || (index.x>ctx.x_max) || (index.y<ctx.y_min) || (index.y>ctx.y_max) )
        throw std::out_of_range( "Field: out of range" );
    #endif
    return *( data.get() + w*(index.y-ctx.y_min) + index.x-ctx.x_min );
};

template<typename type>
Field_iterator<type> Field<type>::begin() {
    return Field_iterator<type>( data.get() );
}

template<typename type>
Field_iterator<type> Field<type>::end() {
    return Field_iterator<type>( data.get()+w*h );
}

// Field_iterator

template<typename type>
Field_iterator<type>::Field_iterator( type *_p ) : p(_p) {};

template<typename type>
Field_iterator<type>::Field_iterator( const Field_iterator& _t ) : p(_t.p) {};

template<typename type>
typename Field_iterator<type>::reference Field_iterator<type>::operator * () {
    return *p;
};

template<typename type>
bool Field_iterator<type>::operator == ( Field_iterator const& other ) const {
    return p == other.p;
};

template<typename type>
bool Field_iterator<type>::operator != ( Field_iterator const& other ) const {
    return p != other.p;
};

template<typename type>
Field_iterator<type>& Field_iterator<type>::operator ++ () {
    ++p; // следующий элемент в строке, иначе новая строка - все элементы выделены по порядку
    return *this;
};

template<typename type>
Field_iterator<type>& Field_iterator<type>::operator -- () {
    ++p;
    return *this;
};

// Field_iterator_dir
template<typename type>
Field_iterator_dir<type>::Field_iterator_dir( type *_p, int d ) : Field_iterator<type>(_p), shift_offset(d) {
}

template<typename type>
Field_iterator_dir<type>& Field_iterator_dir<type>::operator ++ () {
    this->p += shift_offset;
    return *this;
};

template<typename type>
Field_iterator_dir<type>& Field_iterator_dir<type>::operator -- () {
    this->p -= shift_offset;
    return *this;
};

// distance func

template<typename type>
int Field_iterator_dir_distance( Field_iterator_dir<type>& a, Field_iterator_dir<type>& b ) {
    return (int)(b.p-a.p) / a.shift_offset;
};

// directional iterators and begin() and end() methods of the Field class

template<typename type> // первая строка
Field_line_iterator_dir_rows<type> Field<type>::begin_dir_rows() {
    return Field_line_iterator_dir_rows<type>(
        make_pair( (type*)(data.get()),                   XY{ctx.x_min,ctx.y_min} ),
        make_pair( (type*)(data.get()+w),                 XY{ctx.x_max+1,ctx.y_min} ),
        make_pair( (type*)(data.get()-ctx.x_min),         XY{0,ctx.y_min} ),
        w, h, ctx
    );
}

template<typename type> // последняя строка
Field_line_iterator_dir_rows<type> Field<type>::end_dir_rows() {
    return Field_line_iterator_dir_rows<type>(
        make_pair( data.get()+w*(h-1),           XY{ctx.x_min,ctx.y_max+1} ),
        make_pair( data.get()+w*h,               XY{ctx.x_max+1,ctx.y_max+1} ),
        make_pair( data.get()+w*(h-1)-ctx.x_min, XY{0,ctx.y_max+1} ),
        w, h, ctx
    );
}

template<typename type> // первый столбец
Field_line_iterator_dir_columns<type> Field<type>::begin_dir_columns() {
    return Field_line_iterator_dir_columns<type>(
        make_pair( data.get(),                   XY{ctx.x_min,ctx.y_min} ),
        make_pair( data.get()+w*h,               XY{ctx.x_min,ctx.y_max+1} ),
        make_pair( data.get()-ctx.y_min*h,       XY{ctx.x_min,0} ),
        w, h, ctx
    );
}

template<typename type> // последний столбец
Field_line_iterator_dir_columns<type> Field<type>::end_dir_columns() {
    return Field_line_iterator_dir_columns<type>(
        make_pair( data.get()+w,                 XY{ctx.x_max+1,ctx.y_min} ),
        make_pair( data.get()+w*h,               XY{ctx.x_max+1,ctx.y_max+1} ),
        make_pair( data.get()+w*(h-1)-ctx.x_min, XY{ctx.x_max+1,0} ),
        w, h, ctx
    );
}

template<typename type> // главная диагональ в левом нижнем углу
Field_line_iterator_dir_diagonal_main<type> Field<type>::begin_dir_diagonal_main() {
    return Field_line_iterator_dir_diagonal_main<type>(
        make_pair( data.get()+w*(h-1),           XY{ctx.x_min,ctx.y_max} ),
        make_pair( data.get()+w*h+1,             XY{ctx.x_min+1,ctx.y_max+1} ),
        make_pair( nullptr,                      XY{ctx.x_min-ctx.y_max,0} ),
        w, h, ctx
    );
}

template<typename type> // главная диагональ в правом верхнем углу
Field_line_iterator_dir_diagonal_main<type> Field<type>::end_dir_diagonal_main() {
    return Field_line_iterator_dir_diagonal_main<type>(
        make_pair( data.get()+w,                 XY{ctx.x_max+1,ctx.y_min} ),
        make_pair( data.get()+w,                 XY{ctx.x_max+1,ctx.y_min} ), // попало на тот же самый элемент
        make_pair( nullptr,                      XY{ctx.x_max-ctx.y_min+1,0} ),
        w, h, ctx
    );
}

template<typename type> // побочная диагональ в левом верхнем углу
Field_line_iterator_dir_diagonal_minor<type> Field<type>::begin_dir_diagonal_minor() {
    return Field_line_iterator_dir_diagonal_minor<type>(
        make_pair( data.get(),                   XY{ctx.x_min,ctx.y_min} ),
        make_pair( data.get()+w-1,               XY{ctx.x_min-1,ctx.y_min+1} ),
        make_pair( nullptr,                      XY{ctx.x_min+ctx.y_min,0} ),
        w, h, ctx
    );
}

template<typename type> // побочная диагональ в правом нижнем углу
Field_line_iterator_dir_diagonal_minor<type> Field<type>::end_dir_diagonal_minor() {
    return Field_line_iterator_dir_diagonal_minor<type>(
        make_pair( data.get()+w*(h+1)-1,         XY{ctx.x_max,ctx.y_max+1} ),
        make_pair( data.get()+w*(h+1)-1,         XY{ctx.x_max,ctx.y_max+1} ), // попало на тот же самый элемент
        make_pair( nullptr,                      XY{ctx.x_max+ctx.y_max+1,0} ),
        w, h, ctx
    );
}

// the same, but returning pointer via unique_ptr
// and all in one function depending on parameter dir of type lines_direction
// enabling to loop over all the directions
template<typename type>
unique_ptr<Field_line_iterator<type>> Field<type>::line_iterator_begin_by_dir( lines_direction dir ) {
    switch(dir) {
    case dir_rows:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_rows<type>(
            begin_dir_rows()
        ));
    case dir_columns:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_columns<type>(
            begin_dir_columns()
        ));
    case dir_diagonal_main:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_diagonal_main<type>(
            begin_dir_diagonal_main()
        ));
    case dir_diagonal_minor:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_diagonal_minor<type>(
            begin_dir_diagonal_minor()
        ));
    default:
        throw runtime_error("Field::illegal getting pointer to iterator by direction");
    };
};

template<typename type>
unique_ptr<Field_line_iterator<type>> Field<type>::line_iterator_end_by_dir( lines_direction dir ) {
    switch(dir) {
    case dir_rows:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_rows<type>(
            end_dir_rows()
        ));
    case dir_columns:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_columns<type>(
            end_dir_columns()
        ));
    case dir_diagonal_main:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_diagonal_main<type>(
            end_dir_diagonal_main()
        ));
    case dir_diagonal_minor:
        return unique_ptr<Field_line_iterator<type>>(new Field_line_iterator_dir_diagonal_minor<type>(
            end_dir_diagonal_minor()
        ));
    default:
        throw runtime_error("Field::illegal getting pointer to iterator by direction");
    };
};





// Field_line_iterator class

template<typename type>
Field_line_iterator<type>::Field_line_iterator(
    pair< type*,XY> p_s,
    pair< type*,XY> p_e,
    pair< type*,XY> p_n,
    int _w, int _h,
    Field_constraints& _ctx
) :
    p_start(get<0>(p_s)), p_end(get<0>(p_e)), p_neutral(get<0>(p_n)),
    point_start(get<1>(p_s)), point_end(get<1>(p_e)), point_pivot(get<1>(p_n)),
    w(_w), h(_h),
    ctx(_ctx)
{};

template<typename type>
bool Field_line_iterator<type>::operator == ( Field_line_iterator<type> const& other ) const {
    return point_pivot == other.point_pivot;
}

template<typename type>
bool Field_line_iterator<type>::operator != ( Field_line_iterator<type> const& other ) const {
    return point_pivot != other.point_pivot;
}

// overloaded operator implementation

// dir_rows
template<typename type>
Field_line_iterator_dir_rows<type>& Field_line_iterator_dir_rows<type>::operator ++ () {
    this->point_start.y++;
    this->point_end.y++;
    this->point_pivot.y++;
    this->p_start+=this->w;
    this->p_end+=this->w;
    this->p_neutral+=this->w;
    return *this;
};

template<typename type>
Field_line_iterator_dir_rows<type>& Field_line_iterator_dir_rows<type>::operator -- () {
    this->point_start.y--;
    this->point_end.y--;
    this->point_pivot.y--;
    this->p_start-=this->w;
    this->p_end-=this->w;
    this->p_neutral-=this->w;
    return *this;
};

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_rows<type>::begin() {
    return Field_iterator_dir<type>( this->p_start, 1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_rows<type>::end() {
    return Field_iterator_dir<type>( this->p_end, 1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_rows<type>::neutral() {
    return Field_iterator_dir<type>( this->p_neutral, 1 );
}

// dublicates
template<typename type>
Field_line_iterator_dir_rows<type>& Field_line_iterator_dir_rows<type>::next() {
    return ++(*this);
};

template<typename type>
Field_line_iterator_dir_rows<type>& Field_line_iterator_dir_rows<type>::prev() {
    return --(*this);
};

template<typename type>
bool Field_line_iterator_dir_rows<type>::is_equal( Field_line_iterator<type> const& other ) {
    return (*this)==other;
};

template<typename type>
bool Field_line_iterator_dir_rows<type>::is_not_equal( Field_line_iterator<type> const& other ) {
    return (*this)!=other;
};

// dir_columns
template<typename type>
Field_line_iterator_dir_columns<type>& Field_line_iterator_dir_columns<type>::operator ++ () {
    this->point_start.x++;
    this->point_end.x++;
    this->point_pivot.x++;
    this->p_start++;
    this->p_end++;
    this->p_neutral++;
    return *this;
};

template<typename type>
Field_line_iterator_dir_columns<type>& Field_line_iterator_dir_columns<type>::operator -- () {
    this->point_start.x--;
    this->point_end.x--;
    this->point_pivot.x--;
    this->p_start--;
    this->p_end--;
    this->p_neutral--;
    return *this;
};

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_columns<type>::begin() {
    return Field_iterator_dir<type>( this->p_start, this->w );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_columns<type>::end() {
    return Field_iterator_dir<type>( this->p_end, this->w );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_columns<type>::neutral() {
    return Field_iterator_dir<type>( this->p_neutral, this->w );
}

// dublicates
template<typename type>
Field_line_iterator_dir_columns<type>& Field_line_iterator_dir_columns<type>::next() {
    return ++(*this);
};

template<typename type>
Field_line_iterator_dir_columns<type>& Field_line_iterator_dir_columns<type>::prev() {
    return --(*this);
};

template<typename type>
bool Field_line_iterator_dir_columns<type>::is_equal( Field_line_iterator<type> const& other ) {
    return (*this)==other;
};

template<typename type>
bool Field_line_iterator_dir_columns<type>::is_not_equal( Field_line_iterator<type> const& other ) {
    return (*this)!=other;
};

// dir_diagonal_main
template<typename type>
Field_line_iterator_dir_diagonal_main<type>& Field_line_iterator_dir_diagonal_main<type>::operator ++ () {
    if( this->point_start.y>this->ctx.y_min) {
        this->point_start.y--;
        this->p_start-=this->w;
    } else {
        this->point_start.x++;
        this->p_start++;
   };
    this->point_pivot.x++;
    if( this->point_end.x<this->ctx.x_max+1 ) {
        this->point_end.x++;
        this->p_end++;
    } else {
        this->point_end.y--;
        this->p_end-=this->w;
    };
    return *this;
};

template<typename type>
Field_line_iterator_dir_diagonal_main<type>& Field_line_iterator_dir_diagonal_main<type>::operator -- () {
    if( this->point_start.x>this->ctx.x_min) {
        this->point_start.x--;
        this->p_start--;
    } else {
        this->point_start.y++;
        this->p_start+=this->w;
   };
    this->point_pivot.x--;
    if( this->point_end.y<this->ctx.y_max+1 ) {
        this->point_end.y++;
        this->p_end+=this->w;
    } else {
        this->point_end.x--;
        this->p_end--;
    };
    return *this;
};

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_main<type>::begin() {
    return Field_iterator_dir<type>( this->p_start, this->w+1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_main<type>::end() {
    return Field_iterator_dir<type>( this->p_end, this->w+1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_main<type>::neutral() {
    return Field_iterator_dir<type>( this->p_neutral, this->w+1 );
}

// dublicates
template<typename type>
Field_line_iterator_dir_diagonal_main<type>& Field_line_iterator_dir_diagonal_main<type>::next() {
    return ++(*this);
};

template<typename type>
Field_line_iterator_dir_diagonal_main<type>& Field_line_iterator_dir_diagonal_main<type>::prev() {
    return --(*this);
};

template<typename type>
bool Field_line_iterator_dir_diagonal_main<type>::is_equal( Field_line_iterator<type> const& other ) {
    return (*this)==other;
};

template<typename type>
bool Field_line_iterator_dir_diagonal_main<type>::is_not_equal( Field_line_iterator<type> const& other ) {
    return (*this)!=other;
};

// dir_diagonal_minor
template<typename type>
Field_line_iterator_dir_diagonal_minor<type>& Field_line_iterator_dir_diagonal_minor<type>::operator ++ () {
    if( this->point_start.x<this->ctx.x_max) {
        this->point_start.x++;
        this->p_start++;
    } else {
        this->point_start.y++;
        this->p_start+=this->w;
   };
    this->point_pivot.x++;
    if( this->point_end.y<this->ctx.y_max+1 ) {
        this->point_end.y++;
        this->p_end+=this->w;
    } else {
        this->point_end.x++;
        this->p_end++;
    };
    return *this;
};

template<typename type>
Field_line_iterator_dir_diagonal_minor<type>& Field_line_iterator_dir_diagonal_minor<type>::operator -- () {
    if( this->point_start.y>this->ctx.y_min) {
        this->point_start.y--;
        this->p_start-=this->w;
    } else {
        this->point_start.x--;
        this->p_start--;
   };
    this->point_pivot.x--;
    if( this->point_end.x>this->ctx.x_min ) {
        this->point_end.x--;
        this->p_end--;
    } else {
        this->point_end.y--;
        this->p_end-=this->w;
    };
    return *this;
};

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_minor<type>::begin() {
    return Field_iterator_dir<type>( this->p_start, this->w-1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_minor<type>::end() {
    return Field_iterator_dir<type>( this->p_end, this->w-1 );
}

template<typename type>
Field_iterator_dir<type> Field_line_iterator_dir_diagonal_minor<type>::neutral() {
    return Field_iterator_dir<type>( this->p_neutral, this->w-1 );
}

// dublicates
template<typename type>
Field_line_iterator_dir_diagonal_minor<type>& Field_line_iterator_dir_diagonal_minor<type>::next() {
    return ++(*this);
};

template<typename type>
Field_line_iterator_dir_diagonal_minor<type>& Field_line_iterator_dir_diagonal_minor<type>::prev() {
    return --(*this);
};

template<typename type>
bool Field_line_iterator_dir_diagonal_minor<type>::is_equal( Field_line_iterator<type> const& other ) {
    return (*this)==other;
};

template<typename type>
bool Field_line_iterator_dir_diagonal_minor<type>::is_not_equal( Field_line_iterator<type> const& other ) {
    return (*this)!=other;
};



