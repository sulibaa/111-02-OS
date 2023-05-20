#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <iomanip>
# include <sstream>
# include <unistd.h>
# include <pthread.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ipc.h>
# include <sys/shm.h>

using namespace std;

struct Arr_args{
  int *arr ;
  int start ;
  int mid ;
  int coda ;
};

void Bubble_Sort( int *arr, int start, int coda ) {
  for ( int i = coda - 1 ; i >= start ; i-- ) {
    for ( int j = start ; j < i ; j++ ) {
      if ( arr[ j ] > arr[ j + 1 ] ) {
        swap( arr[ j ], arr[ j + 1 ] );
      } // if
    } // for
  } // for
} // Bubble_Sort()

void *Bubble_Sort( void *args ){
  Arr_args *bubble_args = ( Arr_args * )args;
  Bubble_Sort( bubble_args->arr, bubble_args->start, bubble_args->coda );
  pthread_exit( 0 );
} // Bubble_Sort()

void Merge( int *arr, int start, int mid, int coda ) {
  int nSize1 = mid - start;
  int nSize2 = coda - mid;
  int *arr1 = new int[ nSize1 ];
  int *arr2 = new int[ nSize2 ];
  copy( arr + start, arr + mid, arr1 );
  copy( arr + mid, arr + coda, arr2 );

  int i = 0, j = 0;
  int k = start;
  for ( ; k < coda && i < nSize1 && j < nSize2 ; k++ ) {
    if ( arr1[ i ] <= arr2[ j ] ) {
      arr[ k ] = arr1[ i ];
      i++;
    } // if
    else {
      arr[ k ] = arr2[ j ];
      j++;
    } // else
  } // for

  if ( i == nSize1 ) {
    copy( arr2 + j, arr2 + nSize2, arr + k );
  } // if
  else if ( j == nSize2 ) {
    copy( arr1 + i, arr1 + nSize1, arr + k );
  } // else if

  delete[] arr1;
  delete[] arr2;
} // Merge()

void *Merge( void *args ) {
  Arr_args *merge_args = ( Arr_args * )args;
  Merge( merge_args->arr, merge_args->start, merge_args->mid, merge_args->coda );
  pthread_exit( 0 );
} // Merge()

void Merge_Update( int *div_index, int &last_part ) {
  int next_part = last_part / 2 + last_part % 2;  // num of next round partition
  for ( int i = 0 ; i < next_part ; i++ ) {
    div_index[ i ] = div_index[ i * 2 ];
  } // for

  div_index[ next_part ] = div_index[ last_part ];
  last_part = next_part;
} // Merge_Update()

void Cut_arr( int *arr, int slice_num, int arr_size ) {
  int sizeOfPartition = arr_size / slice_num + 1 * ( arr_size % slice_num > 0 );
  arr[ 0 ] = 0;
  for ( int i = 1 ; i < slice_num + 1 ; i++ ) {
    arr[ i ] = arr[ i - 1 ] + sizeOfPartition;
    if ( arr[ i ] > arr_size ) {
      arr[ i ] = arr_size;
    } // if
  } // for
} // Cut_arr()

