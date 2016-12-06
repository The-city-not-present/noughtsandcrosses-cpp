
    try {
        test_ai( ai );


        cout << endl;
        // для тестирования
        for( auto ii = ai.field->begin_dir_rows(); ii!=ai.field->end_dir_rows(); ++ii ) {
            for( auto j=ii.begin(); j!=ii.end(); ++j )
                cout << (char)*j << " ";
            cout << endl << endl;
        };
        //test_field();
    } catch ( exception &e ) {
        cerr << "exception:  " << e.what() << endl;
        return 0xff;
    };
    cout << "Have a nice day! Debug is done" << endl;
