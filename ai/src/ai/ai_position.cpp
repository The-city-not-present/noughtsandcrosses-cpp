#include "ai/ai_position.h"
#include <algorithm>
#include <cmath>




// == 1. AI_position_prototype ==

// == 2. AI_position_recursive ==

int AI_position_recursive::position_index = 0;
double AI_position_recursive::probabilities_pow = 3.2;

AI_position_recursive::AI_position_recursive( Field<Field_cell_type> *o ) : estimates_field(o) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count;
    collect_moves_and_calculate_estimates();
}

AI_position_recursive::AI_position_recursive( Field<Field_cell_type> *o, XY p ) : estimates_field(o,p) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count+1;
    collect_moves_and_calculate_estimates();
}

AI_position_recursive::AI_position_recursive( Field<Estimate_field_cell_type> *o ) : estimates_field(o) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count;
    collect_moves_and_calculate_estimates();
}

AI_position_recursive::AI_position_recursive( Field<Estimate_field_cell_type> *o, XY p ) : estimates_field(o,p) {
    position_id = position_index++;
    estimates_field.calculate();
    this->moves_count = o->moves_count+1;
    collect_moves_and_calculate_estimates();
}

void AI_position_recursive::collect_moves_and_calculate_estimates() {
    XY point;
    char me = estimates_field.moves_count&1;
    long double sum_p       = 0;
    long double sum_e_notme = 0;
    long double sum_e_me    = 0;
    for( point.y = estimates_field.ctx.y_min; point.y<estimates_field.ctx.y_max+1; ++point.y )
        for( point.x = estimates_field.ctx.x_min; point.x<estimates_field.ctx.x_max+1; ++point.x )
            if( !(bool)estimates_field[point] ) {
                long double p = 1.0 - (1.0-estimates_field[point][0])*(1.0-estimates_field[point][1]);//((estimates_field[point][0]+estimates_field[point][1])*0.5);
                if( !(p>0) )
                    continue;
                long double r = ( p<0.9999999 ? 1.0 - p : 1.0 );
                moves.push_back( AI_move{
                    this,
                    point,
                    estimates_field[point],
                    p,
                    r
                } );
                sum_e_me += p * estimates_field[point][me];
                sum_e_notme += p * estimates_field[point][1-me];
                sum_p += p;

            };
    AI_position_prototype* e_max_notme;
    AI_position_prototype* e_max2_notme;
    long double e_max_me = 0;
    if( moves.size()<2 )
        throw runtime_error("unpredictable moves are too few");
    e_max_notme = moves[0].position.get();
    for( auto& i : moves ) {
        if( i.position->estimate[1-me]>e_max_notme->estimate[1-me] )
            e_max_notme = i.position.get();
        if( i.position->estimate[me]>e_max_me )
            e_max_me = i.position->estimate[me];
    };
    if( moves[0].position.get()!=e_max_notme )
        e_max2_notme = moves[0].position.get();
    else
        e_max2_notme = moves[1].position.get();
    for( auto& i : moves )
        if( ( i.position.get()!=e_max_notme) && ( i.position->estimate[1-me]>e_max2_notme->estimate[1-me] ) )
            e_max2_notme = i.position.get();
    const long double e_max_notme_val = e_max_notme->estimate[1-me];
    const long double e_max2_notme_val = e_max2_notme->estimate[1-me];
    for( auto& i : moves ) {
        if( i.probability<0 )
            throw runtime_error("probability < 0");
        if( sum_p<0 )
            throw runtime_error("probability < 0");
        {
            const long double e = i.position->estimate[me];
            i.position->estimate[me] = e * e + ( 1.0 - e ) * ( 0.4*e+0.6*e_max_me );
        };
        {
            i.position->estimate[1-me] =
                /*0.95 * */( i.position.get()==e_max_notme ? e_max2_notme_val : e_max_notme_val );// +
                //0.05 * (e_max_notme_val-i.position->estimate[1-me]);
        };
        if( (i.get_estimate()[0]<0)||(i.get_estimate()[1]<0))
            throw runtime_error("e < 0");
        if( (i.get_estimate()[0]>1)||(i.get_estimate()[1]>1))
            throw runtime_error("e > 1");
    };
    recalculate_estimates();
};

