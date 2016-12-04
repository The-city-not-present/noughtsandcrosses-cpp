#include "ai_position_directory.h"
#include <algorithm>

AI_position_directory::AI_position_directory() {}

AI_position_directory& AI_position_directory::operator << ( AI_position_prototype* item ) {
    data.push_back(item);
};

AI_position_prototype* AI_position_directory::search( Field<Estimate_field_cell_type>* field ) {
    for( auto& i : data )
        if( i->moves_count==field->moves_count ) {
            ;
        };
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Field_cell_type>* field ) {
    for( auto& i : data )
        if( i->moves_count==field->moves_count ) {
            ;
        };
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Estimate_field_cell_type>* field, XY p ) {
    for( auto& i : data )
        if( i->moves_count==field->moves_count+1 ) {
            ;
        };
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Field_cell_type>* field, XY p ) {
    for( auto& i : data )
        if( i->moves_count==field->moves_count+1 ) {
            ;
        };
    return nullptr;
};

void AI_position_directory::delete_nulls() {
    data.erase( remove( data.begin(), data.end(), nullptr ), data.end() );
};

void AI_position_directory::purge( Field<Field_cell_type>* field ) {
    // TODO: реализовать
};

void AI_position_directory::purge( Field<Estimate_field_cell_type>* field ) {
};


AI_position_directory AI_position_recursive::position_directory = {};

AI_position_directory AI_position_static::position_directory = {};

