#ifndef AI_POSITION_DIRECTORY_H
#define AI_POSITION_DIRECTORY_H

#include <map>
#include "ai/ai_position.h"

class AI_position_prototype;

class AI_position_directory
{
    public:
        AI_position_directory();
        shared_ptr<AI_position_prototype> search( Field<Field_cell_type>& );
        shared_ptr<AI_position_prototype> search( Field<Estimate_field_cell_type>& );
        shared_ptr<AI_position_prototype> search( Field<Field_cell_type>&, XY );
        shared_ptr<AI_position_prototype> search( Field<Estimate_field_cell_type>&, XY );
        AI_position_directory& operator << ( shared_ptr<AI_position_prototype> );
        void purge( Field<Field_cell_type>* );
        void purge( Field<Estimate_field_cell_type>* );

        map<int,vector<shared_ptr<AI_position_prototype>>> data;
};

#endif // AI_POSITION_DIRECTORY_H
