
    try {
        test_ai( ai );


        cout << endl;
        // для тестирования
        for( auto ii = ai.field->begin_dir_rows(); ii!=ai.field->end_dir_rows(); ++ii ) {
            for( auto j=ii.begin(); j!=ii.end(); ++j )
                cout << (char)*j << " ";
            cout << endl << endl;
        };

        cout << endl << "field_val end iterator:  " << field_val_end(&ai.start_position->estimates_field).get() << endl;
        cout << endl << "field_val  it iterator:  " << field_val_begin(&ai.start_position->estimates_field).get() << endl;
        cout << endl << "output points by val iterator:" << endl;
        for( auto it=field_val_begin(&ai.start_position->estimates_field); it!=field_val_end(&ai.start_position->estimates_field); ++it )
            cout << " ( "<<(((*it)==cross)?"x":"o")<<", "<<it.pivot.x<<", "<<it.pivot.y<<" )    ";
        cout << endl << endl;

    } catch ( exception &e ) {
        cerr << "exception:  " << e.what() << endl;
        return 0xff;
    };
    cout << "Have a nice day! Debug is done" << endl;
