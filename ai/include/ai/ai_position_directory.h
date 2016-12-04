#ifndef AI_POSITION_DIRECTORY_H
#define AI_POSITION_DIRECTORY_H

#include "ai/ai_position.h"

class AI_position_prototype;

class AI_position_directory
{
    public:
        AI_position_directory();
        AI_position_prototype* search( Field<Field_cell_type>* );
        AI_position_prototype* search( Field<Estimate_field_cell_type>* );
        AI_position_prototype* search( Field<Field_cell_type>*, XY );
        AI_position_prototype* search( Field<Estimate_field_cell_type>*, XY );
        AI_position_directory& operator << ( AI_position_prototype* );
        void delete_nulls();
        void purge( Field<Field_cell_type>* );
        void purge( Field<Estimate_field_cell_type>* );

        vector<AI_position_prototype*>::iterator begin() { return data.begin(); };
        vector<AI_position_prototype*>::iterator end()   { return data.end(); };

    protected:
        vector<AI_position_prototype*> data;

    private:
};

#endif // AI_POSITION_DIRECTORY_H
