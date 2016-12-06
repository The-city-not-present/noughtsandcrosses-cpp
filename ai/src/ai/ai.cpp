#include "ai/ai.h"
#include <cmath>

XY AI::find_move() {
    if( field->moves_count==0 )
        return refmove;
    start_position = make_shared<AI_position_recursive>( &*field );
    //evaluate();
    //evaluate();
    //evaluate();
    return XY{ start_position->moves[0].move.x+refmove.x, start_position->moves[0].move.y+refmove.y };
};

void AI::evaluate() {
    flush_position_probabilities();
    for( int depth = 3; depth>=0; --depth ) { // max 7 iterations, but not guaranteed to be 7 moves deep
        auto candidates = collect_move_candidates();
        for( auto &move_to_promote : candidates ) {
            move_to_promote->position.reset();
            move_to_promote->position = make_shared<AI_position_recursive>( &move_to_promote->parent_position->estimates_field, move_to_promote->move );
        };
        AI_position_static::position_directory.delete_nulls();
    };
    start_position->recalculate_estimates();
    AI_position_recursive::position_directory.delete_nulls();
};

vector<AI_move*> AI::collect_move_candidates() {
    vector<AI_move*> whitelist;
    double limit =0.70;
    for( auto&position_abstract : AI_position_recursive::position_directory ) {
        AI_position_recursive* position = (AI_position_recursive*)position_abstract;
        for( auto& move : position->moves ) {
            double limit_virtual = 1.0 - (1.0-limit) * pow(move.probability * position->probability_global,0.1);
            if(
              ( ( move.get_estimate()[0]>limit_virtual ) ||
                ( move.get_estimate()[1]>limit_virtual ) ) &&
                ( move.get_estimate()[0]<0.9999999     ) &&
                ( move.get_estimate()[1]<0.9999999     ) &&
                move.position->is_static()
            )
                whitelist.push_back( &move );
        };
    };
    return whitelist;
};

void AI::flush_position_probabilities() {
    for( auto &i : AI_position_recursive::position_directory )
        i->probability_global = 0;
    for( auto &i : AI_position_static::position_directory )
        i->probability_global = 0;
    start_position->update_probability_global( 1 );
};

