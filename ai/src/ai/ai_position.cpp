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
    long double sum_p       = 0;
    long double sum_e_notme = 0;
    long double sum_e_me    = 0;
    long double e_max = 0;
    for( point.y = estimates_field.ctx.y_min; point.y<estimates_field.ctx.y_max+1; ++point.y )
        for( point.x = estimates_field.ctx.x_min; point.x<estimates_field.ctx.x_max+1; ++point.x )
            if( !(bool)estimates_field[point] ) {
                if( estimates_field[point][1-me]>e_max )
                    e_max = estimates_field[point][1-me];
                long double p = 1.0 - (1.0-estimates_field[point][0])*(1.0-estimates_field[point][1]);//((estimates_field[point][0]+estimates_field[point][1])*0.5);
                if( !(p>0) )
                    continue;
                long double r = 1.0 - (estimates_field[point][0]*estimates_field[point][0]+estimates_field[point][1]*estimates_field[point][1])*0.5;
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
    for( auto& i : moves ) {
        if( i.probability<0 )
            throw runtime_error("probability < 0");
        if( sum_p<0 )
            throw runtime_error("probability < 0");
        const long double k = i.probability / sum_p;
        const long double p_me    = pow( k, 0.3 );
        const long double p_notme    = pow( k, 0.19 );
        //const long double p_me    = sum_p+i.probability)*(25/(25+(sum_p-1)))+(sum_p+ 2*i.probability)*(1-25/(25+(sum_p-1));
        //const long double p_notme = sum_p-i.probability)*(25/(25+(sum_p-1)))+(sum_p-12*i.probability)*(1-25/(25+(sum_p-1));
        if( (p_me<0)||(sum_p-p_me<0) )
            throw runtime_error("p[me] < 0");
        if( (p_notme<0)||(sum_p-p_notme<0) )
            throw runtime_error("p[notme] < 0");
        i.position->estimate[me] =
            0.15 * i.position->estimate[me] +
            0.85 * ( sum_e_me + p_me * i.get_estimate()[me] ) / ( sum_p + p_me);
        i.position->estimate[1-me] =
            0.35 * (e_max - i.get_estimate()[1-me] ) +
            0.65 * ( sum_e_notme - p_notme * i.get_estimate()[1-me] ) / ( sum_p - p_notme);
        if( (i.get_estimate()[0]<0)||(i.get_estimate()[1]<0))
            throw runtime_error("e < 0");
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
        i.probability = 1.0/(1.0+std::exp((1.0/(1.0-b) - 1.0/(1.0-a))/0.7213475204444817));
    };
   sort(
        moves.begin(),
        moves.end(),
        [&me] ( AI_move& a, AI_move& b ) -> bool {
            return /*(
                a.probability!=b.probability ?
                */(( a.probability - b.probability ) >= 0 )/* :
                (( a.est_base[me] - b.est_base[me] ) > 0 )
            )*/;
        }
    );
    {
        long double p = moves[0].probability*0.97;
        long double po = 1/((1-p)*(1-p)*(1-p));
        po *= 3.2;
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



