#ifndef AI_ESTIMATES_FIELD_H
#define AI_ESTIMATES_FIELD_H

#include "lines.h"



struct est_pair {
    est_pair() : s0(0), s1(1) {};
    Lines_val player;
    double s0;
    double s1;
    double& operator [] ( char index ) { return ( index==0 ? s0 : s1 ); };
    operator bool () const { return (bool)player; };
};




template<typename type>
static Field_constraints field_constraints_expand( Field_constraints, Field<type>* pointer, bool(*func)(Field_iterator_dir<type>) );

class AI_estimates_field : public Field<est_pair>
{
    public:
        AI_estimates_field() = delete; // нельзя создавать без параметров
        explicit AI_estimates_field( Field<Lines_val>* );
        explicit AI_estimates_field( Field<Lines_val>*, XY );
        AI_estimates_field& operator = ( AI_estimates_field ) = delete;
        AI_estimates_field& operator = ( AI_estimates_field& ) = delete;
        void calculate();

    protected:

    private:
};

template<typename type>
Field_constraints field_constraints_expand( Field_constraints ctx, Field<type>* pointer, bool(*func)(Field_iterator_dir<type>) ) {
    Field_constraints ctx_factual;
};

#endif // AI_ESTIMATES_FIELD_H
