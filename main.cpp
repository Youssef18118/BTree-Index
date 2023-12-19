#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

void CreateIndexFileFile (char* filename, int numberOfRecords, int ordr);
int InsertNewRecordAtIndex (char* filename, int RecordID, int Reference); //insert function should return -1 if there is no place to insert the record or the index of the node where the new record is inserted if the record was inserted successfully.
void DeleteRecordFromIndex (char* filename, int RecordID); 
void DisplayIndexFileContent (char* filename); // this method should display content of the file, each node in a line.
int SearchARecord (char* filename, int RecordID); // this method should return -1 if the record doesn’t exist in the index or the reference value to the data file if the record exist on the index

class B_Index {
    private:
        vector <int> visited_records;
        int no_records = 10; // 10 can be removed
        int order = 5; // 5 can be removed
 
    public:
        void CreateIndexFileFile(char* filename, int numberOfRecords, int ordr) {
            no_records = numberOfRecords;
            order = ordr;
            ofstream file(filename);

            int counter = 1;
            for (int i = 0; i < numberOfRecords; ++i) {
               file << -1;
               for (int j = 0; j < (2*ordr) ; j++)
               {
                    
                    if(j == 0){
                        if (counter == numberOfRecords){
                            counter = -1;
                        }
                        file << ' ' << counter;
                    }else{
                        file << ' ' << -1;
                    }
                    
                    
               }
               file << '\n';
               counter++;
        
                
            }

            file.close();
        }

        void DisplayIndexFileContent(char* filename) {
            ifstream file(filename);

            if (!file.is_open()) {
                cerr << "Error: Unable to open file " << filename << endl;
                return;
            }

            for (int i = 0; i < no_records; i++)
            {
                string token;
                getline(file, token, '\n');

                cout << token << endl;

            }
            


            file.close();

        }

        int SearchARecord (char* filename, int RecordID){
            ifstream file(filename);
            int record = 1;
            int target = 0;
            int key_counter = 1;

            // ignore first line
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            begin:
            string leaf_indicator, key, index;
            getline(file, leaf_indicator, ' ');  // Storing the leaf indicator 

            start:
            getline(file, key, ' '); // Storing the key  
            getline(file, index, ' '); // Storing the index 


            // if next key is -1 (empty) or key counter is bigger than order of the node
            if (stoi(key) == -1 || key_counter > order){
                cout << "Record not found" << endl;
                return -1;
            } 
            // if RecordID is less or equal to key
            else if(RecordID <= stoi(key)){
                if(stoi(key) == RecordID){  // if RecordID is equal to key
                            
                    if(stoi(leaf_indicator) == 0){  // checking if the Record is leaf Node using leaf indicator 
                        return stoi(index);
                    }else{    // if Record is not leaf Node
                        target = stoi(index); // String Index
                        if(target > no_records){ // index is bigger than number of records (intitialized with bigger number) return index
                            return target;
                        }
                        file.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of record
                        record++;
                        cout << "Record equality: " << record << endl;
                    }
                        
                }else{ // if RecordID is less than key
                    target = stoi(index); // String Index
                    if(target > no_records){ // index is bigger than number of records, return -1
                        cout << "Record not found" << endl;
                        return -1;
                    }
                    file.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of record
                    record++;
                    cout << "Record less than: " << record << endl;
                }
            }else{ // if RecordID is greater than key
                key_counter++; // increment key counter
                goto start; // begin reading next key and index
            }

            // Looping till reaching the last record
            while(record < target){
                // till reaching specified record
                file.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of record
                record++;
                cout << "Record while: " << record << endl;
            }
            
            // now we are in the last record
            getline(file, leaf_indicator, ' ');

            // if we are not in the leaf node go back to begining to check earlier cases
            if(stoi(leaf_indicator) == 1){ 
                goto begin;
            }
            
            last:
            key_counter = 1;

            getline(file, key, ' ');
            getline(file, index, ' ');

            if(stoi(key) == RecordID){
                return stoi(index);
            }else if (RecordID < stoi(key)){ // Record id is less than a key in last record so it is not found
                cout << "Record not found" << endl;
                return -1;
            }else{ // Recod id is bigger than current key so look at next key
                key_counter++;
                goto last;
            }

        }



};

int main() {
    char filename[] = "btree.txt";
    int numberOfRecords = 10;
    int m = 5;

    B_Index b_index;
    // b_index.CreateIndexFileFile(filename, numberOfRecords, m);

    b_index.DisplayIndexFileContent(filename);
    // cout << endl;
    cout << b_index.SearchARecord(filename, 24) << endl;

    cout << "B-tree index file created successfully." << endl;

    return 0;
}