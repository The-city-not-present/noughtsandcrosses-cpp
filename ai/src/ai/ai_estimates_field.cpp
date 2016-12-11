#include "ai_estimates_field.h"



long double AI_estimates_field::calc_est_data[14] = { 1.0, 1.0, 1.0, 0.995, 0.88, 0.75, 0.19, 0.13, 0.09, 0.08, 0.0006, 0.0005, 0.0, 0.0 };

// табличные значения
long double AI_estimates_field::calc_est( char n, bool player_is_me ) {
    switch(n) {
    case 5:
        return ( player_is_me ?  calc_est_data[0]  :  calc_est_data[1] );
    case 4:
        return ( player_is_me ?  calc_est_data[2]  :  calc_est_data[3] );
    case 3:
        return ( player_is_me ?  calc_est_data[4]  :  calc_est_data[5] );
    case 2:
        return ( player_is_me ?  calc_est_data[6]  :  calc_est_data[7] );
    case 1:
        return ( player_is_me ?  calc_est_data[8]  :  calc_est_data[9] );
    case 0:
        return ( player_is_me ? calc_est_data[10]  : calc_est_data[11] );
    default:
        return ( player_is_me ? calc_est_data[12]  : calc_est_data[13] );
    };
};




AI_estimates_field::AI_estimates_field( Field<Field_cell_type>* p ) {
    moves_count = p->moves_count;
    // now we find the really used area, adjust constraints to it and expand them by 4 squares
    // this calculations help us keep field area wider then real field size, and avoid expansion of space

    find_min_bounds( p->ctx, p );
    expand_bounds();

    Create(); // "init" function (mem allocation), substitution of a constructor call

    XY point;
    for( point.y = p->ctx.y_min; point.y<p->ctx.y_max+1; ++point.y )
        for( point.x = p->ctx.x_min; point.x<p->ctx.x_max+1; ++point.x )
            if( (bool)(*p)[point] )
                (*this)[point].player = (*p)[point];
};

AI_estimates_field::AI_estimates_field( Field<Field_cell_type>* p, XY add ) {
    moves_count = p->moves_count+1;

    find_min_bounds( p->ctx, p );
    if( add.x<ctx.x_min )   ctx.x_min = add.x;
    if( add.x>ctx.x_max )   ctx.x_max = add.x;
    if( add.y<ctx.y_min )   ctx.y_min = add.y;
    if( add.y>ctx.y_max )   ctx.y_max = add.y;
    expand_bounds();

    Create();
    XY point;
    for( point.y = p->ctx.y_min; point.y<p->ctx.y_max+1; ++point.y )
        for( point.x = p->ctx.x_min; point.x<p->ctx.x_max+1; ++point.x )
            if( (bool)(*p)[point] )
                (*this)[point].player = (*p)[point];
    (*this)[add].player = ( (p->moves_count&1)==0 ? cross : nought );
};

AI_estimates_field::AI_estimates_field( Field<Estimate_field_cell_type>* p ) {
    moves_count = p->moves_count;
    // now we find the really used area, adjust constraints to it and expand them by 4 squares
    // this calculations help us keep field area wider then real field size, and avoid expansion of space

    find_min_bounds( p->ctx, p );
    expand_bounds();

    Create(); // "init" function (mem allocation), substitution of a constructor call

    XY point;
    for( point.y = p->ctx.y_min; point.y<p->ctx.y_max+1; ++point.y )
        for( point.x = p->ctx.x_min; point.x<p->ctx.x_max+1; ++point.x )
            if( (bool)(*p)[point] )
                (*this)[point].player = (*p)[point].player;
};

AI_estimates_field::AI_estimates_field( Field<Estimate_field_cell_type>* p, XY add ) {
    moves_count = p->moves_count+1;

    find_min_bounds( p->ctx, p );
    if( add.x<ctx.x_min )   ctx.x_min = add.x;
    if( add.x>ctx.x_max )   ctx.x_max = add.x;
    if( add.y<ctx.y_min )   ctx.y_min = add.y;
    if( add.y>ctx.y_max )   ctx.y_max = add.y;
    expand_bounds();

    Create();
    XY point;
    for( point.y = p->ctx.y_min; point.y<p->ctx.y_max+1; ++point.y )
        for( point.x = p->ctx.x_min; point.x<p->ctx.x_max+1; ++point.x )
            if( (bool)(*p)[point] )
                (*this)[point].player = (*p)[point].player;
    (*this)[add].player = ( (p->moves_count&1)==0 ? cross : nought );
};

