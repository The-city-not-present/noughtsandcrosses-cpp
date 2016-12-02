#include "ai/ai_position.h"
#include <algorithm>



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
    XY point;
    char me = estimates_field.moves_count&1;
    double e_max = 0;
    for( point.y = estimates_field.ctx.y_min; point.y<estimates_field.ctx.y_max+1; ++point.y )
        for( point.x = estimates_field.ctx.x_min; point.x<estimates_field.ctx.x_max+1; ++point.x )
            if( !(bool)estimates_field[point] ) {
                if( estimates_field[point][1-me]>e_max ) e_max = estimates_field[point][1-me];
                moves.push_back( unique_ptr<AI_move>(new AI_move{
                    this,
                    point,
                    estimates_field[point]
                }) );
            };
    for( auto& i : moves )
        i->position->estimate[1-me] = e_max - i->position->estimate[1-me];
    sort(
        moves.begin(),
        moves.end(),
        [&me] ( const unique_ptr<AI_move>& a, const unique_ptr<AI_move>& b ) -> bool {
            return ((a->get_estimate()[me]-a->get_estimate()[1-me]) - (b->get_estimate()[me]-b->get_estimate()[1-me])>0);
        }
    );
};

void AI_position_recursive::calculate_estimate() {
};

// == 3. AI_position_static ==

AI_position_static::AI_position_static( int count, est_pair e ) {
    this->moves_count  = count;
    this->estimate = e;
};



