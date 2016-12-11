#ifndef AI_ESTIMATES_FIELD_H
#define AI_ESTIMATES_FIELD_H

#include "lines.h"



struct double_pair {
    double_pair() : s0(0), s1(0) {};
    double_pair( double a, double b ) : s0(a), s1(b) {};
    double s0;
    double s1;
    double& operator [] ( char index ) { return ( index==0 ? s0 : s1 ); };
};

struct Estimate_field_cell_type {
    Estimate_field_cell_type() : s0(0), s1(0) {};
    Field_cell_type player;
    double s0;
    double s1;
    operator double_pair() { return double_pair{s0,s1}; };
    double& operator [] ( char index ) { return ( index==0 ? s0 : s1 ); };
    operator bool () const { return (bool)player; };
};




template<typename type>
static Field_constraints field_constraints_expand( Field_constraints, Field<type>* pointer, bool(*func)(Field_iterator_dir<type>) );

class AI_estimates_field : public Field<Estimate_field_cell_type>
{
    public:
        AI_estimates_field() = delete; // нельзя создавать без параметров
        explicit AI_estimates_field( Field<Field_cell_type>* );
        explicit AI_estimates_field( Field<Field_cell_type>*, XY );
        explicit AI_estimates_field( Field<Estimate_field_cell_type>* );
        explicit AI_estimates_field( Field<Estimate_field_cell_type>*, XY );
        AI_estimates_field& operator = ( AI_estimates_field ) = delete;
        AI_estimates_field& operator = ( AI_estimates_field& ) = delete;
        void calculate();

        static double calc_est_data[14];
        static double calc_est( char, bool );

    protected:

    private:
        void find_min_bounds( Field_constraints&, Field<Field_cell_type>* );
        void find_min_bounds( Field_constraints&, Field<Estimate_field_cell_type>* );
        void expand_bounds();
};

template<typename type>
Field_constraints field_constraints_expand( Field_constraints ctx, Field<type>* pointer, bool(*func)(Field_iterator_dir<type>) ) {
    Field_constraints ctx_factual;
};

#endif // AI_ESTIMATES_FIELD_H
