#include "ai/ai.h"

XY AI::find_move() {
    if( field->moves_count==0 )
        return XY{20,15};
    start_position = make_shared<AI_position_recursive>( &*field );
    evaluate();
    return XY{ (*(*start_position).moves[0]).move.x+refmove.x, (*(*start_position).moves[0]).move.y+refmove.y };
};

void AI::evaluate() {
    flush_position_probabilities();
    collect_move_candidates();
};

void AI::collect_move_candidates() {
    for( auto &i : AI_position_recursive::position_directory )
        ;
};

void AI::flush_position_probabilities() {
    for( auto &i : AI_position_recursive::position_directory )
        i->probability_global = 0;
    for( auto &i : AI_position_static::position_directory )
        i->probability_global = 0;
    start_position->update_probability_global( 1 );
};

