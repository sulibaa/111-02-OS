#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>

using namespace std ;

struct Process {
  int ID ;
  int CPU_Burst ;
  int Arrival_Time ;
  int Priority ;
  
  int finish_time ;
  int already_do ;
};

struct Gantt {
  string type ;
  vector<char> token ;
};

struct Ex_Information {
  int ID ;
  int FCFS ;
  int RR ;
  int SJF ;
  int SRTF ;
  int PPRR ;
  int HRRN ;
};

vector<Gantt> Gantt_list ;
vector<Ex_Information> waiting ;
vector<Ex_Information> turnaround ;

char Num2Char( int num )
{
  char re ;
  if ( num == 1 )
    re = '1' ;
  else if ( num == 2 )
    re = '2' ;
  else if ( num == 3 )
    re = '3' ;
  else if ( num == 4 )
    re = '4' ;
  else if ( num == 5 )
    re = '5' ;
  else if ( num == 6 )
    re = '6' ;
  else if ( num == 7 )
    re = '7' ;
  else if ( num == 8 )
    re = '8' ;
  else if ( num == 9 )
    re = '9' ;
  else if ( num == 0 )
    re = '0' ;
  else if ( num >= 10 )
  {
    num = num + 55 ;
    re = char( num ) ;
  } // else if

  return re ;
} // Num2Char()

void pop_front( vector<int>& vec )
{
    assert(!vec.empty());
    vec.erase(vec.begin());
} // pop_front()

int Calculate_Turnaround( int finish, int arrival )
{
  int re = finish - arrival ;
  return re ;
} // Calculate_Turnaround()

int Calculate_Waiting( int turnaround, int burst )
{
  int re = turnaround - burst ;
  return re ;
} // Calculate_Waiting()

void Put_in_queue( vector<int> &queue, vector<Process> list, int count )
{
  for ( int i = 0 ; i < list.size() ; i++ )
    {
      if ( list[i].Arrival_Time == count )
      {
        queue.push_back( list[i].ID ) ;
      } // if
    } // for
} // Put_in_queue()

int Find_smallest_brust_ID( vector<Process> list, int count )
{
  int re = -1 ;
  int smallest_burst_time = 99999 ;
  int smallest_arrival_time = 99999 ;
  for ( int i = 0 ; i < list.size() ; i++ )
  {
    int now_burst_time = list[i].CPU_Burst - list[i].already_do ;
    if ( list[i].Arrival_Time <= count && now_burst_time <= smallest_burst_time && now_burst_time != 0)
    {
      if ( now_burst_time == smallest_burst_time )
      {
        if ( list[i].Arrival_Time < smallest_arrival_time )
        {
          smallest_burst_time = now_burst_time ;
          smallest_arrival_time = list[i].Arrival_Time ;
          re = list[i].ID ;
        } // if
        else if ( list[i].Arrival_Time == smallest_arrival_time )
        {
          if ( list[i].ID < re )
          {
            smallest_burst_time = now_burst_time ;
            smallest_arrival_time = list[i].Arrival_Time ;
            re = list[i].ID ;
          } // if
        } // if
      } // if
      else
      {
        smallest_burst_time = now_burst_time ;
        smallest_arrival_time = list[i].Arrival_Time ;
        re = list[i].ID ;
      } // else
    } // if
  } // for

  return re ;
} // Find_smallest_brust_ID()

int Get_priority( int ID, vector<Process> list )
{
  for ( int i = 0 ; i < list.size() ; i++ )
  {
    if ( list[i].ID == ID )
      return list[i].Priority ;
  } // for

  return -1 ;
} // Get_priority()

int Get_Arrival_Time( int ID, vector<Process> list )
{
  for ( int i = 0 ; i < list.size() ; i++ )
  {
    if ( list[i].ID == ID )
      return list[i].Arrival_Time ;
  } // for

  return -1 ;
} // Get_priority()