// важнейшая функция
// заполняет поле оценками клеток
void AI_estimates_field::calculate() {
     lines_direction dirs[4] { dir_rows, dir_columns, dir_diagonal_main, dir_diagonal_minor };
     for( auto i_dir : dirs )
        for( auto ii = line_iterator_begin_by_dir(i_dir); ii->is_not_equal(*line_iterator_end_by_dir(i_dir)); ii->next() ) {
            auto j = ii->begin();
            auto to   = ii->end();
            for( int i=0; i<4; ++i ) {
                if( to==j ) break;
                --to;
            };
            for( ; j!=to; ++j ) {
                int t_0 = 0;
                int t_1 = 0;
                int i; auto jj = j;
                for( i=0; i<5; ++jj,++i ) {
                    if( (Val)(*jj).player == cross )
                        t_0++;
                    if( (Val)(*jj).player == nought )
                        t_1++;
                };
                long double e_0 = calc_est( ( t_1>0 ? -1 : t_0 ), (moves_count&1)==0 );
                long double e_1 = calc_est( ( t_0>0 ? -1 : t_1 ), (moves_count&1)==1 );
                for( jj=j,i=0; i<5; ++jj,++i ) {
                    if( (bool)(*jj) )
                        continue; // не будем ставить оценку в ту клетку, где уже есть ход
                    (*jj).s0 = 1 - (1-(*jj).s0)*(1-e_0);
                    (*jj).s1 = 1 - (1-(*jj).s1)*(1-e_1);
                };
            };
        };
};

void AI_estimates_field::expand_bounds() {
    ctx.x_min -= 4;
    ctx.x_max += 4;
    ctx.y_min -= 4;
    ctx.y_max += 4;
};

void AI_estimates_field::find_min_bounds( Field_constraints& ctx_ref, Field<Field_cell_type>* p ) {
    XY t;
    for( t.x = ctx_ref.x_min; t.x<0; t.x++ ) {
        bool found = false;
        for( t.y = ctx_ref.y_min; t.y<=ctx_ref.y_max; ++t.y )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.x_min = t.x;
            break;
        };
    };
    for( t.x = ctx_ref.x_max; t.x>=0; t.x-- ) {
        bool found = false;
        for( t.y = ctx_ref.y_min; t.y<=ctx_ref.y_max; ++t.y )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.x_max = t.x;
            break;
        };
    };
    for( t.y = ctx_ref.y_min; t.y<0; t.y++ ) {
        bool found = false;
        for( t.x = ctx_ref.x_min; t.x<=ctx_ref.x_max; ++t.x )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.y_min = t.y;
            break;
        };
    };
    for( t.y = ctx_ref.y_max; t.y>=0; t.y-- ) {
        bool found = false;
        for( t.x = ctx_ref.x_min; t.x<=ctx_ref.x_max; ++t.x )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.y_max = t.y;
            break;
        };
    };
};

void AI_estimates_field::find_min_bounds( Field_constraints& ctx_ref, Field<Estimate_field_cell_type>* p ) {
    XY t;
    for( t.x = ctx_ref.x_min; t.x<0; t.x++ ) {
        bool found = false;
        for( t.y = ctx_ref.y_min; t.y<=ctx_ref.y_max; ++t.y )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.x_min = t.x;
            break;
        };
    };
    for( t.x = ctx_ref.x_max; t.x>=0; t.x-- ) {
        bool found = false;
        for( t.y = ctx_ref.y_min; t.y<=ctx_ref.y_max; ++t.y )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.x_max = t.x;
            break;
        };
    };
    for( t.y = ctx_ref.y_min; t.y<0; t.y++ ) {
        bool found = false;
        for( t.x = ctx_ref.x_min; t.x<=ctx_ref.x_max; ++t.x )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.y_min = t.y;
            break;
        };
    };
    for( t.y = ctx_ref.y_max; t.y>=0; t.y-- ) {
        bool found = false;
        for( t.x = ctx_ref.x_min; t.x<=ctx_ref.x_max; ++t.x )
            found = found || (bool)(*p)[t];
        if( found ) {
            ctx.y_max = t.y;
            break;
        };
    };
};



