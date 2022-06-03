#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "npy.hpp"

using namespace std;


#define WINDOW_TIME 20e3 //Measured in picoseconds
#define AP_TIME 30e3 //Afterpulse time, also in ps


#define PREFIX "ArLamp_30min_sweep_2Lamps_"

#define FILE_MIN 0
#define FILE_MAX 2

//Loads data into a size-4 array of timestamp vectors (each vector correlating to a channel)
void load_bin(std::string filename, vector<long long> * timestamps)
{

     //vector <long long> timestamps [4];

     


      long long time_val=0;
      short channel_val;


      timestamps[0].push_back(0);
      timestamps[1].push_back(0);
      timestamps[2].push_back(0);
      timestamps[3].push_back(0);

      ifstream in;
      //in.open(argv[1]+std::string("0.bin"), std::ios::binary);
      in.open(filename, std::ios::binary);
      in.seekg(40);









      std::cout<<"[";
      int i=0;
      int ap_count=0;

      while (in){
            in.read(reinterpret_cast<char *>(&time_val), sizeof(time_val));
            in.read(reinterpret_cast<char *>(&channel_val), sizeof(channel_val));

            if (channel_val <5 && channel_val >0)
            {   
                  if (time_val - timestamps[channel_val-1].back() < AP_TIME) //Check if this is afterpulse
                  {
                        ap_count++;
                    
                  }else{ //Add if not an afterpulse
                        timestamps[channel_val-1].push_back(time_val);
                  }
            }



            i++;
            if (i%20000000==0) // Counting amount of entries
            {
                  std::cout<<"#"<<std::flush;
            }
      }
      std::cout<<"]"<<std::endl;

      std::cout<<"File read, channel counts in file:"<<" c1 "<<timestamps[0].size() << " c2 "<<timestamps[1].size() << " c3 "<<timestamps[2].size() << " c4 "<<timestamps[3].size() << std::endl;
      std::cout << ap_count << " Afterpulses in data (" << float(ap_count)/float(timestamps[0].size()+timestamps[1].size()+timestamps[2].size()+timestamps[3].size()+ap_count)*100.0<<"%)"<< std::endl;





}


unsigned int recordPairs(vector<long long> *timestamps, vector <long long> *h12,vector <long long> *h13,vector <long long> *h14,vector <long long> *h23,vector <long long> *h24,vector <long long> *h34)
{


      vector <unsigned int> d12,d13,d14,d23,d24,d34;


      long long time_val=0;
      unsigned int index[4];

      index[0]=1;
      index[1]=1;
      index[2]=1;
      index[3]=1;

      unsigned int pairs12=0,pairs13=0,pairs14=0,pairs23=0,pairs24=0,pairs34=0;
      
      long long channel1time,channel2time,channel3time,channel4time,time_diff;

      //Pass 1: pairs 1-2,1-3,1-4

      
      while (index[0]<timestamps[0].size())
      {
            channel1time=timestamps[0][index[0]];

            for (int channel=2;channel<=4;channel++)
            {
                  if (index[channel-1]<timestamps[channel-1].size())
                  {
                        do {
                              time_val=timestamps[channel-1][index[channel-1]];
                              time_diff=channel1time-time_val;

                              if (time_diff<=WINDOW_TIME && time_diff>=WINDOW_TIME*(-1) && time_val!= 0) // Within a recording window
                              {
                                    // Record into vector here

                                    if (channel==2)
                                    {
                                          pairs12++;

                                          h12->push_back(channel1time);
                                          h12->push_back(time_diff);
                                          //d12.push_back(time_diff);
                                    }else if (channel==3)
                                    {
                                          pairs13++;

                                          
                                          h13->push_back(channel1time);
                                          h13->push_back(time_diff);
                                          //d13.push_back(time_diff);
                                    }else if (channel==4)
                                    {
                                          pairs14++;
                                          h14->push_back(channel1time);
                                          h14->push_back(time_diff);
                                          //d14.push_back(time_diff);
                                    }
                                    index[channel-1]++;
                              }else if (time_diff>WINDOW_TIME)
                              {
                                    index[channel-1]++;
                              }



                        }while (time_diff>WINDOW_TIME*(-1) && index[channel-1]<timestamps[channel-1].size());
                  
                  }

            }
            
            index[0]++;

      }







      index[0]=1;
      index[1]=1;
      index[2]=1;
      index[3]=1;

      // Pass 2: pairs 2-3,2-4

      while (index[1]<timestamps[1].size())
      {
            channel2time=timestamps[1][index[1]];

            for (int channel=3;channel<=4;channel++)
            {
                  if (index[channel-1]<timestamps[channel-1].size())
                  {
                        do {
                              time_val=timestamps[channel-1][index[channel-1]];
                              time_diff=channel2time-time_val;

                              if (time_diff<=WINDOW_TIME && time_diff>=WINDOW_TIME*(-1)) // Within a recording window
                              {
                                    // Record into vector here

                                    if (channel==3)
                                    {
                                          pairs23++;

                                          h23->push_back(channel2time);
                                          h23->push_back(time_diff);
                                          //d23.push_back(time_diff);
                                    }else if (channel==4)
                                    {
                                          pairs24++;
                                          h24->push_back(channel2time);
                                          h24->push_back(time_diff);
                                          //d24.push_back(time_diff);
                                    }
                                    index[channel-1]++;
                              }else if (time_diff>WINDOW_TIME)
                              {
                                    index[channel-1]++;
                              }



                        }while (time_diff>WINDOW_TIME*(-1) && index[channel-1]<timestamps[channel-1].size());
                  
                  }

            }
            
            index[1]++;

      }  

      index[0]=1;
      index[1]=1;
      index[2]=1;
      index[3]=1;


      //Pass 3 : pairs 3-4

      while (index[2]<timestamps[2].size())
      {
            channel3time=timestamps[2][index[2]];

            int channel=4;

            if (index[channel-1]<timestamps[channel-1].size())
            {
                  do {
                        time_val=timestamps[channel-1][index[channel-1]];
                        time_diff=channel3time-time_val;

                        if (time_diff<=WINDOW_TIME && time_diff>=WINDOW_TIME*(-1)) // Within a recording window
                        {
                              // Record into vector here

                              if (channel==4)
                              {
                                    pairs34++;
                                    h34->push_back(channel3time);
                                    h34->push_back(time_diff);
                                    //d34.push_back(time_diff);
                              }
                              index[channel-1]++;
                        }else if (time_diff>WINDOW_TIME)
                        {
                              index[channel-1]++;
                        }



                  }while (time_diff>WINDOW_TIME*(-1) && index[channel-1]<timestamps[channel-1].size());
            
            }

            
            index[2]++;

      } 

      return 0;

}