void Put_in_priority_queue( vector<int> &queue, vector<Process> list, int count, int time_slice )
{  
  for ( int i = 0 ; i < list.size() ; i++ )
  {
    if ( list[i].Arrival_Time == count )
    {
      if ( queue.size() == 0 ) 
      {
        for ( int k = 0 ; k < time_slice ; k++ )
          queue.push_back( list[i].ID ) ;
      } // if
      else 
      {
      	
        int size_temp = queue.size() ;
        /*
        if ( count < 90 && count >= 70 )
        {
        	cout << count << endl ;
        	for ( int i = 0 ; i < queue.size() ; i++ )
        	  cout << queue[i] << " " ;
  				system("pause") ;
				} // if
				*/
        for ( int j = 0 ; j < size_temp ; j++ )
        {
          if ( list[i].Priority < Get_priority(queue[j], list) )
          {
            for ( int k = 0 ; k < time_slice ; k++ )
              queue.insert( queue.begin() + j, list[i].ID ) ;
            break ;
          } // if
          else if ( list[i].Priority == Get_priority(queue[j], list) )
          {
            if ( list[i].Arrival_Time < Get_Arrival_Time(queue[j], list) )
            {
              for ( int k = 0 ; k < time_slice ; k++ )
                queue.insert( queue.begin() + j, list[i].ID ) ;
              break ;
            } // if
            else if ( list[i].Arrival_Time == Get_Arrival_Time(queue[j], list) )
            {
              if ( list[i].ID < queue[j] )
              {
                for ( int k = 0 ; k < time_slice ; k++ )
                  queue.insert( queue.begin() + j, list[i].ID ) ;
                break ;
              } // if
            } // else if  
          } // else if
        } // for
   
        if ( queue.size() == size_temp )
          for ( int k = 0 ; k < time_slice ; k++ )
            queue.push_back( list[i].ID ) ;
      } // else        
    } // if
  } // for
  
} // Put_in_priority_queue()

int Find_highest_ratio_ID( vector<Process> list, int count )
{
  int re = -1 ;
  float highest_ratio_time = -99999 ;
  int smallest_arrival_time = 99999 ;
  for ( int i = 0 ; i < list.size() ; i++ )
  {
    float now_ratio_time = float((count + list[i].CPU_Burst - list[i].Arrival_Time))/float(list[i].CPU_Burst);
    //if ( list[i].Arrival_Time <= count )cout << list[i].ID << " " << now_ratio_time << endl;
    if ( list[i].Arrival_Time <= count && now_ratio_time >= highest_ratio_time 
         && list[i].already_do != list[i].CPU_Burst )
    {
      if ( now_ratio_time == highest_ratio_time )
      {
        if ( list[i].Arrival_Time < smallest_arrival_time )
        {
          highest_ratio_time = now_ratio_time ;
          smallest_arrival_time = list[i].Arrival_Time ;
          re = list[i].ID ;
        } // if
        else if ( list[i].Arrival_Time == smallest_arrival_time )
        {
          if ( list[i].ID < re )
          {
            highest_ratio_time = now_ratio_time ;
            smallest_arrival_time = list[i].Arrival_Time ;
            re = list[i].ID ;
          } // if
        } // else if
      } // if
      else
      {
        highest_ratio_time = now_ratio_time ;
        smallest_arrival_time = list[i].Arrival_Time ;
        re = list[i].ID ;
      } // else
    } // if
  } // for

  return re ;
} // Find_highest_ratio_ID()

void FCFS( vector<Process> list )
{
  Process temp ;  
  for( int i = list.size()-1 ; i > 0 ; i-- )
  {
    for( int j = 0 ; j <= i-1 ; j++ )
    {
      if( list[j].Arrival_Time > list[j+1].Arrival_Time )
      {
        temp = list[j];
        list[j] = list[j+1];
        list[j+1] = temp;
      } // if
      else if( list[j].Arrival_Time == list[j+1].Arrival_Time )
      {
        if ( list[j].ID > list[j+1].ID ) 
        {
          temp = list[j];
          list[j] = list[j+1];
          list[j+1] = temp;
        } // if
      } // else if
    } // for
  } // for

  vector<int> queue ;
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完 
  int count = 0 ;
  Gantt temp_g ;
  
  Put_in_queue( queue, list, count ) ;
  while ( fully_brust < list.size() )
  {
    if( queue.size() != 0 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == queue[0] )
        {
          for ( int j = 0 ; j < list[i].CPU_Burst ; j++ )
          {
            temp_g.token.push_back(Num2Char( list[i].ID )) ;
            count++ ;
            Put_in_queue( queue, list, count ) ;
          } // for
          fully_brust++ ;
          list[i].finish_time = count ;
          break ;
        } // if
      } // for
      pop_front( queue ) ;
    } // if
    else
    {
      count++ ;
      temp_g.token.push_back('-');
      Put_in_queue( queue, list, count ) ;
    } // else
  } // while
  temp_g.type = "FCFS" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].FCFS = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].FCFS = Calculate_Waiting( turnaround[j].FCFS, list[i].CPU_Burst ) ;
        break ;
      } // if
    } // for  
  } // for
} // FCFS()

