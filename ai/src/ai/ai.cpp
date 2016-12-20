#include "ai/ai.h"
#include "ai_estimates_field.h"
#include <cmath>
#include <map>
#include <algorithm>
#include <chrono>

#include "logname.h"

XY AI::find_move() {
    options_update();
    log_harddrive << "      test: calc_est(3)[me] = " << AI_estimates_field::calc_est(3,true) << endl;
    log_harddrive << "      test: ai_evaluate_limit = " << opts["ai_evaluate_limit"] << endl;
    if( field->moves_count==0 )
        return refmove;
    start_position = shared_ptr<AI_position_recursive>( new AI_position_recursive{&*field} );
    AI_position_recursive::position_directory << std::static_pointer_cast<AI_position_prototype>(start_position);
    long double limit = opts["ai_evaluate_limit"];
    auto start_time = std::chrono::steady_clock::now();
    const long double time_limit = opts["ai_evaluate_time"];
    while( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()<time_limit ) {
        log_harddrive << "adjusting limit..."<<std::endl;
        limit = adjust_limit_val( limit, [&](long double p)->int{int n = collect_move_candidates(p).size();if(n>120)return 1;if(n==0)return -1;return 0;}, 0 );
        log_harddrive << "limit == "<<limit<<std::endl;
        evaluate( limit, [&start_time,&time_limit]()->bool{return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()<time_limit;} );
    };
    return XY{ start_position->moves[0].move.x+refmove.x, start_position->moves[0].move.y+refmove.y };
};


bool AI::evaluate( long double limit, evaluate_cond_fn cond_fn ) {
    log_harddrive << "   evaluate()" << endl;
    flush_position_probabilities();
    do {
        auto candidates = collect_move_candidates( limit );
        log_harddrive << "   collected " << candidates.size() << " to evaluate" << endl;
        if( candidates.size()>3000 ) { // оставляем первые 3000, а то может быть много
            std::sort( candidates.begin(), candidates.end(), [](AI_move* a, AI_move* b)->bool {a->position->probability_global/a->position->reliability>b->position->probability_global/b->position->reliability;} );
            candidates.resize(3000);
            log_harddrive << "   collected number cut to " << candidates.size() << endl;
        };
        if( candidates.size()==0 )
            break;
        for( auto &move_to_promote : candidates )
            evaluate_node( *move_to_promote );
    } while( cond_fn() );
    recalculate_estimates_recursive();
    return true;
};

vector<AI_move*> AI::collect_move_candidates( long double limit ) {
    vector<AI_move*> whitelist;
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( shared_ptr<AI_position_prototype> &position : position_depth->second ) {
            int count = 0;
            for( auto& move : static_pointer_cast<AI_position_recursive>(position)->moves ) {
                if( count>10 )
                    break;
                if( !move.position->is_static() )
                    continue;
                move.position->probability_global = move.probability * position->probability_global;
                const long double limit_virtual = pow( limit * (-1/(log(move.position->probability_global)/36.5-1)), 0.3 );
                if( move.position->reliability<limit_virtual )
                    whitelist.push_back( &move );
                else
                    count++;
            };
        };
    return whitelist;
};

int AI::collect_move_candidates_fake( long double limit ) {
    int whitelist;
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ ) {
        int zn = ((int)log( position_depth->second.size() )/2.3);
        if(zn<1)    zn=1;
        int n = 0;
        for( shared_ptr<AI_position_prototype> &position : position_depth->second ) {
            int count = 0;
            int i=0;
            for( auto& move : static_pointer_cast<AI_position_recursive>(position)->moves ) {
                i++;
                if( count>6 )
                    break;
                if( i%zn!=0 )
                    continue;
                const long double limit_virtual = pow( limit * (-1/(log(move.probability*position->probability_global)/36.5-1)), 0.3 );
                if(
                    move.position->is_static() &&
                    move.position->reliability<limit_virtual
                )
                    n++;
                else
                    count++;
            };
        };
        whitelist += n * zn;
    };
    return whitelist;
};

void AI::flush_position_probabilities() {
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &position : position_depth->second )
            position->probability_global = 0;
    start_position->update_probability_global( 1 );
};

void AI::recalculate_estimates_recursive() {
    for( auto position_depth=AI_position_recursive::position_directory.data.rbegin(); position_depth!=AI_position_recursive::position_directory.data.rend(); position_depth++ )
        for( auto &position : position_depth->second )
            static_pointer_cast<AI_position_recursive>(position)->recalculate_estimates();
};

void AI::evaluate_node( AI_move &move_object ) {
    auto &position = move_object.position;
    auto temp = AI_position_recursive::position_directory.search( &move_object.parent_position->estimates_field, move_object.move );
    position.reset();
    position = (
        temp ?
        temp :
        shared_ptr<AI_position_prototype>( new AI_position_recursive{ &move_object.parent_position->estimates_field, move_object.move } )
    );
    if( !temp )
        AI_position_recursive::position_directory << position;
};




long double adjust_limit_val( long double limit_base, adjust_limit_fn criterion, int call_count=0 ) {
    long double val = limit_base;
    int result = criterion(val);
    if( call_count>10 ) {
        log_harddrive << "adjust_limit_val cut off"<<std::endl;
        return val;
    };
    if( result==0 )
        return val;
    if( result>0 )
        return adjust_limit_val( val*0.75, criterion, call_count+1 );
    if( result<0 )
        return adjust_limit_val( 1.0-(1.0-val)*0.75, criterion, call_count+1 );
};




//void AI::options_update() {
// см. ai_options.cpp


