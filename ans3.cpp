#include<bits/stdc++.h>
using namespace std;

struct Inode {
    string name;
    int size;
    int blockpointers[8];
    int used=0;
};

struct Inode inodes[16];

int free_blocks[128];

int no_free_blocks = 127;
char data_block[128][1024];

void create_file(string filename, int size){
    if(no_free_blocks<size){
        cout<<"Error not enough space in the disk"<<endl;
        return ;
    }
    no_free_blocks -= size;
    for(int i=0;i<16;i++){
        if(inodes[i].name==filename && inodes[i].used==1){
            cout<<"File with this name is already present"<<endl;
            return ;
        }
    }
    for(int i=0;i<16;i++){
        if(inodes[i].used==0){
            inodes[i].used=1;
            inodes[i].size = size;
            inodes[i].name = filename;
            int block_found=0,j=1;
            while(block_found<size && j<128){
                while(j<128){
                    if(free_blocks[j]==0){
                        free_blocks[j]=1;
                        inodes[i].blockpointers[block_found]=j;
                        block_found+=1;
                        j+=1;
                        break;
                    }
                    j+=1;
                }
            }
            break;
        }
    }
    return ;
}
void delete_file(string filename){
    int i=0;
    for(i=0;i<16;i++){
        if(inodes[i].used==1 && inodes[i].name == filename){
            inodes[i].used=0;
            no_free_blocks+=inodes[i].size;
            int j=0;
            while(j<inodes[i].size){
                free_blocks[inodes[i].blockpointers[j]] = 0;
                j++;
            }
            inodes[i].size=0;
            break;
        }
    }
    if(i==16){
        cout<<"No file with this name found"<<endl;
    }
    else{
        cout<<"File deleted successfully"<<endl;
    }
    return ;
}

void ls_file(){
    cout<<"List of files: "<<endl;
    for(int i=0;i<16;i++){
        if(inodes[i].used==1){
            cout<<inodes[i].name<<" and size is "<<inodes[i].size<<" blocks"<<endl;
        }
    }
    return ;
}

char* read_file(string filename, int block_number, char* buffer){
    int i=0;
    int block;
    for(i=0;i<16;i++){
        if(inodes[i].used==1 && inodes[i].name == filename){
            block = inodes[i].blockpointers[block_number];
            buffer = data_block[block] ;
            break;
        }
    }
    return buffer;
}

void write_file(string filename, int block_number, char* buffer){
    int i=0;
    int block;
    // cout<<block_number<<endl;
    for(i=0;i<16;i++){
        if(inodes[i].used==1 && inodes[i].name == filename){
            // cout<<block_number<<endl;
            // // if(block_number>=inodes[i].size){
            // //     cout<<"Error block number you are trying to write is out of bound"<<endl;
            // //     return;
            // }
            block = inodes[i].blockpointers[block_number];
            for(int j=0;j<strlen(buffer);j++){
                data_block[block][j] = buffer[j];
            }
            
            break;
        }
    }
    return ;
}

int main(){
    int x,exit=0,size;
    string filename;
    char* buffer;
    for(int i=0;i<128;i++){
        free_blocks[i]=0;
    }
    free_blocks[0]=1;
    while(1){
        cout<<"Enter the choice number: "<<endl;
        cout<<"1. Create a new file"<<endl;
        cout<<"2. Delete a file"<<endl;
        cout<<"3. List of files in the system"<<endl;
        cout<<"4. Read a block from file"<<endl;
        cout<<"5. Write a block into file"<<endl;
        cout<<"6. Exit"<<endl;
        cin>>x;
        switch (x)
        {
            case 1: cout<<"Give filename to be created(should be of length less than 9): ";
                    cin>>filename;
                    cout<<endl;
                    cout<<"Give the number of blocks needed in file(should be less than 9): ";
                    cin>>size;
                    cout<<endl;
                    if(size>8 || filename.length()>8){
                        cout<<"Error in filename or size. Either one of them is greater than 8."<<endl;
                        break;
                    }
                    create_file(filename,size);
                    break;
            case 2: cout<<"Give filename to be deleted(should be of length less than 9): ";
                    cin>>filename;
                    cout<<endl;
                    if(filename.length()>8){
                        cout<<"Error in filename. Filename length is greater than 8."<<endl;
                        break;
                    }
                    delete_file(filename);
                    break;
            case 3: ls_file();
                    break;
            case 4: cout<<"Give filename to be read(should be of length less than 9): ";
                    cin>>filename;
                    cout<<endl;
                    cout<<"Give the block number to be read in file(should be less than 8): ";
                    cin>>size;
                    cout<<endl;
                    if(size>7 || filename.length()>8){
                        if(size>7){
                            cout<<"The block number is invalid"<<endl;
                        }
                        if(filename.length()>8){
                            cout<<"Error in filename. Filename length is greater than 8."<<endl;
                        }
                        break;
                    }
                    buffer = read_file(filename,size,buffer);
                    cout<<"Buffer read is: "<<buffer<<endl;
                    break;
            case 5: cout<<"Give filename to be write(should be of length less than 9): ";
                    cin>>filename;
                    cout<<endl;
                    cout<<"Give the block number to be written in file(should be less than 8): ";
                    cin>>size;
                    cout<<endl;
                    cout<<"Give the buffer data to be read in file(should be of length less than 1024 chars): ";
                    cin>>buffer;
                    cout<<endl;
                    if(size>7 || filename.length()>8 || strlen(buffer)>1024){
                        if(size>7){
                            cout<<"The block number is invalid"<<endl;
                        }
                        if(filename.length()>8){
                            cout<<"Error in filename. Filename length is greater than 8."<<endl;
                        }
                        if(strlen(buffer)>1024){
                            cout<<"Error in buffer. Buffer length is greater than 1024."<<endl;
                        }
                        break;
                    }
                    // cout<<strlen(buffer)<<endl;
                    write_file(filename, size, buffer);
                    break;
            case 6: exit=1;
                    break;
            default: printf("Choice other than 1 to 6\n");
                    exit=1; 
                    break;
        }
        if(exit==1){
            break;
        }
    }
    return 0;
}