void RR( vector<Process> list, int time_slice )
{
  int count = 0 ;
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完
  Gantt temp_g ;
  vector<int> queue ;
  Put_in_queue( queue, list, count ) ;
  while ( fully_brust < list.size() )
  {
    if( queue.size() != 0 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == queue[0] )
        {
          for ( int j = 0 ; j < time_slice && list[i].already_do != list[i].CPU_Burst ; j++ )
          {
            list[i].already_do++ ;
            count++ ;
            Put_in_queue( queue, list, count ) ;
            temp_g.token.push_back(Num2Char( list[i].ID )) ;
          } // for
          if ( list[i].already_do == list[i].CPU_Burst ) 
          {
            fully_brust++ ;
            list[i].finish_time = count ;
          } // if
          else
          {
            queue.push_back( list[i].ID ) ;
          } // else
          pop_front( queue ) ;
          break ;
        } // if
      } // for
      
    } // if
    else
    {
      count++ ;
      temp_g.token.push_back('-');
      Put_in_queue( queue, list, count ) ; 
    } // else
  } // while

  temp_g.type = "RR" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].RR = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].RR = Calculate_Waiting( turnaround[j].RR, list[i].CPU_Burst ) ;
        break ;
      } // if
    } // for  
  } // for
  /*
  for ( int a = 0 ; a < temp_g.token.size() ; a++ )
  {
    cout << temp_g.token[a] ;
  } // for
  cout << endl ;
  for ( int a = 0 ; a < turnaround.size() ; a++ )
  {
    cout << turnaround[a].ID << " " << turnaround[a].RR << endl;
  } // for
  for ( int a = 0 ; a < waiting.size() ; a++ )
  {
    cout << waiting[a].ID << " " << waiting[a].RR << endl;
  } // for
  */
} // RR()
 
void SJF( vector<Process> list )
{
	int next_ID = -1 ; // 下一個COUNT要處理的ID 若沒有則顯示-1 
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完 
  int count = 0 ;
  Gantt temp_g ;
  
  next_ID = Find_smallest_brust_ID( list, count ) ;
  while ( fully_brust < list.size() )
  {
    if( next_ID != -1 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == next_ID )
        {
        	while ( list[i].already_do != list[i].CPU_Burst )
        	{
        		temp_g.token.push_back(Num2Char( list[i].ID )) ;
          	list[i].already_do++ ;
          	count++ ;
					} // while
          
          fully_brust++ ;
          list[i].finish_time = count ;
          
          break ;
        } // if
      } // for

      next_ID = Find_smallest_brust_ID( list, count ) ;
    } // if
    else
    {
    	count++ ;
      temp_g.token.push_back('-');
      next_ID = Find_smallest_brust_ID( list, count ) ;
    } // else
  } // while

  temp_g.type = "SJF" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].SJF = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].SJF = Calculate_Waiting( turnaround[j].SJF, list[i].CPU_Burst ) ;
        break ;
      } // if
    } // for  
  } // for
} // SJF 
 
void SRTF( vector<Process> list )
{
  int next_ID = -1 ; // 下一個COUNT要處理的ID 若沒有則顯示-1 
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完 
  int count = 0 ;
  Gantt temp_g ;
  
  next_ID = Find_smallest_brust_ID( list, count ) ;
  while ( fully_brust < list.size() )
  {
    if( next_ID != -1 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == next_ID )
        {
          temp_g.token.push_back(Num2Char( list[i].ID )) ;
          list[i].already_do++ ;
          count++ ;
          if ( list[i].already_do == list[i].CPU_Burst )
          {
            fully_brust++ ;
            list[i].finish_time = count ;
          } // if
          
          break ;
        } // if
      } // for

      next_ID = Find_smallest_brust_ID( list, count ) ;
    } // if
    else
    {
      count++ ;
      temp_g.token.push_back('-');
      next_ID = Find_smallest_brust_ID( list, count ) ;
    } // else
  } // while

  temp_g.type = "SRTF" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].SRTF = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].SRTF = Calculate_Waiting( turnaround[j].SRTF, list[i].CPU_Burst ) ;
        break ;
      } // if
    } // for  
  } // for
  /*
  for ( int a = 0 ; a < temp_g.token.size() ; a++ )
  {
    cout << temp_g.token[a] ;
  } // for
  cout << endl ;
  for ( int a = 0 ; a < turnaround.size() ; a++ )
  {
    cout << turnaround[a].ID << " " << turnaround[a].SRTF << endl;
  } // for
  for ( int a = 0 ; a < waiting.size() ; a++ )
  {
    cout << waiting[a].ID << " " << waiting[a].SRTF << endl;
  } // for
  */
} // SRTF()