void AI_position_recursive::update_probability_global( long double val ) {
    probability_global = 1-(1-probability_global)*(1-val);
    for( auto& i : moves )
        i.position->update_probability_global( val * i.probability );
};

void AI_position_recursive::recalculate_estimates() {
    char me = moves_count&1;
    vector<AI_move*> win_moves;
    for( auto& i : moves )
        if( i.position->estimate[me]>=0.99999999 )
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
    long double p_sum = 0.0;
    double_pair e_sum;
    if( moves.size()==0 )
        throw runtime_error("no moves");
    for( auto &i : moves ) {
        if( i.get_estimate()[1-me]>=0.99999999 ) {
            i.probability = 0;
            continue;
        };
        long double a = i.get_estimate()[me];
        long double b = i.get_estimate()[1-me];
        i.probability = 1.0/(1.0+std::exp((1.0/sqrt(1.0-b) - 1.0/sqrt(1.0-a))/0.7213475204444817));
        if( !( (i.probability>=0)&&(i.probability<=1) ) )
            throw runtime_error("recalculate0: probabilities are not in range < -1, 1 >   ( p == "+std::to_string(i.probability)+" )");
    };
    sort(
        moves.begin(),
        moves.end(),
        [&me] ( AI_move& a, AI_move& b ) -> bool {
            return ( a.probability - b.probability ) > 0;
        }
    );
    if( moves[0].probability<0.0000001 ) {
        estimate[me] = 0;
        estimate[1-me]   = 1;
        reliability = 1; // нет ходов
        return;
    };
    for( auto &i : moves )
        if( !( (i.probability>=0)&&(i.probability<=1) ) )
            throw runtime_error("recalculate1: probabilities are not in range < -1, 1 >   ( p == "+std::to_string(i.probability)+" )");
    if( probabilities_pow>0 )
    {
        long double p = moves[0].probability*0.97;
        long double po = 1/((1-p)*(1-p)*(1-p));
        po *= probabilities_pow;
        for( auto &i : moves )
            i.probability = pow( i.probability, po );
    };
    for( auto &i : moves )
        if( !( (i.probability>=0)&&(i.probability<=1000000) ) )
            throw runtime_error("recalculate2: probabilities are not in range < -1, 1000000 >   ( p == "+std::to_string(i.probability)+" )");
    int count = 0;
    for( auto &i : moves ) {
        count++;
        p_sum += i.probability;
        e_sum[0] += i.get_estimate()[0] * i.probability;
        e_sum[1] += i.get_estimate()[1] * i.probability;
        /*if( (probabilities_pow<=0)&&(count>24) )
            break;*/
    };
    if( p_sum>0 ) {
        e_sum[0] = e_sum[0]/p_sum;
        e_sum[1] = e_sum[1]/p_sum;
        /*if( probabilities_pow<=0 ) {
            e_sum[0] =
                moves[0].position->reliability *
                    moves[0].get_estimate()[0] +
                (1.0-moves[0].position->reliability) *
                    e_sum[0];
            e_sum[1] =
                moves[0].position->reliability *
                    moves[0].get_estimate()[1] +
                (1.0-moves[0].position->reliability) *
                    e_sum[1];
        };*/
        estimate = e_sum;
        for( auto &i : moves )
            i.probability = i.probability / p_sum;
        {
            long double temp{1};
            for( auto &i : moves )
                temp *= pow(1.0-i.position->reliability,i.probability);
            reliability = 1.0-temp*0.9;
        };
    } else {
        estimate[me] = 0;
        estimate[1-me]   = 1;
        reliability = 1; // нет ходов
    };
};




// == 3. AI_position_static ==

AI_position_static::AI_position_static( int count, double_pair e ) {
    this->moves_count  = count;
    this->estimate = e;
};

void AI_position_static::update_probability_global( long double val ) {
    probability_global = val;
};



