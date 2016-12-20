#ifndef FIELD_VALUE_ITERATOR_H
#define FIELD_VALUE_ITERATOR_H

#include "ai/ai_estimates_field.h"

class Field_value_iterator : std::iterator<std::input_iterator_tag, Estimate_field_cell_type> {
    public:
        Field_value_iterator() = delete;
        Field_value_iterator( AI_estimates_field* );
        Field_value_iterator( const Field_value_iterator& );

        const Val operator * ();

        bool operator == ( Field_value_iterator& );
        bool operator != ( Field_value_iterator& );
        bool operator == ( Field_value_iterator );
        bool operator != ( Field_value_iterator );
        Field_value_iterator& operator ++();
        Field_value_iterator& operator --();

        friend class AI_estimates_field;
        friend Field_value_iterator field_val_begin( AI_estimates_field* );

        XY pivot;
        Estimate_field_cell_type* get();
    protected:
        AI_estimates_field* field;

    private:
};

#endif // FIELD_VALUE_ITERATOR_H

Field_value_iterator field_val_begin( AI_estimates_field* field );

Field_value_iterator field_val_end( AI_estimates_field* field );