void PPRR( vector<Process> list, int time_slice )
{
  int count = 0 ;
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完
  Gantt temp_g ;
  vector<int> queue ;

  Put_in_priority_queue( queue, list, count, time_slice ) ;
  while ( fully_brust < list.size() )
  {
    if( queue.size() != 0 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == queue[0] )
        {
          if ( list[i].already_do < list[i].CPU_Burst )
          {
            for( int j = 0 ; j < time_slice && list[i].already_do != list[i].CPU_Burst ; j++ )
            {
              temp_g.token.push_back(Num2Char( list[i].ID )) ;
              list[i].already_do++ ;
              count++ ;
              pop_front( queue ) ;
              Put_in_priority_queue( queue, list, count, time_slice ) ;
              if ( list[i].ID != queue[0] ) //  有更小的priority加入 
              {
              	for ( int k = 0 ; k < queue.size() ; k++ )
              	 	if ( queue[k] == list[i].ID )
              	 		queue.erase( queue.begin() + k ) ;
                break ;
              } // if
            } // for
            
            if ( list[i].already_do == list[i].CPU_Burst ) 	// finish
            {
              fully_brust++ ;
              list[i].finish_time = count ;
            } // else            
            else																						// not yet
            {
              int size_temp = queue.size() ;
              for ( int j = 0 ; j < size_temp ; j++ )
              {
                if ( list[i].Priority < Get_priority(queue[j], list) )
                {
                  for ( int k = 0 ; k < time_slice ; k++ )
                    queue.insert( queue.begin() + j, list[i].ID ) ;
                  break ;
                } // if 
              } // for
              if ( queue.size() == size_temp ) 
                for ( int k = 0 ; k < time_slice ; k++ )
                  queue.push_back( list[i].ID ) ;
            } // else
            break ;
          } // if
          else
          {
            pop_front( queue ) ;
            break ;
          } // else 
        } // if        
      } // for
    } // if
    else
    {
      count++ ;
      temp_g.token.push_back('-');
      Put_in_priority_queue( queue, list, count, time_slice ) ;
      
    } // else
    
     
  } // while

  temp_g.type = "PPRR" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].PPRR = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].PPRR = Calculate_Waiting( turnaround[j].PPRR, list[i].CPU_Burst ) ;
        break ;
      } // if 
    } // for  
  } // for
} // PPRR()

void HRRN( vector<Process> list )
{
  int next_ID = -1 ; // 下一個COUNT要處理的ID 若沒有則顯示-1 
  int fully_brust = 0 ; // brust完成的數目 用來判斷是否做完 
  int count = 0 ;
  Gantt temp_g ;
  
  next_ID = Find_highest_ratio_ID( list, count ) ;
  while ( fully_brust < list.size() )
  {
    if( next_ID != -1 )
    {
      for ( int i = 0 ; i < list.size() ; i++ )
      {
        if( list[i].ID == next_ID )
        {
          for ( int j = 0 ; j < list[i].CPU_Burst ; j++ )
          {
            temp_g.token.push_back(Num2Char( list[i].ID )) ;
            list[i].already_do++ ;
            count++ ;
          } // for
          
          fully_brust++ ;
          list[i].finish_time = count ;
          break ;
        } // if
      } // for

      next_ID = Find_highest_ratio_ID( list, count ) ;
    } // if
    else
    {
      count++ ;
      temp_g.token.push_back('-');
      next_ID = Find_highest_ratio_ID( list, count ) ;
    } // else
  } // while

  temp_g.type = "HRRN" ;
  Gantt_list.push_back( temp_g ) ;

  for ( int i = 0 ; i < list.size() ; i++ )
  {
    for ( int j = 0 ; j < waiting.size() ; j++ )
    {
      if ( list[i].ID == waiting[j].ID )
      {
        turnaround[j].HRRN = Calculate_Turnaround( list[i].finish_time, list[i].Arrival_Time ) ;
        waiting[j].HRRN = Calculate_Waiting( turnaround[j].HRRN, list[i].CPU_Burst ) ;
        break ;
      } // if 
    } // for  
  } // for
  /*
  for ( int a = 0 ; a < temp_g.token.size() ; a++ )
  {
    cout << temp_g.token[a] ;
  } // for
  cout << endl ;
  for ( int a = 0 ; a < turnaround.size() ; a++ )
  {
    cout << turnaround[a].ID << " " << turnaround[a].HRRN << endl;
  } // for
  for ( int a = 0 ; a < waiting.size() ; a++ )
  {
    cout << waiting[a].ID << " " << waiting[a].HRRN << endl;
  } // for
  */
} // SRTF()