void Start_sort( int *arr, int arr_size, int slice_num, int sel ){
  if ( sel == 1 ){ // bubble
    Bubble_Sort( arr, 0, arr_size );
  } // if
  else if ( sel == 2 ){ // bubble and merge
    int *div_index = new int[slice_num+1] ; // save index of the head of each part of arr
    Cut_arr( div_index, slice_num, arr_size ) ;
    for ( int i = 0 ; i < slice_num ; i++ )
      Bubble_Sort( arr, div_index[i], div_index[i+1] );
    int last_part = slice_num ; // remain part of slice
    while ( last_part > 1 ){ // merge until only one arr
      for ( int i = 0 ; i < last_part - 1 ; i += 2 ) {
        Merge( arr, div_index[ i ], div_index[ i + 1 ], div_index[ i + 2 ] );
      } // for

      Merge_Update( div_index, last_part );
    } // while

    delete[] div_index;
  } // else if
  else if ( sel == 3 ){ // mul processes
  // int shmget(key_t key, size_t size, int shmflg)
  // key : IPC 鍵值
  // size : 共享儲存的長度
  // shmfig: 函數的行為和權限

    int shmid = shmget( IPC_PRIVATE, arr_size * sizeof( int ), IPC_CREAT | 0600 );
    int *shmaddr = NULL;

    int *div_index = new int[slice_num+1] ; // save index of the head of each part of arr
    Cut_arr( div_index, slice_num, arr_size ) ;
    vector<pid_t> pids;
    for ( int i = 0 ; i < slice_num ; i++ ) {
      pid_t pid = fork();
      if ( pid == 0 ) {  // child process
        shmaddr = ( int * )shmat( shmid, NULL, 0 );
        copy( arr + div_index[ i ], arr + div_index[ i + 1 ], shmaddr + div_index[ i ] );
        Bubble_Sort( shmaddr, div_index[ i ], div_index[ i + 1 ] );
        shmdt( shmaddr );
        exit( EXIT_SUCCESS );
      } // else if
      else {  // parent process
        pids.push_back( pid );
      } // else
    } // for

    for ( int i = 0 ; i < pids.size() ; i++ ) {
      waitpid( pids.at( i ), nullptr, 0 );
    } // for

    int last_part = slice_num;
    while ( last_part > 1 ) {
      pids.clear();
      for ( int i = 0 ; i < last_part - 1 ; i += 2 ) {
        pid_t pid = fork();
        if ( pid == -1 ) {  // error
          perror("fork error");
          exit( EXIT_FAILURE );
        } // if
        else if ( pid == 0 ) {  // child process
          shmaddr = ( int * )shmat( shmid, NULL, 0 );
          Merge( shmaddr, div_index[ i ], div_index[ i + 1 ], div_index[ i + 2 ] );
          shmdt( shmaddr );
          exit( EXIT_SUCCESS );
        } // else if
        else {  // parent process
          pids.push_back( pid );
        } // else
      } // for

      for ( int i = 0 ; i < pids.size() ; i++ ) {
        waitpid( pids.at( i ), nullptr, 0 );
      } // for

      Merge_Update( div_index, last_part );
    } // while

    delete[] div_index;
    shmaddr = ( int * )shmat( shmid, NULL, 0 );
    copy( shmaddr, shmaddr + arr_size, arr );
    shmdt( shmaddr );
    shmctl( shmid, IPC_RMID, NULL );
  } // else if
  else if ( sel == 4 ){ // mul threads
    int *div_index = new int[slice_num+1] ; // save index of the head of each part of arr
    Cut_arr( div_index, slice_num, arr_size ) ;
    vector<pthread_t> threads;
    Arr_args **bubble_arr = new Arr_args*[slice_num]; // save a set of arrays to be sorting
    for ( int i = 0 ; i < slice_num ; i++ ){
      pthread_t t;
      bubble_arr[i] = new Arr_args ;
      bubble_arr[i]->arr = arr ;
      bubble_arr[i]->start = div_index[i];
      bubble_arr[i]->coda = div_index[i+1]  ;
      pthread_create( &t, NULL, Bubble_Sort, ( void * )bubble_arr[i] );
      threads.push_back( t );
    } // for

    for ( int i = 0 ; i < threads.size() ; i++ ) {
      pthread_join( threads.at( i ), NULL );
      delete[] bubble_arr[i] ;
    } // for
    delete[] bubble_arr;

    int last_part = slice_num ; // remain part of slice
    while ( last_part > 1 ){ // merge until only one arr
      threads.clear();
      Arr_args **merge_arr = new Arr_args*[last_part/2];// save a set of arrays to be sorting
      for ( int i = 0 ; i < last_part - 1 ; i += 2 ) {
        pthread_t t;
        merge_arr[i/2] = new Arr_args ;
        merge_arr[i/2]->arr = arr ;
        merge_arr[i/2]->start = div_index[i];
        merge_arr[i/2]->mid = div_index[i+1];
        merge_arr[i/2]->coda = div_index[i+2]  ;
        pthread_create( &t, NULL, Merge, ( void * )merge_arr[i/2] );
        threads.push_back( t );
      } // for

      for ( int i = 0 ; i < threads.size() ; i++ ) {
        pthread_join( threads.at( i ), NULL );
        delete[] merge_arr[ i ];
      } // for
      delete[] merge_arr;

      Merge_Update( div_index, last_part );
    } // while

    delete[] div_index;

  } // else if

}

#endif // SORT_H_INCLUDED
