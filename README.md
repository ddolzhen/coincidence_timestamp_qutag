# coincidence_timestamp_qutag

Run with 
./timestamp_parsing FILE_PREFIX FILE_MIN FILE MAX

Example 
./timestamp_parsing ArLamp_30min_sweep_2Lamps_ 0 5


Produces 6 2-column .npy  files for pairs (each file is a channel pair) and 1 5-column .npy file for single rates



# Program flow

### Loading data 
During the execution of the load_bin() function, the program will iteratively proceed through the .dat file, decoding the timestamps and channel values from binary and pushing them into their respective channel vectors. There are four vectors for each of the channels respectively. While the program is iterating through the file, each entry will also be checked against the previous entry in the same channel. If the two entries are too close, the latter timestamp will be discarded and deemed as an afterpulse. Afterpulse window width is defined in picoseconds in the beginning of the .cpp file (#define AP_TIME 30e3 for 30ns) and can be adjusted before compilation.

### Getting pairs
After the data is loaded, the program proceeds to find pairs of timestamp entries between separate channels. This happens in 3 loops

##### Loop 1
Iterate through timestamp_ch1 and look for the nearest entries between ch1 and ch2 , ch3 , ch4

##### Loop 2
Iterate through timestamp_ch2 and look for the nearest entries between ch2 and ch3 , ch4

##### Loop 3
Iterate through timestamp_ch3 and look for the nearest entries between ch3 and ch4

If timestamp delays between two different channels are found to be within the WINDOW_TIME variable (#define WINDOW_TIME 20e3 in the beginning of the file), this pair of timestamps is recorded as 2 variables : timestamp of the lower-valued channel and the time delay to the higher-valued channel. Note that time delay can be either positive or negative.

### Singles-rates
During binTimeStamps(), the program iterates through the timestamps on all channels while binning them into 1s (by default) wide chunks. This produces data for creating histograms with 1s binning. Bin width can be adjusted

# Produced Files
The program produces 6 pair files (pairs1__2.npy,pairs1__3.npy,pairs1__4.npy,pairs2__3.npy,pairs2__4.npy,pairs3__4.npy) and 1 single rates binned histogram file (single_channel_frequencies.npy). .npy is the format prefered by NumPy for data storage, so one could use `data = np.load('data.npy')` to load it in Python relatively quickly

##### Pairs files
2 columns. Column 1 is timestamp of the first channel in the two-channel pair. Column 2 is the time-delay to the second labeled channel. Both measured in picoseconds. Data is time ordered
##### Singles file
5 columns. Column 1 is the left edge of the bin, Columns 2-3-4-5 are frequencies of channels 1,2,3,4 respectively within this time bin. Data is time ordered and populates all time bins