void Print_all( int command_num, string file_name )
{
  file_name = "out_" + file_name ;
  ofstream fout( file_name.c_str() );
  if ( command_num == 1 ) fout << "FCFS" << endl ;
  else if ( command_num == 2 ) fout << "RR" << endl ;
  else if ( command_num == 3 ) fout << "SJF" << endl ;
  else if ( command_num == 4 ) fout << "SRTF" << endl ;
  else if ( command_num == 5 ) fout << "HRRN" << endl ;
  else if ( command_num == 6 ) fout << "PPRR" << endl ;
  else if ( command_num == 7 ) fout << "All" << endl ;
 
  if ( command_num == 1 || command_num == 7 ) 
  {
    fout << "==        FCFS==" << endl ;
    for ( int i = 0 ; i < Gantt_list[0].token.size() ; i++ )
    {
      fout << Gantt_list[0].token[i] ;
    } // for
    fout << endl ;
  } // if
  if ( command_num == 2 || command_num == 7 ) 
  {
    fout << "==          RR==" << endl ;
    for ( int i = 0 ; i < Gantt_list[1].token.size() ; i++ )
    {
      fout << Gantt_list[1].token[i] ;
    } // for
    fout << endl ;
  } // if
  if ( command_num == 3 || command_num == 7 ) 
  {
    fout << "==         SJF==" << endl ;
    for ( int i = 0 ; i < Gantt_list[2].token.size() ; i++ )
    {
      fout << Gantt_list[2].token[i] ;
    } // for
    fout << endl ;
  } // if
  if ( command_num == 4 || command_num == 7 ) 
  {
    fout << "==        SRTF==" << endl ;
    for ( int i = 0 ; i < Gantt_list[3].token.size() ; i++ )
    {
      fout << Gantt_list[3].token[i] ;
    } // for
    fout << endl ;
  } // if
  if ( command_num == 5 || command_num == 7 ) 
  {
    fout << "==        HRRN==" << endl ;
    for ( int i = 0 ; i < Gantt_list[4].token.size() ; i++ )
    {
      fout << Gantt_list[4].token[i] ;
    } // for
    fout << endl ;
  } // if
  if ( command_num == 6 || command_num == 7 ) 
  {
    fout << "==        PPRR==" << endl ;
    for ( int i = 0 ; i < Gantt_list[5].token.size() ; i++ )
    {
      fout << Gantt_list[5].token[i] ;
    } // for
    fout << endl ;
  } // if
  fout << "===========================================================" << endl << endl ;
  fout << "Waiting Time" << endl ;
  fout << "ID" << "\t" ;
  if ( command_num == 1 || command_num == 7 ) 
  {
    fout << "FCFS" << "\t" ;
  } // if
  if ( command_num == 2 || command_num == 7 ) 
  {
    fout << "RR" << "\t" ;
  } // if
  if ( command_num == 3 || command_num == 7 ) 
  {
    fout << "SJF" << "\t" ;
	} // IF
  if ( command_num == 4 || command_num == 7 ) 
  {
    fout << "SRTF" << "\t" ;
  } // if
  if ( command_num == 5 || command_num == 7 ) 
  {
    fout << "HRRN" << "\t" ;
  } // if
  if ( command_num == 6 || command_num == 7 ) 
  {
    fout << "PPRR" << "\t" ;
  } // if  
  fout << endl << "===========================================================" << endl ;
  for ( int i = 0 ; i < waiting.size() ; i++ )
  {
    fout << waiting[i].ID << "\t" ;
    if ( command_num == 1 || command_num == 7 ) 
    {
      fout << waiting[i].FCFS << "\t" ;
    } // if
    if ( command_num == 2 || command_num == 7 ) 
    {
      fout << waiting[i].RR << "\t" ;
    } // if
    if ( command_num == 3 || command_num == 7 ) 
    {
      fout << waiting[i].SJF << "\t" ;
    } // if
    if ( command_num == 4 || command_num == 7 ) 
    {
      fout << waiting[i].SRTF << "\t" ;
    } // if
    if ( command_num == 5 || command_num == 7 ) 
    {
      fout << waiting[i].HRRN << "\t" ;
    } // if
    if ( command_num == 6 || command_num == 7 ) 
    {
      fout << waiting[i].PPRR << "\t" ;
    } // if
    fout << endl ;
  } // for
  fout << "===========================================================" << endl << endl ;
  fout << "Turnaround Time" << endl ;
  fout << "ID" << "\t" ;
  if ( command_num == 1 || command_num == 7 ) 
  {
    fout << "FCFS" << "\t" ;
  } // if
  if ( command_num == 2 || command_num == 7 ) 
  {
    fout << "RR" << "\t" ;
  } // if
  if ( command_num == 3 || command_num == 7 ) 
  {
    fout << "SJF" << "\t" ;
  } // if
  if ( command_num == 4 || command_num == 7 ) 
  {
    fout << "SRTF" << "\t" ;
  } // if
  if ( command_num == 5 || command_num == 7 ) 
  {
    fout << "HRRN" << "\t" ;
  } // if
  if ( command_num == 6 || command_num == 7 ) 
  {
    fout << "PPRR" << "\t" ;
  } // if  
  fout << endl << "===========================================================" << endl ;
  for ( int i = 0 ; i < turnaround.size() ; i++ )
  {
    fout << turnaround[i].ID << "\t" ;
    if ( command_num == 1 || command_num == 7 ) 
    {
      fout << turnaround[i].FCFS << "\t" ;
    } // if
    if ( command_num == 2 || command_num == 7 ) 
    {
      fout << turnaround[i].RR << "\t" ;
    } // if
    if ( command_num == 3 || command_num == 7 ) 
    {
      fout << turnaround[i].SJF << "\t" ;
    } // if
    if ( command_num == 4 || command_num == 7 ) 
    {
      fout << turnaround[i].SRTF << "\t" ;
    } // if
    if ( command_num == 5 || command_num == 7 ) 
    {
      fout << turnaround[i].HRRN << "\t" ;
    } // if
    if ( command_num == 6 || command_num == 7 ) 
    {
      fout << turnaround[i].PPRR << "\t" ;
    } // if
    fout << endl ;
  } // for
  fout << "===========================================================" << endl << endl ;
  fout.close() ;
} // Print_all()

