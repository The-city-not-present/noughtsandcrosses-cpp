#include "field_value_iterator.h"

Field_value_iterator::Field_value_iterator( AI_estimates_field* f ) : field(f) {
    pivot = { f->ctx.x_min, f->ctx.y_min };
};

Field_value_iterator::Field_value_iterator( const Field_value_iterator& other ) : field(other.field), pivot(other.pivot) {};

Estimate_field_cell_type* Field_value_iterator::get() {
    return (
        field->ctx.in_range(pivot) ?
        &(*field)[pivot] :
        nullptr
    );
};

const Val Field_value_iterator::operator * () {
    return (
        field->ctx.in_range(pivot) ?
        (Val)((*field)[pivot].player) :
        undefined
    );
};

bool Field_value_iterator::operator == ( Field_value_iterator &other ) {
    return get()==other.get();
};

bool Field_value_iterator::operator != ( Field_value_iterator &other ) {
    return get()!=other.get();
};

bool Field_value_iterator::operator == ( Field_value_iterator other ) {
    return get()==other.get();
};

bool Field_value_iterator::operator != ( Field_value_iterator other ) {
    return get()!=other.get();
};

Field_value_iterator& Field_value_iterator::operator ++() {
    do {
        ( pivot.x<field->ctx.x_max ? pivot.x++ : (pivot.y++,pivot.x=field->ctx.x_min) );
        if( pivot.y>field->ctx.y_max )
            break;
    } while( !(bool)((*field)[pivot]) );
    return *this;
};

Field_value_iterator& Field_value_iterator::operator --() {
    do {
        ( pivot.x>field->ctx.x_min ? pivot.x-- : (pivot.y--,pivot.x=field->ctx.x_max) );
        if( pivot.y<field->ctx.y_min )
            break;
    } while( !(bool)((*field)[pivot]) );
    return *this;
};




Field_value_iterator field_val_begin( AI_estimates_field* field ) {
    auto temp = Field_value_iterator{field};
    if( !(bool)(*temp) )
        ++temp;
    return temp;
};

Field_value_iterator field_val_end( AI_estimates_field* field ) {
    auto temp = Field_value_iterator{field};
    temp.pivot = { field->ctx.x_min, field->ctx.y_max+1 };
    return temp;
};



