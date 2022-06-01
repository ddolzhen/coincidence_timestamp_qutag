#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


#define NAME "ArLamp_30min_sweep_2Lamps_0.bin"
#define WINDOW_TIME 20e3 //Measured in picoseconds
#define AP_TIME 30e3 //Afterpulse time, also in ps

int main()
{





















      vector <long long> timestamps [4];


      long long time_val=0;
      short channel_val;


      timestamps[0].push_back(0);
      timestamps[1].push_back(0);
      timestamps[2].push_back(0);
      timestamps[3].push_back(0);

      ifstream in;
      in.open(NAME, std::ios::binary);
      in.seekg(40);


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
                  std::cout<<i<<std::endl;
            }
      }

      std::cout<< timestamps[0].size() << "  "<<timestamps[1].size() << "  "<<timestamps[2].size() << "  "<<timestamps[3].size() << "  "<<std::endl;
      std::cout << ap_count << " Afterpulses" << std::endl;



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

                              if (time_diff<=WINDOW_TIME && time_diff>=WINDOW_TIME*(-1)) // Within a recording window
                              {
                                    // Record into vector here

                                    if (channel==2)
                                    {
                                          pairs12++;
                                    }else if (channel==3)
                                    {
                                          pairs13++;
                                    }else if (channel==4)
                                    {
                                          pairs14++;
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
                                    }else if (channel==4)
                                    {
                                          pairs24++;
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

      std::cout<<"1-2 "<< pairs12<<"\n1-3 "<<pairs13<<"\n1-4 "<<pairs14<<"\n2-3 "<<pairs23<<"\n2-4 "<<pairs24<<"\n3-4 "<<pairs34<<std::endl;

      
}
