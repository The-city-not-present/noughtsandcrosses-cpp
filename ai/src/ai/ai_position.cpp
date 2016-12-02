#include "ai/ai_position.h"
#include <algorithm>
#include <math.h>



// == 1. AI_position_prototype ==

// == 2. AI_position_recursive ==

AI_position_recursive::AI_position_recursive( Field<Lines_val> *o ) : estimates_field(o) {
    this->moves_count = estimates_field.moves_count;
    estimates_field.calculate();
    collect_moves();
    calculate_estimate();
}

AI_position_recursive::AI_position_recursive( Field<Lines_val> *o, XY p ) : estimates_field(o,p) {
    this->moves_count = estimates_field.moves_count;
    estimates_field.calculate();
    collect_moves();
    calculate_estimate();
}

void AI_position_recursive::collect_moves() {
    vector<AI_move*> win_moves;
    XY point;
    char me = estimates_field.moves_count&1;
    double e_max = 0;
    for( point.y = estimates_field.ctx.y_min; point.y<estimates_field.ctx.y_max+1; ++point.y )
        for( point.x = estimates_field.ctx.x_min; point.x<estimates_field.ctx.x_max+1; ++point.x )
            if( !(bool)estimates_field[point] ) {
                if( estimates_field[point][1-me]>e_max )
                    e_max = estimates_field[point][1-me];
                 moves.push_back( unique_ptr<AI_move>(new AI_move{
                    this,
                    point,
                    estimates_field[point]
                }) );
                if( estimates_field[point][me]>=0.99999999 )
                    win_moves.push_back( moves.back().get() );
            };
    for( auto& i : moves )
        i->position->estimate[1-me] = e_max - i->position->estimate[1-me];
    if( win_moves.size()>0 ) {
        for( auto &i : moves )
            i->probability = 0;
        for( auto &i : win_moves )
            i->probability = 1.0 / win_moves.size();
        estimate[1-me] = 0;
        estimate[me]   = 1;
    } else {
        double p_sum = 0.0;
        double_pair e_sum;
        for( auto &i : moves ) {
            if( i->get_estimate()[1-me]>=0.99999999 ) {
                i->probability = 0;
                continue;
            };
            double &a = i->get_estimate()[me];
            double &b = i->get_estimate()[1-me];
            i->probability = 1/(1+exp((1.0/(1.0-b) - 1.0/(1.0-a))/0.7213475204444817));

            p_sum += i->probability;
            e_sum[0] += i->get_estimate()[0] * i->probability;
            e_sum[1] += i->get_estimate()[1] * i->probability;
        };
        e_sum[0] = e_sum[0]/p_sum;
        e_sum[1] = e_sum[1]/p_sum;
        estimate = e_sum;
        for( auto &i : moves )
            i->probability = i->probability / p_sum;
    };
    sort(
        moves.begin(),
        moves.end(),
        [&me] ( const unique_ptr<AI_move>& a, const unique_ptr<AI_move>& b ) -> bool {
            return (( a->probability - b->probability ) > 0 );
        }
    );
};

void AI_position_recursive::calculate_estimate() {
    // already done
};

// == 3. AI_position_static ==

AI_position_static::AI_position_static( int count, double_pair e ) {
    this->moves_count  = count;
    this->estimate = e;
};



