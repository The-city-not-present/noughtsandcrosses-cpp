#ifndef AI_POSITION_H
#define AI_POSITION_H

#include <vector>

#include "ai/ai_estimates_field.h"
#include "ai/ai_move.h"
#include "ai/ai_position_directory.h"

class AI_move;
class AI_position_directory;


// == 1. AI_position_prototype ==

class AI_position_prototype {
    public:
        virtual ~AI_position_prototype() {};

/* 1. */int moves_count;
/* 2. */double_pair estimate;

        double probability_global = {0};
        virtual void update_probability_global( double ) = 0;
        virtual const bool is_static() = 0;
        virtual const bool is_recursive() = 0;
        virtual const int id() = 0;
};




// == 2. AI_position_recursive ==

class AI_position_recursive : public AI_position_prototype
{
    public:
        AI_position_recursive() = delete;
        AI_position_recursive( Field<Field_cell_type>* );
        AI_position_recursive( Field<Field_cell_type>*, XY );
        AI_position_recursive( Field<Estimate_field_cell_type>* );
        AI_position_recursive( Field<Estimate_field_cell_type>*, XY );

        AI_estimates_field estimates_field;
        vector<AI_move> moves;

        const bool is_static() { return false; };
        const bool is_recursive() { return true; };
        const int id() { return position_id; };

        void update_probability_global( double );
        void recalculate_estimates();

        static AI_position_directory position_directory;
        static int position_index;
    protected:
        int position_id;
        void collect_moves_and_calculate_estimates(); // called once in constructor

    private:
};




// == 3. AI_position_static ==

class AI_position_static : public AI_position_prototype {
    public:
        AI_position_static() = delete;
        AI_position_static( int, double_pair );

        const bool is_static() { return true; };
        const bool is_recursive() { return false; };
        const int id() { return 0; };

        void update_probability_global( double );
};

#endif // AI_POSITION_H

