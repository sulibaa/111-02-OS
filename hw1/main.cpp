#include "Sort.h"
#include "Timer.h"

bool Write_into_seq( string fileName, vector<int> &orig_seq ) {
  ifstream fin;
  fin.open( fileName );
  if ( ! fin.is_open() ) {
    fin.open( fileName + ".txt" );
    if ( ! fin.is_open() ) {
      return false;
    } // if
  } // if

  string str;
  int n;
  do {
    getline( fin, str );
    if ( ! str.empty() ) {
        n = stoi( str ); // str_to_int
        orig_seq.push_back( n );
    } // if
  } while ( ! fin.eof() );

  fin.close();
  return true;
} // Write_into_seq()

bool Print_file( string fileName, const vector<int> orig_seq, milliseconds cpuTime, int sel ) {
  ofstream fout;

  fout.open( fileName + "_output" + to_string( sel ) + ".txt" );
  fout << "Sort:" << endl;
  for ( int i = 0 ; i < orig_seq.size() ; i++ ) {
    //cout << orig_seq.at( i ) << endl;
    fout << orig_seq.at( i ) << endl;
  } // for

  fout << "CPU Time: " << ( double )cpuTime.count() / 1000.0 << endl;
  fout << "Output Time: " << Get_time();
  fout.close();
  return true;
} // Print_file()

int main() {
  string fileName, str;
  int sel, slice_num;
  vector<int> orig_seq;
  bool exit = false;
  do {
    orig_seq.clear();
    cout << "Please enter the file's name:" << endl;
    cin >> fileName;
    while ( ! Write_into_seq( fileName, orig_seq ) ) {
      cout << "Can't find the file ! Please enter again:" << endl;
      cin >> fileName;
    } // while

    cout << "1.Bubble sort" << endl;
    cout << "2.Bubble + merge sort" << endl;
    cout << "3.Mul processes" << endl;
    cout << "4.Mul threads" << endl;
    cout << "Select an option: " << endl;
    cin >> sel;
    while ( ! ( sel >= 1 && sel <= 4 ) ) {
      cout << "Error! Select againg" << endl;
      cout << "1.Bubble sort" << endl;
      cout << "2.Bubble + merge sort" << endl;
      cout << "3.Mul processes" << endl;
      cout << "4.Mul threads" << endl;
      cout << "Select an option: " << endl;
      cin >> sel;
    } // while

    int seq_size = orig_seq.size();
    // using pointer is easer to do mul_p & mul_d
    int *arr = new int[ seq_size ];
    copy( orig_seq.begin(), orig_seq.end(), arr );
    orig_seq.clear() ;

    if ( sel == 1 ) {
      slice_num = 1;
    } // if
    else {
      cout << "How much to divide:" << endl;
      cin >> slice_num;
      while ( ! ( slice_num > 0 && slice_num <= seq_size ) ) {
        cout << "Error! Please enter again:" << endl;
        cout << "How much to divide:" << endl;
        cin >> slice_num;
      } // while
    } // else

    steady_clock::time_point start_t, end_t;
    start_t = steady_clock::now();  // timer start
    cout << "sorting..." << endl;
    Start_sort( arr, seq_size, slice_num, sel ); // start sorting
    end_t = steady_clock::now();  // timer stop
    milliseconds used_t = duration_cast<milliseconds>( end_t - start_t );
    vector<int> final_seq ;
    copy( &arr[ 0 ], &arr[ seq_size ], back_inserter( final_seq ) );
    delete[] arr;

    Print_file( fileName, final_seq, used_t, sel );
    cout << "done" << endl;

    final_seq.clear();

    cout << "continue? (Y/y)" << endl;
    cin >> str;
    if ( str != "Y" && str != "y" ) {
      exit = true;
    } // if
  } while ( ! exit );
} // main()