int main(int argc, char * argv[])
{
      
      std::cout<< argc<<std::endl;
      if (argc != 4 )
      {
            std::cout<<"USAGE: ./timestamp_parsing FILE_PREFIX FILE_MIN FILE_MAX"<<std::endl;
            return 0xBADBEEF;
      }

      std::string file_prefix(argv[1]);

      int file_min=std::stoi(argv[2]);
      int file_max=std::stoi(argv[3]);

/*
      std::string file_prefix(PREFIX);

      int file_min=FILE_MIN;
      int file_max=FILE_MAX;*/



      vector <long long> *timestamps;
      vector <long long> * h12,*h13,*h14,*h23,*h24,*h34;
      h12=new vector <long long>;
      h13=new vector <long long>;
      h14=new vector <long long>;
      h23=new vector <long long>;
      h24=new vector <long long>;
      h34=new vector <long long>;




      for (int file_num=file_min;file_num<=file_max;file_num++)

      {
            std::cout<<'\t'<<"Processing file "<< file_prefix << file_num<<".bin"<<std::endl;
            timestamps=new vector<long long> [4];
            load_bin(file_prefix+std::to_string(file_num)+".bin",timestamps);
            std::cout<< '\t'<<"Finding pairs in timestamps for file "<< file_prefix << file_num<<".bin"<<std::endl<<std::endl;
            recordPairs(timestamps,h12,h13,h14,h23,h24,h34);


            delete[] timestamps;


      }
      //std::cout<< (*h12)[1234*2]<<std::endl;

      
      


      std::cout<<"Ch1-Ch2="<<(*h12).size()/2<<" Ch1-Ch3="<<(*h13).size()/2<<" Ch1-Ch4="<<(*h14).size()/2<<" Ch2-Ch3="<<(*h23).size()/2<<" Ch2-Ch4="<<(*h24).size()/2<<" Ch3-Ch4="<<(*h34).size()/2<<" pair counts total"<<std::endl;

      std::array<long unsigned, 2> shape {{(*h12).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs1_2.npy",false,shape.size(),shape.data(),(*h12));
      usleep(1000);
      shape= {{(*h13).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs1_3.npy",false,shape.size(),shape.data(),(*h13));
      usleep(1000);
      shape ={{(*h14).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs1_4.npy",false,shape.size(),shape.data(),(*h14));

      usleep(1000);
      shape ={{(*h23).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs2_3.npy",false,shape.size(),shape.data(),(*h23));
      usleep(1000);

      shape ={{(*h24).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs2_4.npy",false,shape.size(),shape.data(),(*h24));
      usleep(1000);

      shape ={{(*h34).size()/2,2}};
      npy::SaveArrayAsNumpy("pairs3_4.npy",false,shape.size(),shape.data(),(*h34));
      usleep(1000);
      //std::cout<<timestamps[0][1]<<" "<< timestamps[2][1]<<" "<< timestamps[2][1]<<" "<< timestamps[3][1]<<std::endl;


      
}
