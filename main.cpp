#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct PhysicalMemory{
    int owner;
    int virAddress;
    bool accessed;
    bool dirty;
};

struct swapMemory{
    int owner;
    int virAddress;
    bool accessed;
    bool dirty;
};

int main()
{
    vector <int> process_ID;
    vector <char> action;
    vector <int> page;
    vector <PhysicalMemory> physcial;
    vector <int> test;
    vector <swapMemory> swap;
    
    int idHolder=0;
    int pageHolder=0;
    char actHolder = 'Z';
    int entry = 0;
    
    int createCount = 0;
    int terminateCount = 0;
    
    string line;
    ifstream file ("cat.txt");  // file containing numbers in 3 columns //replace cat.txt later
    //reading and stroing file data
    if(file.is_open()){ // checks to see if file opened
        
        while(getline(file, line)){ // reads file to end of *file*, not line
            stringstream ss(line);
            idHolder=0;
            pageHolder=0;
            actHolder = 'Z';
            ss>> idHolder >> actHolder >> pageHolder;
            process_ID.push_back(idHolder);
            action.push_back(actHolder);
            page.push_back(pageHolder);
            entry++;
        }
        file.close();
    }
    
    cout << "Num is:"<< entry<<endl<< "\t\tOriginal\n";
    
    for (int i  = 0; i < entry; i++ ){
        cout<< "\tPID: "<< process_ID[i] << " Act: " << action[i]<< " Page: " << page[i]<< " i:"<<i <<endl;// debug print
    }
    
    int PhysSize = 0;
    int SwapSize = 0;
    cout << "PS was: "<<PhysSize<<endl;
    cout << "SS was: "<<SwapSize<<endl;

    //FIFO ... Still working on this but kept it in here so you can see how to use the struct as a vector
    for (int i =0; i < entry; i++){
        if (PhysSize < 20){
            if (action[i] == 'A'){
                physcial.push_back(PhysicalMemory());
                physcial[PhysSize].owner = process_ID[i];
                physcial[PhysSize].virAddress = page[i];
                PhysSize = physcial.size();
            }
        }
        if (PhysSize >= 20){
            if (action[i] == 'A'){
                cout << "Need to swap No room for PID: " << process_ID[i] << " Page: "<< page[i]<<endl; // debug print
                swap.push_back(swapMemory());
                swap[SwapSize].owner = process_ID[i];
                swap[SwapSize].virAddress = page[i];
                SwapSize = swap.size();
            }
        }
        if (action[i] == 'F'){
            cout << "Need to check and free PID: "<< process_ID[i] << " Page: "<< page[i]<<endl;// debug print
            for (int  k = 0; k < PhysSize; k++){
                if(process_ID[i] == physcial[k].owner && page[i] == physcial[k].virAddress)
                {
                    cout << "Found at: "<< k<<endl;// debug print
                    physcial.erase(physcial.begin() + k);
                    PhysSize = physcial.size();
                }
            }
        }
    }
    cout << "PS now: "<<PhysSize<<endl;
    
    cout << "\tPhysical Memory\n";
    for(int j =0; j <PhysSize; j ++){
        cout <<"\tPM: "<< physcial[j].owner << " VA: " << physcial[j].virAddress<<  " at: "<<j  << endl;// debug print
    }
    cout << "\tSwap Memory\n";

    for(int j =0; j <SwapSize; j ++){
        cout <<"\tSM: "<< swap[j].owner << " VA: " << swap[j].virAddress<<  " at: "<<j  << endl;// debug print
    }

    return 0;
}
