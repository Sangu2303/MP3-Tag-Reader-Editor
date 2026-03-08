/*
Name : SANGAMESH ITIGATTI
Date : 19/08/2025
Description : MP3 Tag Reader and Editor for ID3v2
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "type.h"



char *strline="-------------------------------------------------------------------------";
// Buffer to store flags
char default_flag[3];
// Strings used for displaying edit details
char *edit_details="--------------------SELECTED EDIT DETAILS--------------------";
char *edit_opt="--------------SELECTED EDIT OPTION--------------------";

// Function to check type of operation: view, edit, help, unsupported
operationType check_operation_type(char *argv){
    // Check if the argument is "-v"(view) 
    if(!(strcmp(argv,"-v"))){
        return view;
    }
    // Check if the argument is "-e"(edit)
    else if(!(strcmp(argv,"-e"))){
        return edit;
    }
    else if (!(strcmp(argv,"--help"))){
        return help;
    }
    else{
        // Unsupported operation
        return unsupported;
    }
}
// Structure to store MP3 tag details
struct tag{
    char title[50];
    char artistname[50];
    char albumname[50];
    char year[50];
    char content[50];
    char comment[100];
    int comment_len;

    FILE *source_fptr; // Original file pointer
    char *filename;

    FILE *extra_fptr;   // Temporary file pointer
    char *extra_filename;

    char modfy_content[100]; // Content to modify
    char modfy_type[5];     // Type of modification (-t, -a, etc.)
    
};


// Function to open file in read mode
status open_file(struct tag *vw){
    vw->source_fptr=fopen(vw->filename,"r");
    if(vw->source_fptr==NULL){
        printf("\n----------ERROR MESSAGE----------\n");
        printf("unable to open %s File ",vw->filename);
        return failure;
    }
    return success;
}

// Function to check ID3 version of MP3 file
status check_version(struct tag *vw){
    char buffer[4];
    char ver[3];
    fread(buffer,sizeof(char),3,vw->source_fptr);
    buffer[3]='\0';
    //printf("%s\n",buffer);
    if(!(strcmp(buffer,"ID3"))){
        //printf("Version is  ID3\n"); // Must begin with "ID3"
    }
    else{
        printf("Please provide sample file in ID3 version\n");
        return failure;
    }
    fread(ver,sizeof(char),2,vw->source_fptr);
    ver[2]='\0';
    if(ver[0]==3){
       // printf("Version is ID3 v3\n");// Must be version ID3v2.3
    }
    else{
        
        printf("Please provide sample file in ID3 version\n");
        return failure;
    }
    return success;
}

// Function to convert big-endian to little-endian integer
int big_end_to_lit_end(unsigned char *ptr){
    int size_tlt = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
    return size_tlt;
}

// Function to read MP3 tag data (title, artist, album, year, content, comment)
status read_data(struct tag *vw){
    rewind(vw->source_fptr);
    fseek(vw->source_fptr, 10, SEEK_SET); // Skip header
    char buffer[5];
    char size[4];
    
    
    for(int i=0;i<=5;i++){
        fread(buffer,sizeof(char),4,vw->source_fptr);
        buffer[4]='\0';

        // Read Title frame
        if(!(strcmp(buffer,"TIT2"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
            }
            string[len-1]='\0';
            strcpy(vw->title,string);
            //printf("%s\n",vw->title);
        }

         // Read Artist frame
        if(!(strcmp(buffer,"TPE1"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
            }
            string[len-1]='\0';
            strcpy(vw->artistname,string);
            //printf("%s\n",vw->artistname);
        }

        // Read Album frame
        if(!(strcmp(buffer,"TALB"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
            }
            string[len-1]='\0';
            strcpy(vw->albumname,string);
           // printf("%s\n",vw->albumname);
        }

        // Read Year frame
        if(!(strcmp(buffer,"TYER"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
            }
            string[len-1]='\0';
            strcpy(vw->year,string);
           // printf("%s\n",vw->year);
        }

        // Read Content frame
        if(!(strcmp(buffer,"TCON"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
            }
            string[len-1]='\0';
            
            strcpy(vw->content,string);
            //printf("%s\n",vw->content);
        }

        // Read Comment frame
        if(!(strcmp(buffer,"COMM"))){
            unsigned char size[4];
            fread(size, sizeof(char), 4, vw->source_fptr);  
            int len=big_end_to_lit_end(size);
            //printf("%d\n", len);

            fseek(vw->source_fptr,3,SEEK_CUR);

            char string[len];
            for(int i=0;i<len-1;i++){
                fread(&string[i],sizeof(char),1,vw->source_fptr);
                vw->comment[i]=string[i];
            }
            string[len-1]='\0';
            vw->comment_len=len-1;
            
            // for(int i=0;i<len-1;i++){
            //     printf("%c",vw->comment[i]);
            // }
            printf("\n");
            
        }
        

    }
    
    return success;
}

// Function to display read details
status Display(struct tag *vw){
    printf("\n\n ---------------SELECTED VIEW DETAILS---------------\n\n");
    printf("%s\n",strline);
    printf("\t\t MP3 TAG READERD AND EDITOR FOR ID3v2\n");
    printf("%s\n",strline);
    printf("TITLE               :\t%s\n",vw->title);
    printf("ARTIST              :\t%s\n",vw->artistname);
    printf("ALBUM               :\t%s\n",vw->albumname);
    printf("YEAR                :\t%s\n",vw->year);
    printf("CONTENT             :\t%s\n",vw->content);
    printf("COMMENT             :\t");
    for(int i=0;i<vw->comment_len;i++){
        printf("%c",vw->comment[i]);
    }
    printf("\n");
    printf("%s\n",strline);
    printf("\n -------------------DETAILS DISPLAYED SUCCESSFULLY------------------------\n");

    return success;
}

// Function to view details (open -> check version -> read -> display)
status Detailsview(struct tag *vw){
    if(open_file(vw)==failure){
        printf("Failure in opening file\n");
        return failure;
    }
    else{
        //printf("File opened successfully\n");
    }
    if(check_version(vw)==failure){
        printf("Failure in version check\n");
        return failure;
    }
    else{
        //printf("version checked successfully\n");
    }
    if(read_data(vw)==failure){
        printf("Failure in reading data\n");
        return failure;
    }
    else{
        //printf("reading data successfully\n");
    }
    if(Display(vw)==failure){
        printf("Failure in Displaying\n");
        return failure;
    }
    else{
        //printf("successfully Displayed\n");
    }

    return success;
}


// Function to open original file (read mode) and create a temp file (write mode)
status open_files(struct tag *edt){
    edt->source_fptr=fopen(edt->filename,"r");
    if(edt->source_fptr==NULL){
        printf("\n----------ERROR MESSAGE----------\n");
        printf("unable to open %s File ",edt->filename);
        return failure;
    }
    edt->extra_filename="extra_file.mp3"; // temporary file
    edt->extra_fptr=fopen("extra_file.mp3","w");
    if(edt->extra_fptr==NULL){
        printf("\n----------ERROR MESSAGE----------\n");
        printf("unable to open %s File ",edt->extra_filename);
        return failure;
    }
    return success;
}
// Function to copy MP3 header from original file to temp file
status copy_header(struct tag *edt){
    rewind(edt->source_fptr);
    rewind(edt->extra_fptr);
    char buffer[10];
    fread(buffer,sizeof(char),10,edt->source_fptr); // Read first 10 bytes
    fwrite(buffer,sizeof(char),10,edt->extra_fptr);// Write to temp file
    return success;
}

// Function to convert integer to 4-byte BigEndian (ID3 format)
void int_to_BigEndian(unsigned int value, unsigned char bytes[]) {
    bytes[0] = (value >> 21) & 0x7F;  
    bytes[1] = (value >> 14) & 0x7F;
    bytes[2] = (value >> 7) & 0x7F;
    bytes[3] = value & 0x7F;          
}

// Function to perform editing operation depending on flag (-t/-a/-A/-y/-m/-c)
status operation(struct tag *edt){
    // ---------------- Change Title ----------------
    if(!(strcmp(edt->modfy_type,"-t"))){
        printf("title\n");
        char buffer[5];
        unsigned char bytes[4];
         fread(buffer,sizeof(char),4,edt->source_fptr); // Read frame name
         buffer[4]='\0';
         if(!(strcmp(buffer,"TIT2"))){ // If title frame
            fwrite(buffer,sizeof(char),4,edt->extra_fptr);

            int new_len=strlen(edt->modfy_content)+1; // New length
            
            int_to_BigEndian(new_len,bytes);
            
            fwrite(bytes,sizeof(char),4,edt->extra_fptr);
             unsigned char size[4];
             fread(size, sizeof(char), 4, edt->source_fptr);  // Read old size
             int len=big_end_to_lit_end(size);

            
            fread(default_flag,sizeof(char),3,edt->source_fptr); 
            fwrite(default_flag,sizeof(char),3,edt->extra_fptr); 

            char string[len];
                fread(string,sizeof(char),len-1,edt->source_fptr);// Skip old data
                fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr); // Write new data
            }
            // Copy remaining file content
            char ch;
            while(fread(&ch,1,1,edt->source_fptr)>0){
                fwrite(&ch,1,1,edt->extra_fptr);
            }
            printf("%s\n",edit_details);
            printf("%s\n",edit_opt);
            printf("---------------------CHANGE THE TITLE--------------------\n");
            printf("TITLE :   %s\n\n",edt->modfy_content);
    }

// ---------------- Change Artist ----------------
    else if(!(strcmp(edt->modfy_type,"-a"))){
        printf("Artist name\n");
        char buffer[5];
        unsigned char bytes[4];
         fread(buffer,sizeof(char),4,edt->source_fptr);
         buffer[4]='\0';
         if(!(strcmp(buffer,"TIT2"))){
            fwrite(buffer,sizeof(char),4,edt->extra_fptr);

            fread(bytes,sizeof(char),4,edt->source_fptr);
            fwrite(bytes,sizeof(char),4,edt->extra_fptr);

             int len=big_end_to_lit_end(bytes);

            fread(default_flag,sizeof(char),3,edt->source_fptr);
            fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

            char string[len];
                fread(string,sizeof(char),len-1,edt->source_fptr);
                fwrite(string,sizeof(char),len-1,edt->extra_fptr);
            }
            
            fread(buffer,sizeof(char),4,edt->source_fptr);
            buffer[4]='\0';
            if(!(strcmp(buffer,"TPE1"))){
                
                fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                int new_len=strlen(edt->modfy_content)+1;
                
                int_to_BigEndian(new_len,bytes);
                
                fwrite(bytes,sizeof(char),4,edt->extra_fptr);
                unsigned char size[4];
                fread(size, sizeof(char), 4, edt->source_fptr);  
                int len=big_end_to_lit_end(size);

                
                fread(default_flag,sizeof(char),3,edt->source_fptr);
                fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                char string[len];
                    fread(string,sizeof(char),len-1,edt->source_fptr);
                    fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr);
                }
                char ch;
                while(fread(&ch,1,1,edt->source_fptr)>0){
                    fwrite(&ch,1,1,edt->extra_fptr);
                }
                printf("%s\n",edit_details);
                printf("%s\n",edit_opt);
                printf("---------------------CHANGE THE ARTIST--------------------\n");
                printf("ARTIST :   %s\n\n",edt->modfy_content);

            }
            else if(!(strcmp(edt->modfy_type,"-A"))){
                printf("in Album name\n");
                char buffer[5];
                unsigned char bytes[4];
                fread(buffer,sizeof(char),4,edt->source_fptr);
                buffer[4]='\0';
                if(!(strcmp(buffer,"TIT2"))){
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    fread(bytes,sizeof(char),4,edt->source_fptr);
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);

                    int len=big_end_to_lit_end(bytes);

                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(string,sizeof(char),len-1,edt->extra_fptr);
                }
                fread(buffer,sizeof(char),4,edt->source_fptr);
                buffer[4]='\0';
                if(!(strcmp(buffer,"TPE1"))){
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    fread(bytes,sizeof(char),4,edt->source_fptr);
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);

                    int len=big_end_to_lit_end(bytes);

                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(string,sizeof(char),len-1,edt->extra_fptr);
                }
                fread(buffer,sizeof(char),4,edt->source_fptr);
                buffer[4]='\0';
                if(!(strcmp(buffer,"TALB"))){
                
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    int new_len=strlen(edt->modfy_content)+1;
                    
                    int_to_BigEndian(new_len,bytes);
                    
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);
                    unsigned char size[4];
                    fread(size, sizeof(char), 4, edt->source_fptr);  
                    int len=big_end_to_lit_end(size);

                    
                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr);
                }
                char ch;
                while(fread(&ch,1,1,edt->source_fptr)>0){
                    fwrite(&ch,1,1,edt->extra_fptr);
                }
                printf("%s\n",edit_details);
                printf("%s\n",edit_opt);
                printf("---------------------CHANGE THE ALBUM--------------------\n");
                printf("ALBUM :    %s\n\n",edt->modfy_content);
            }
            else if(!(strcmp(edt->modfy_type,"-y"))){
                printf("In year\n");
                 char buffer[5];
                unsigned char bytes[4];
                for(int i=0;i<3;i++){
                  
                   
                    fread(buffer,sizeof(char),4,edt->source_fptr);
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    fread(bytes,sizeof(char),4,edt->source_fptr);
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);

                    int len=big_end_to_lit_end(bytes);

                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(string,sizeof(char),len-1,edt->extra_fptr);
                }
                fread(buffer,sizeof(char),4,edt->source_fptr);
                buffer[4]='\0';
                if(!(strcmp(buffer,"TYER"))){
                
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    int new_len=strlen(edt->modfy_content)+1;
                    
                    int_to_BigEndian(new_len,bytes);
                    
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);
                    unsigned char size[4];
                    fread(size, sizeof(char), 4, edt->source_fptr);  
                    int len=big_end_to_lit_end(size);

                    
                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr);
                }
                char ch;
                while(fread(&ch,1,1,edt->source_fptr)>0){
                    fwrite(&ch,1,1,edt->extra_fptr);
                }
                printf("%s\n",edit_details);
                printf("%s\n",edit_opt);
                printf("---------------------CHANGE THE YEAR--------------------\n");
                printf("YEAR :   %s\n\n",edt->modfy_content);

                }
            
                else if(!(strcmp(edt->modfy_type,"-m"))){
                     printf("In content\n");
                     char buffer[5];
                     unsigned char bytes[4];
                     for(int i=0;i<4;i++){
                  
                   
                        fread(buffer,sizeof(char),4,edt->source_fptr);
                        fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                        fread(bytes,sizeof(char),4,edt->source_fptr);
                        fwrite(bytes,sizeof(char),4,edt->extra_fptr);

                        int len=big_end_to_lit_end(bytes);

                        fread(default_flag,sizeof(char),3,edt->source_fptr);
                        fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                        char string[len];
                        fread(string,sizeof(char),len-1,edt->source_fptr);
                        fwrite(string,sizeof(char),len-1,edt->extra_fptr);
                    }
                    fread(buffer,sizeof(char),4,edt->source_fptr);
                    buffer[4]='\0';
                    if(!(strcmp(buffer,"TCON"))){
                
                        fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                        int new_len=strlen(edt->modfy_content)+1;
                        
                        int_to_BigEndian(new_len,bytes);
                        
                        fwrite(bytes,sizeof(char),4,edt->extra_fptr);
                        unsigned char size[4];
                        fread(size, sizeof(char), 4, edt->source_fptr);  
                        int len=big_end_to_lit_end(size);

                        
                        fread(default_flag,sizeof(char),3,edt->source_fptr);
                        fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                        char string[len];
                            fread(string,sizeof(char),len-1,edt->source_fptr);
                            fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr);
                    }
                    char ch;
                    while(fread(&ch,1,1,edt->source_fptr)>0){
                        fwrite(&ch,1,1,edt->extra_fptr);
                    }
                    printf("%s\n",edit_details);
                    printf("%s\n",edit_opt);
                    printf("---------------------CHANGE THE CONTENT--------------------\n");
                    printf("CONTENT:   %s\n\n",edt->modfy_content);
                }
              //not editable
            else if(!(strcmp(edt->modfy_type,"-c"))){
                printf("In comment\n");
                 char buffer[5];
                unsigned char bytes[4];
                for(int i=0;i<5;i++){
                  
                   
                    fread(buffer,sizeof(char),4,edt->source_fptr);
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    fread(bytes,sizeof(char),4,edt->source_fptr);
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);

                    int len=big_end_to_lit_end(bytes);

                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    char string[len];
                    fread(string,sizeof(char),len-1,edt->source_fptr);
                    fwrite(string,sizeof(char),len-1,edt->extra_fptr);
                }
                fread(buffer,sizeof(char),4,edt->source_fptr);
                buffer[4]='\0';
                if(!(strcmp(buffer,"COMM"))){
                
                    fwrite(buffer,sizeof(char),4,edt->extra_fptr);

                    int new_len=strlen(edt->modfy_content)+1;
                    
                    int_to_BigEndian(new_len,bytes);
                    
                    fwrite(bytes,sizeof(char),4,edt->extra_fptr);
                    unsigned char size[4];
                    fread(size, sizeof(char), 4, edt->source_fptr);  
                    int len=big_end_to_lit_end(size);

                    edt->comment_len=new_len;
                    fread(default_flag,sizeof(char),3,edt->source_fptr);
                    fwrite(default_flag,sizeof(char),3,edt->extra_fptr);

                    // fread(default_flag,sizeof(char),4,edt->source_fptr);
                    // fwrite(default_flag,sizeof(char),4,edt->extra_fptr);

                    char string[len];
                    fread(string,sizeof(char),len-1,edt->source_fptr);
                    fwrite(edt->modfy_content,sizeof(char),new_len-1,edt->extra_fptr);
                }
                char ch;
                while(fread(&ch,1,1,edt->source_fptr)>0){
                    fwrite(&ch,1,1,edt->extra_fptr);
                }
                printf("%s\n",edit_details);
                printf("%s\n",edit_opt);
                printf("---------------------CHANGE THE COMMENT--------------------\n");
                printf("COMMENT:  %s\n\n",edt->modfy_content);
            }  
            else{
                printf("Error in operation\n");
                return failure;
            }
    return success;
}

// Function to rename temp file back to original file name
status file_rename(struct tag *edt){
    if(rename(edt->extra_filename,edt->filename)==failure){
        printf("ERROR in file renaming\n");
        return failure;
    }
    else{
       // printf("File renamed successfully\n");
    }
    if(remove(edt->extra_filename)==failure){
        printf("Error in file removing\n");
        return failure;
    }
    else{
       // printf("File removed successfully\n");
    }
    return success;
}
// Main editing function (open -> copy header -> operation -> rename file)
status editing(struct tag *edt){

    if(open_files(edt)==failure){
        printf("ERROR in opening file\n");
        return failure;
    }
    else{
        //printf("Files opened successfully\n");
    }
    if(copy_header(edt)==failure){
        printf("Error in copy header\n");
        return failure;
    }
    else{
        //printf("Copied header successfully\n");
    }
    if(operation(edt)==failure){
        printf("Error in operation\n");
        return failure;
    }
    else{
       // printf("Operation done successfully\n");
    }
    if(file_rename(edt)==failure){
        printf("Error in file handling\n");
        return failure;
    }
    else{
        //printf("File handling done successfully\n");
    }
    return success;
}

// ---------------- Main Function ----------------
int main(int argc,char *argv[]){
    // If no arguments provided
    if(argc==1){
        printf("\n----------ERROR MESSAGE----------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE\n");
        printf("To view please pass like: ./a.out -v  filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        return 0;
    }
    int ret=check_operation_type(argv[1]);// Check operation type
    // ---------------- View Mode ----------------
    if(ret==view){
        // Require exactly 3 arguments
        if(argc<3||argc>3){
            printf("\n----------ERROR MESSAGE----------\n");
            printf("USAGE\n");
            printf("To view please pass like: ./a.out -v  filename.mp3\n");
            return 0;
        }
        struct tag vw;
        if((strstr(argv[2],".mp3"))==NULL){ // Validate mp3 file
            printf("\n----------ERROR MESSAGE----------\n");
            printf("please provide mp3 file.\n");
            return 0;
        }
        vw.filename=argv[2];// Store filename
        if(Detailsview(&vw)==failure){ // Display file details
            printf("Failure in viewing\n");
            return 0;
        }
        else{
            printf("View done successfully\n");
        }   
    }

      // ---------------- Edit Mode ----------------
    else if(ret==edit){
        printf("in editing\n");
        if(argc<5||argc>5){ // Require exactly 5 arguments
            printf("\n----------ERROR MESSAGE----------\n");
            printf("USAGE\n");
            printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename\n");
            return 0;
        }
        if((strstr(argv[2],"-t")||strstr(argv[2],"-a")||strstr(argv[2],"-A")||strstr(argv[2],"-y")||strstr(argv[2],"-m")||strstr(argv[2],"-c"))==0){
            printf("\n----------ERROR MESSAGE----------\n");
            printf("Please provide -t/-a/-A/-m/-y/-c \n");
            return 0;
        }
        struct tag edt;
        strcpy(edt.modfy_type,argv[2]); // Store modification type
        if((strstr(argv[4],".mp3"))==NULL){ // Validate mp3 file
            printf("\n----------ERROR MESSAGE----------\n");
            printf("please provide mp3 file.\n");
            return 0;
        }
        edt.filename=argv[4];
        strcpy(edt.modfy_content,argv[3]);// Store new content
        if(editing(&edt)==failure){ // Perform editing
            printf("ERROR in editing\n");
            return 0;
        }
        else{
            printf("EDITING done successfully\n");
        }
    }
    // ---------------- Help Menu ----------------
    else if(ret==help){
        printf("\n----------HELP MENU----------\n");
        printf("1. -v -> to view mp3 file content\n");
        printf("2. -e -> to edit mp3 file content\n");
        printf("\t 2.1 -t -> to edit song title\n");
        printf("\t 2.2 -a -> to edit artist name\n");
        printf("\t 2.3 -A -> to edit album name\n");
        printf("\t 2.4 -y -> to edit year\n");
        printf("\t 2.5 -m -> to edit content\n");
        printf("\t 2.6 -c -> to edit comment\n");
        printf("\n---------------------------------\n");
        return 0;
    }
    // ---------------- Invalid Argument ----------------
    else{
        printf("%s\n",strline);
         printf("\n-------------------ERROR MESSAGE--------------------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE\n");
        printf("To view please pass like: ./a.out -v  filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("%s\n",strline);
    }
return 0;
}