int main()
{
  ifstream fin ;
  bool open = false ;
  string file_name ;
  while ( !open )
  {   
    cout << "please enter the file name: " ;
    cin >> file_name ;
    file_name += ".txt" ;
    fin.open( file_name.c_str() ) ;
    if( !fin )
    {
  		cout << file_name << " does not exist!" << endl ;
      open = false ;
    } // if
    else 
      open = true ;
  } // while

  int command_num ;
  int time_slice ;
  string temp ;
  Process p ;
  Ex_Information ex ;
  vector<Process> p_list ;

  fin >> command_num >> time_slice ;
  getline( fin, temp ) ;
  getline( fin, temp ) ;
  while( fin )
  {
    fin >> p.ID >> p.CPU_Burst >> p.Arrival_Time >> p.Priority ;
    p.already_do = 0 ;
    ex.ID = p.ID ;
    p_list.push_back( p ) ;
    waiting.push_back( ex ) ;
    turnaround.push_back( ex ) ;
  } // while
  p_list.pop_back() ;
  waiting.pop_back() ;
  turnaround.pop_back() ;
  fin.close() ;
  
  Process p_temp ;
  Ex_Information ex_temp ;
  for( int i = waiting.size()-1 ; i > 0 ; i-- ) // 以ID大小排序做成的 waiting & turnaround time 
  {
    for( int j = 0 ; j <= i-1 ; j++ )
    {
      if( waiting[j].ID > waiting[j+1].ID )
      {
        p_temp = p_list[j];
        p_list[j] = p_list[j+1];
        p_list[j+1] = p_temp;
        ex_temp = waiting[j];
        waiting[j] = waiting[j+1];
        waiting[j+1] = ex_temp;
        ex_temp = turnaround[j];
        turnaround[j] = turnaround[j+1];
        turnaround[j+1] = ex_temp;
      } // if
    } // for
  } // for

  FCFS( p_list ) ;
  RR( p_list, time_slice ) ;
  SJF( p_list ) ;
  SRTF( p_list ) ;
  HRRN( p_list ) ;
  PPRR( p_list, time_slice ) ;

  Print_all( command_num, file_name ) ;

  cout << "done~" ;
} // main()


