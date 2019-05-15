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
    vector <int> createdCheck;
    vector <swapMemory> swap;
    
    int idHolder=0;
    int pageHolder=0;
    char actHolder = 'Z';
    int entry = 0;
    int max = 20;
    
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
    
    cout << "\t\t FIFO\n";
    
    for (int i  = 0; i < entry; i++ ){
//        cout<< "\tPID: "<< process_ID[i] << " Act: " << action[i]<< " Page: " << page[i]<< " i:"<<i <<endl;// debug print
    }
    
    int PhysSize = 0;
    int SwapSize = 0;
    int CreatedSize = 0;
    int track = 0;
//    cout << "PS was: "<<PhysSize<<endl;
//    cout << "SS was: "<<SwapSize<<endl;
//    cout << "CSwas: "<<CreatedSize<<endl;
    
    
    //FIFO ... Still working on this but kept it in here so you can see how to use the struct as a vector
    for (int i =0; i < entry; i++){
        //        cout << "i:" << i <<endl;
        if (action[i] == 'C'){
//            cout << "need to add: " << process_ID[i]<< " to the vector\n";
            createdCheck.push_back(process_ID[i]);
            CreatedSize = createdCheck.size();
        }
        else if (action[i] == 'A'){
            for (int k =0; k < CreatedSize; k++){
                if(process_ID[i] == createdCheck[k]){
                    if (PhysSize < max){
                        physcial.push_back(PhysicalMemory());
                        physcial[PhysSize].owner = process_ID[i];
                        physcial[PhysSize].virAddress = page[i];
                        PhysSize = physcial.size();
                    }
                    else if (PhysSize >= max){
                        swap.push_back(swapMemory());
                        swap[SwapSize].owner = physcial[track].owner;
                        swap[SwapSize].virAddress = physcial[track].virAddress;
                        swap[SwapSize].accessed = physcial[track].accessed;
                        swap[SwapSize].dirty = physcial[track].dirty;
                        SwapSize = swap.size();

                        physcial[track].owner = process_ID[i];
                        physcial[track].virAddress = page[i];
                        
//                        cout << "Need to swap No room for PID: " << process_ID[i] << " Page: "<< page[i]<<endl; // debug print
                        if (track > max-2){
                            track =0;
                        }
                        else{
                            track++;
                        }
                    }
//                    else if (PhysSize >= max){
//                        cout << "Need to swap No room for PID: " << process_ID[i] << " Page: "<< page[i]<<endl; // debug print
//
//                        swap.push_back(swapMemory());
//                        swap[SwapSize].owner = process_ID[i];
//                        swap[SwapSize].virAddress = page[i];
//                        SwapSize = swap.size();
//                    }
                }
            }
        }
        else if (action[i] == 'F'){
            for (int  k = PhysSize; k >= 0; k--){
                if(process_ID[i] == physcial[k].owner && page[i] == physcial[k].virAddress)
                {
//                    cout << "PHYS Need to check and free PID: "<< process_ID[i] << " Page: "<< page[i] << " Found at: "<< k<<endl;// debug print
                    physcial.erase(physcial.begin() + k);
                    PhysSize = physcial.size();
                }
            }
            for (int j = SwapSize-1; j > 0; j--){
                if (process_ID[i] == swap[j].owner && page[i] == swap[j].virAddress){
//                    cout << "Need to terminate: "<< physcial[j].owner << ", "<< page[j]<< " T @: "<< j <<endl;// debug print
                    swap.erase(swap.begin()+j);
                    SwapSize = swap.size();
                }
            }
            
        }
        else if (action[i] == 'T'){
//            cout << "Need to terminate all with PID: "<< process_ID[i] <<endl;// debug print
            for (int  k = PhysSize; k >= 0; k--){
                //            cout << "comparing PID[i]: " << process_ID[i] << " to: " << physcial[k].owner<<endl;
                if(process_ID[i] == physcial[k].owner)
                {
//                    cout << "Need to terminate: "<< physcial[k].owner << ", "<< page[k]<< " T @: "<< k <<endl;// debug print
                    physcial.erase(physcial.begin() + k);
                    PhysSize = physcial.size();
                    //                    cout << "updating PS to: "<<PhysSize<<endl;
                }
            }
            for (int j = SwapSize; j >= 0; j--){
                if (process_ID[i] == swap[j].owner){
//                    cout << "Need to terminate: "<< physcial[j].owner << ", "<< page[j]<< " T @: "<< j <<endl;// debug print
                    swap.erase(swap.begin()+j);
                    SwapSize = swap.size();
                }
            }
            for (int s =0; s < CreatedSize; s++){
                if(process_ID[i] == createdCheck[s]){
                    createdCheck.erase(createdCheck.begin()+s);
                    CreatedSize = createdCheck.size();
                }
            }
            
        }
        else if (action[i] == 'R'){
            for (int  k = 0; k <= PhysSize; k++){
                if(process_ID[i] == physcial[k].owner && page[i] == physcial[k].virAddress){
//                    cout << "Need to Read: "<< physcial[k].owner << ", "<< physcial[k].virAddress<<endl;// debug print
                    physcial[k].accessed =true;
                }
            }
        }
        else if (action[i] == 'W'){
            for (int  k = 0; k <= PhysSize; k++){
                if(process_ID[i] == physcial[k].owner && page[i] == physcial[k].virAddress){
//                    cout << "Need to Write: "<< physcial[k].owner << ", "<< physcial[k].virAddress<<endl;// debug print
                    physcial[k].dirty =true;
                }
            }
        }
    }
//    cout << "PS now: "<<PhysSize<<endl;
    
    cout << "\tPhysical Memory\n";
    for(int j =0; j <PhysSize; j ++){
        cout <<"Process: "<< physcial[j].owner<<  " PhysicalAdd: "<< j<<" Virtual: " << physcial[j].virAddress<<endl;// debug print
    }
    cout << "\tSwap Memory\n";
    
    for(int j =0; j <SwapSize; j ++){
        cout <<"Process: "<< swap[j].owner<<  " SwapAdd: "<< j << " Virtual: "<< swap[j].virAddress<< endl;// debug print
    }
    process_ID.clear();
    action.clear();
    page.clear();
    physcial.clear();
    createdCheck.clear();
    swap.clear();
    
    //ADD NEW CODE HERE
    
    return 0;
}
//" VA: " << physcial[j].virAddress
//<< " VA: " << swap[j].virAddress


