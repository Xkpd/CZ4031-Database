#include "b_plus_tree.h"
#include "memory_pool.h"

void *startAddress = NULL;
unsigned int blockSize;

void experiment2(BPTree root);
void experiment3(BPTree root, int numVotes);
void experiment4(BPTree root, int lowerBound, int upperBound);
void experiment5(BPTree root, int numVotes);

int main(){   
    // Set parameters here
    const size_t MEMORY_STORAGE_SIZE = 500000000; // IN BYTES
    // const size_t MEMORY_STORAGE_SIZE = 5000; // IN BYTES
    const size_t BLOCK_SIZE_OPTION_1 = 500; // IN BYTES
    const size_t BLOCK_SIZE_OPTION_2 = 200; // IN BYTES
    const size_t NO_OF_RECORDS_TO_READ = 1000; // -1 to indicate read all


    cout << "\n---------------------------CZ4031 Database Project 1----------------------------\n";
    cout << "By: Kong Tat, Lyndon, Qi Wei, Ryan, Xing Kun\n";
    cout << "Objective: To simulate the storage on memory and use B+ tree as index for records.\n";
    cout << "----------------------------------------------------------------------------------\n";

    std::string dataset = "data.tsv";
    std::ifstream data_file(dataset);
    int choice = 0;
    std::cout << "\nPlease select block size from the following options:\n";
    std::cout << "1) 200B\n";
    std::cout << "2) 500B\n";
    
    std::cin >> choice; 
    if (int(choice) == 1){
        blockSize = BLOCK_SIZE_OPTION_1;
    }
    else if (int(choice) == 2){
        blockSize = BLOCK_SIZE_OPTION_2;
    }
    else{
        std::cout << "Invalid option, re-run the program again!\n";
        return 0;
    }

    std::cout << "Your dataset is processing...\n";

    memoryPool memory_pool{MEMORY_STORAGE_SIZE, blockSize};    
    std::vector<std::tuple<void *, unsigned int>> data;
    std::string data_info;
    Record record;
    std::string temp;
    bool flag = true;

    if (data_file.is_open())
    {
        // Iterate the data_file and store the individual bytes
        while (std::getline(data_file, data_info))
        {
            if (flag)
            {
                flag = !flag;
                continue;
            }

            //Unable to write directly to disk as tconst is a string
            //Copy the string data_info into record.tconst
            std::strcpy(record.tconst, data_info.substr(0, data_info.find('\t')).c_str());
            std::stringstream lineStream(data_info);
            std::getline(lineStream, temp, '\t'); //terminates upon 'tab' key button
            lineStream  >> record.averageRating >> record.numVotes; //Write the averageRating and numVotes into disk 

            //The void pointer contains the address of the blocks 
            std::tuple<void *, unsigned int> data_record = memory_pool.insertRecord(sizeof(record));
            data.push_back(data_record);

            void *rcdAdr = (unsigned char *)get<0>(data_record) + get<1>(data_record); //0 = block pointer; 1 = offset
            std::memcpy(rcdAdr, &record, sizeof(record)); //memcpy into the record address
        }

        std::cout << "\n===================================Experiment 1===================================\n";
        std::cout << "Number of blocks: " << memory_pool.getBlocksAssigned() << "\n";
        std::cout << "Size of database: " << memory_pool.getmemorySize()/1e6 << "MB\n";
        std::cout << "==================================================================================\n";
        data_file.close();
    }

    //For data vector iteration
    std::vector<std::tuple<void *, unsigned int>>::iterator iterator;

    //Initialising the B+ tree
    BPTree root;

    std::cout << "\nCreating the B+ tree with the given data...\n";

    for (iterator = data.begin(); iterator != data.end(); ++iterator)
    {
        void *blockAddress = get<0>(*iterator);
        unsigned int block_offset = get<1>(*iterator);

        if (startAddress == NULL)
        {
            startAddress = blockAddress;
        }

        void *recordAddress = (unsigned char *)blockAddress + block_offset;
        float num = (*(Record *)recordAddress).numVotes;
        keys_struct key;
        key.key_value = num;
        key.add_vect.push_back((unsigned char *)blockAddress + block_offset);
        root.insert(key);
    }

    std::cout << "B+ tree entry complete!\n";

    experiment2(root);
    experiment3(root, 500);
    experiment4(root, 30000, 40000);
    experiment5(root, 1000);

    return 0;
}

void experiment2(BPTree root)
{
    std::cout << "\n===================================Experiment 2===================================\n";

    std::cout << "Parameter n of B+ tree: " << root.getMax() << "\n";
    std::cout << "Number of nodes of the B+ tree: " << root.calculateNumNodes(root.getRoot()) << "\n";
    std::cout << "Height (Level) of the B+ tree: " << root.getHeight(root.getRoot()) << "\n";
    root.display(root.getRoot());
    std::cout << "==================================================================================\n";
}

void experiment3(BPTree root, int numVotes)
{
    std::cout << "\n===================================Experiment 3===================================\n";
    std::cout << "Retrieve records where 'numVotes' equal to " << numVotes << ": \n\n";
    root.findSingle(numVotes);
    std::cout << "==================================================================================\n";
}

void experiment4(BPTree root, int lowerBound, int upperBound)
{
    std::cout << "\n===================================Experiment 4===================================\n";
    std::cout << "Retrieve records where 'numVotes' is from "<< lowerBound << " to " << upperBound << ": \n\n";
    root.findRange(lowerBound, upperBound);
    std::cout << "==================================================================================\n";
}

void experiment5(BPTree root, int numVotes)
{
    std::cout << "\n===================================Experiment 5===================================\n";
    std::cout << "Deleting records where 'numVotes' equal to " << numVotes << "...\n";

    keys_struct key;
    key.key_value = numVotes;
    key.add_vect.push_back((unsigned char *)nullptr);
    int numNodeBeforeRemoval = root.calculateNumNodes(root.getRoot());
    int numNodeMerged = root.remove(key);
    int numNodeAfterRemoval = root.calculateNumNodes(root.getRoot());
    int numNodeRemoved = numNodeBeforeRemoval - numNodeAfterRemoval;

    std::cout << "Number of times that a node is deleted (or two nodes are merged) when updating B+ tree: " 
                << numNodeRemoved+numNodeMerged << "\n";
    std::cout << "Number of nodes of the updated B+ tree: " << root.calculateNumNodes(root.getRoot()) << "\n";
    std::cout << "Height (Level) of the updated B+ tree: " << root.getHeight(root.getRoot()) << "\n";
    root.display(root.getRoot());
    std::cout << "==================================================================================\n\n";
}