#include "ai/ai_position.h"
#include <algorithm>
#include <cmath>



// == 1. AI_position_prototype ==

// == 2. AI_position_recursive ==

int AI_position_recursive::position_index = 0;

AI_position_recursive::AI_position_recursive( Field<Field_cell_type> *o ) : estimates_field(o) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count;
    collect_moves_and_calculate_estimates();
    position_directory << this;
}

AI_position_recursive::AI_position_recursive( Field<Field_cell_type> *o, XY p ) : estimates_field(o,p) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count+1;
    collect_moves_and_calculate_estimates();
    position_directory << this;
}

AI_position_recursive::AI_position_recursive( Field<Estimate_field_cell_type> *o ) : estimates_field(o) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count;
    collect_moves_and_calculate_estimates();
    position_directory << this;
}

AI_position_recursive::AI_position_recursive( Field<Estimate_field_cell_type> *o, XY p ) : estimates_field(o,p) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count+1;
    collect_moves_and_calculate_estimates();
    position_directory << this;
}

void AI_position_recursive::collect_moves_and_calculate_estimates() {
    XY point;
    char me = estimates_field.moves_count&1;
    double e_max  { 0 };
    double e_max2 { 0 };
    for( point.y = estimates_field.ctx.y_min; point.y<estimates_field.ctx.y_max+1; ++point.y )
        for( point.x = estimates_field.ctx.x_min; point.x<estimates_field.ctx.x_max+1; ++point.x )
            if( !(bool)estimates_field[point] ) {
                if( estimates_field[point][1-me]>=e_max ) {
                    e_max2 = e_max;
                    e_max = estimates_field[point][1-me];
                } else
                    if( estimates_field[point][1-me]>e_max2 )
                        e_max2 = estimates_field[point][1-me];
                moves.push_back( AI_move{
                    this,
                    point,
                    estimates_field[point]
                } );
            };
    for( auto& i : moves )
        i.position->estimate[1-me] = 0.8 * ( e_max-i.position->estimate[1-me]>e_max2 ? e_max-i.position->estimate[1-me] : e_max2 ) + 0.2 * (e_max-i.position->estimate[1-me]);
    recalculate_estimates();
};

void AI_position_recursive::update_probability_global( double val ) {
    probability_global = 1-(1-probability_global)*(1-val);
    for( auto& i : moves )
        i.position->update_probability_global( val * i.probability );
};

void AI_position_recursive::recalculate_estimates() {
    char me = moves_count&1;
    vector<AI_move*> win_moves;
    for( auto& i : moves )
        if( i.get_estimate()[me]>=0.99999999 )
            win_moves.push_back( &i );
    if( win_moves.size()>0 ) {
        for( auto &i : moves )
            i.probability = 0;
        for( auto &i : win_moves )
            i->probability = 1.0 / win_moves.size();
        estimate[1-me] = 0;
        estimate[me]   = 1;
        sort(
            moves.begin(),
            moves.end(),
            [&me] ( const AI_move& a, const AI_move& b ) -> bool {
                return (( a.probability - b.probability ) > 0 );
            }
        );
        return;
    };
    double p_sum = 0.0;
    double_pair e_sum;
    if( moves.size()==0 )
        throw runtime_error("no moves");
    for( auto &i : moves ) {
        if( i.get_estimate()[1-me]>=0.99999999 ) {
            i.probability = 0;
            continue;
        };
        double &a = i.get_estimate()[me];
        double &b = i.get_estimate()[1-me];
        i.probability = 1.0/(1.0+std::exp((1.0/(1.0-b) - 1.0/(1.0-a))/0.7213475204444817));
    };
   sort(
        moves.begin(),
        moves.end(),
        [&me] ( AI_move& a, AI_move& b ) -> bool {
            return (
                a.probability!=b.probability ?
                (( a.probability - b.probability ) >= 0 ) :
                (( a.est_base[me] - b.est_base[me] ) > 0 )
            );
        }
    );
    {
        double p = moves[0].probability*0.97;
        double po = 1/((1-p)*(1-p)*(1-p));
        for( auto &i : moves )
            i.probability = pow( i.probability, po );
    };
    for( auto &i : moves ) {
        p_sum += i.probability;
        e_sum[0] += i.get_estimate()[0] * i.probability;
        e_sum[1] += i.get_estimate()[1] * i.probability;
    };
    if( p_sum>0 ) {
        e_sum[0] = e_sum[0]/p_sum;
        e_sum[1] = e_sum[1]/p_sum;
        estimate = e_sum;
        for( auto &i : moves )
            i.probability = i.probability / p_sum;
    } else {
        estimate[me] = 0;
        estimate[1-me]   = 1;
    };
};




// == 3. AI_position_static ==

AI_position_static::AI_position_static( int count, double_pair e ) {
    this->moves_count  = count;
    this->estimate = e;
};

void AI_position_static::update_probability_global( double val ) {
    probability_global = val;
};



