#include "datalist.h"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataList::DataList(){
    }
////////////////////////////////////////////////////////////////////////////////
    DataList::~DataList(){
    }
////////////////////////////////////////////////////////////////////////////////
    void
    DataList::AddData(Data *data){
        data_array.push_back(data);
    }
///////////////////////////////////////////////////////////////////////////////
     int
     DataList::Size(){
        return data_array.size();
     }
///////////////////////////////////////////////////////////////////////////////
     Data*
     DataList::GetDataForTime(time_t _time){
         std::vector<Data*>::const_iterator next_it, it;
         Data* temp_data, temp_data_result;
         for(it=this->data_array.begin(); it!=this->data_array.end(); ++it) {
            ++(next_it = it);
            temp_data =*it;
         //   if (temp_data)
         }
         return data_array[0];
     }

////////////////////////////////////////////////////////////////////////////////
} // namespace Core
