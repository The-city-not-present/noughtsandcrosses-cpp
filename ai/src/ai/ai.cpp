#include "ai/ai.h"
#include <cmath>
#include <map>

#include "logname.h"

XY AI::find_move() {
    if( field->moves_count==0 )
        return refmove;
    start_position = unique_ptr<AI_position_recursive>( new AI_position_recursive{&*field} );
    for( int count=0; count<6; count++ )
        evaluate();
    return XY{ start_position->moves[0].move.x+refmove.x, start_position->moves[0].move.y+refmove.y };
};

void AI::evaluate() {
    log_harddrive << "   evaluate()" << endl;
    flush_position_probabilities();
    for( int depth = 20; depth>=0; --depth ) { // max 7 iterations, but not guaranteed to be 7 moves deep
        auto candidates = collect_move_candidates(0.49);
            log_harddrive << "   collected " << candidates.size() << " to evaluate" << endl;
        if( candidates.size()==0 )
            break;
        for( auto &move_to_promote : candidates ) {
            move_to_promote->position.reset();
            move_to_promote->position = unique_ptr<AI_position_recursive>( new AI_position_recursive{ &move_to_promote->parent_position->estimates_field, move_to_promote->move } );
        };
    };
    recalculate_estimates_recursive();
};

vector<AI_move*> AI::collect_move_candidates( double limit ) {
    vector<AI_move*> whitelist;
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &position : position_depth->second )
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
    return whitelist;
};

void AI::flush_position_probabilities() {
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &i : position_depth->second )
            i->probability_global = 0;
    start_position->update_probability_global( 1 );
};

void AI::recalculate_estimates_recursive() {
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &i : position_depth->second )
            i->recalculate_estimates();
};

