#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    vector <int> process_ID;
    vector <char> action;
    vector <int> page;
    vector <int> addPhysicalMem;
    vector <int> addPhysicalProc;
    vector <int> freePhysicalMem;
    vector <int> freePhysicalProc;
    
    int idHolder=0;
    int pageHolder=0;
    char actHolder = 'Z';
    int entry = 0; // num must start at 0
    int createCount = 0;
    int terminateCount = 0;
    int physPages [19] = {0};
    string line;
    ifstream file ("cat.txt");  // file containing numbers in 3 columns //replace cat.txt later
    //    cout << "original: " <<endl;
    
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
    cout << "Num is:"<< entry<<endl;
    for (int i  = 0; i < entry; i++ ){
        cout<< "PID: "<< process_ID[i] << " Act: " << action[i]<< " Page: " << page[i]<< " i:"<<i <<endl;
    }
    int addSize = 0;
    int freeSize = 0;
    //FIFO
    
    return 0;
}



