#include "ai/ai.h"
#include "ai_estimates_field.h"
#include <cmath>
#include <map>
#include <functional>
#include <chrono>

#include "logname.h"

using adjust_limit_fn = std::function<int (long double)>;
long double adjust_limit_val( long double limit_base, adjust_limit_fn );

XY AI::find_move() {
    options_update();
    log_harddrive << "      test: calc_est(3)[me] = " << AI_estimates_field::calc_est(3,true) << endl;
    log_harddrive << "      test: ai_evaluate_limit = " << opts["ai_evaluate_limit"] << endl;
    if( field->moves_count==0 )
        return refmove;
    start_position = unique_ptr<AI_position_recursive>( new AI_position_recursive{&*field} );
    long double limit = opts["ai_evaluate_limit"];
    auto start_time = std::chrono::steady_clock::now();
    while( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()<3500 ) {
        limit = adjust_limit_val( limit, [&](long double p)->int{int n = collect_move_candidates(p).size();if(n>600)return 1;if(n==0)return -1;return 0;} );
        evaluate( limit );
    };
    return XY{ start_position->moves[0].move.x+refmove.x, start_position->moves[0].move.y+refmove.y };
};


bool AI::evaluate( long double limit ) {
    log_harddrive << "   evaluate()" << endl;
    flush_position_probabilities();
    for( int depth = 0; depth<opts["ai_evaluate_max_iteraions_count"]; ++depth ) {
        auto candidates = collect_move_candidates( limit );
            log_harddrive << "   collected " << candidates.size() << " to evaluate" << endl;
        if( candidates.size()==0 )
            break;
        for( auto &move_to_promote : candidates ) {
            move_to_promote->position.reset();
            move_to_promote->position = unique_ptr<AI_position_recursive>( new AI_position_recursive{ &move_to_promote->parent_position->estimates_field, move_to_promote->move } );
        };
    };
    recalculate_estimates_recursive();
    return true;
};

vector<AI_move*> AI::collect_move_candidates( long double limit ) {
    vector<AI_move*> whitelist;
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &position : position_depth->second )
            for( auto& move : position->moves ) {
                const long double limit_virtual = pow( limit * (-1/(log(move.probability*position->probability_global)/5.5-1)), 0.3 );
                if(
                    move.position->is_static() &&
                    move.position->reliability<limit_virtual
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

long double adjust_limit_val( long double limit_base, adjust_limit_fn criterion ) {
    long double val = limit_base;
    int result = criterion(val);
    if( val>0.95 )
        return val;
    if( result==0 )
        return val;
    if( result>0 )
        return adjust_limit_val( val*0.9, criterion );
    if( result<0 )
        return adjust_limit_val( 1.0-(1.0-val)*0.9, criterion );
};




void AI::options_update() {
    AI_estimates_field::calc_est_data[0]  = opts.at("ai_estimates_5_me");
    AI_estimates_field::calc_est_data[1]  = opts.at("ai_estimates_5_notme");
    AI_estimates_field::calc_est_data[2]  = opts.at("ai_estimates_4_me");
    AI_estimates_field::calc_est_data[3]  = opts.at("ai_estimates_4_notme");
    AI_estimates_field::calc_est_data[4]  = opts.at("ai_estimates_3_me");
    AI_estimates_field::calc_est_data[5]  = opts.at("ai_estimates_3_notme");
    AI_estimates_field::calc_est_data[6]  = opts.at("ai_estimates_2_me");
    AI_estimates_field::calc_est_data[7]  = opts.at("ai_estimates_2_notme");
    AI_estimates_field::calc_est_data[8]  = opts.at("ai_estimates_1_me");
    AI_estimates_field::calc_est_data[9]  = opts.at("ai_estimates_1_notme");
    AI_estimates_field::calc_est_data[10] = opts.at("ai_estimates_0_me");
    AI_estimates_field::calc_est_data[11] = opts.at("ai_estimates_0_notme");
    AI_estimates_field::calc_est_data[12] = opts.at("ai_estimates_x_me");
    AI_estimates_field::calc_est_data[13] = opts.at("ai_estimates_x_notme");
};


