//
// Created by uchia on 9/23/2024.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define PASS_LEN 6
#define PHONE_LEN 9
#define MAXIMUM_RECORD 5
// variable declaration and initialization
float transaction_bank=100000;
int g_userCount=0;
int trans_count=0;
// create a structure
typedef struct {
    int user_ID;
    char username[10];
    char email[20];
    char passcode[10];
    /*char dob[10];
    char addr[30];
    char country[10];
    int postal_code;*/
    char phone_no[10];
    double signs;
    int account_state;
    int period;
    int record_count;
    char stored_record[MAXIMUM_RECORD][150];

}Db;
Db data[20];


void initilize_Data();
void record_Data();
void retrieve_Data();
void menu();
void registration();
void copy_char_data(char destination[30],char source[30]);
int comp_cha_arr(char first[30],char second[30]);
int size_of_cha_arr(char arr[30]);
int email_exists(char email[]);
int email_valid(char check_email[]);
int length_check(char ch_string[]);
int pass_valid_check(char password[]);
int ph_valid(char ph_no[]);
int phone_exists(char ph_no[]);
void login();
void admin_privileges(int userid);
void user_privileges(int userid);
void currency_transaction(int userid);
void do_transaction(int sender_id,int receiver_id,float* bankptr);
void manage_user(int admin_id);
void block_user(int id);
void suspend_user(int id);
void resume_user(int id);
void profile(int userid);
void setting(int userid);
void history(int userid);
void showrecord(int userid);
// initial user data
Db initialize[4]={{1,"admin1","admin1@gmail.com","@tMi&1","094200001",2000,0,0,0,""},
                 {2,"admin2","admin2@gmail.com","@tMi&2","094200005",2000,0,0,0,""},
                 {3,"admin3","admin3@gmail.com","@tMi&3","094200010",2000,0,0,0,""},
                 {4,"user1","user1@gmail.com","Y0u1#r","094200015",1000,0,0,0,""}};


int main(){
    FILE *init_file;
    int ch;

    init_file = fopen("database.bin","rb");
    if (!init_file ){
        init_file  = fopen("database.bin","wb");
        fclose(init_file );
        init_file  = fopen("database.bin","rb");
    }
    ch = getc(init_file );
    if (feof(init_file )){
        fclose(init_file );
        initilize_Data();
    }else{
        retrieve_Data();
    }
    menu();

    return 0;
}
void initilize_Data(){
    printf("Initializing Data Structure Db!!!\n");
    for(int j=0;j<4;j++){
        data[j]=initialize[j];
        printf("Db%d--ID:%d-%s-%s-%s-%s-%lf-%d-%d-%d-%s\n",j,data[j].user_ID,data[j].username,data[j].email,data[j].passcode,
               data[j].phone_no,data[j].signs,data[j].account_state,data[j].period,data[j].record_count,data[j].stored_record);
        g_userCount = j+1;
    }
}

void record_Data(){
    printf("\n\nSaving User Data to File!!!\n");
    printf("Transaction bank record of asset(%.2f) is stored !!!\n",transaction_bank);
    printf("%d registered users' record is stored!!!\n",g_userCount);
    // restore data
    FILE* stdata = fopen("database.bin","wb");
    if(!stdata){
        perror("Recording Data Error!\n"); }

    for (int i=0;i<g_userCount;i++){
        fwrite(&data[i],sizeof(Db),1,stdata); }
    fclose(stdata);
}

void retrieve_Data(){
    printf("Loading data to Db!!!\n");
    FILE *stdata = fopen("database.bin","rb");
    if(!stdata){
        perror("Retrieving Data Error!\n");
    }
    while(fread(&data[g_userCount],sizeof(Db),1,stdata)){
        printf("Db%d--ID:%d-%s-%s-%s-%s-%lf-%d-%d-%d\n",g_userCount,data[g_userCount].user_ID,data[g_userCount].username,
               data[g_userCount].email,data[g_userCount].passcode,data[g_userCount].phone_no,data[g_userCount].signs,
               data[g_userCount].account_state,data[g_userCount].period,data[g_userCount].record_count);
               for (int j=0;j<data[g_userCount].record_count;j++){
                    printf("%s",data[g_userCount].stored_record[j]);
                }printf("%s","\n");
        g_userCount++;
    }
}

void menu(){
    while (1){
        printf("\nWelcome to Our ChinLand Bank System\n");
        printf("Explore Your Sings!");
        int option =0;
        printf("\nPress 1 To_Register: \nPress 2 To_Login: \nPress Any-key To_Exit: ");
        scanf("%d",&option);
        if (option == 1){
            printf("Welcome Registration Session\n");
            registration();
        }else if (option == 2){
            printf("Welcome login Session\n");
            login();
        }else{
            printf("Exit!!!");
            record_Data();
            exit(1);
        }
    }
}

void registration(){
    printf("Welcome User Registration:\n");
    printf("%d users are already registered!!!\n",g_userCount);

    int email_check_status =0;
    int password_check_status =0;
    int phone_check_status =0;
    //reg_username
    char r_username[20];
    printf("\nEnter username: ");
    scanf(" %[^\n]",&r_username);
    copy_char_data(data[g_userCount].username,r_username);
    // end of user name registration

    //reg_email(no_of_reg_user);
    char r_email[20];
    while(email_check_status==0){
        printf("Enter your email: ");
        scanf(" %[^\n]",&r_email);
        int exist_check = email_exists(r_email);
        int valid_check = email_valid(r_email);
        if (!exist_check&&!valid_check){
            email_check_status=1;
        }else if(exist_check){
            //email_check_status=0;
            printf("email exists! Use different email!!!\n");
        }else if(valid_check){
            printf("email format invalid!\nUse 'name@domain.com' format!\nUse 'valid domain'\nUse 'lowercase character and number' only for name\n");
        }
    }
    copy_char_data(data[g_userCount].email,r_email);
    // End of email registration

    // password registration
    char r_passcode[20];
    while(password_check_status==0){
        printf("Enter user password\tMinimum length requirement\t-At least 6 characters\n");
        printf("Other requirements-\t-At least a number\t-At least a special character\t-At least an Upper Case letter:  ");
        scanf("%s",&r_passcode);
        int pass_len_check_status = length_check(r_passcode);
        int pass_valid_check_status = pass_valid_check(r_passcode);
        if (pass_len_check_status &&pass_valid_check_status){
            printf("password valid\n");
            password_check_status=1;
        }else if(!pass_len_check_status ){
            printf("Minimum password length must be fulfilled\n");
        }else if(!pass_valid_check_status){
            printf("Password requirements must be fulfilled\n");
        }
    }
    copy_char_data(data[g_userCount].passcode,r_passcode);
// End password registration

// date of birth registration
//    char r_dob[10];
//    printf("Enter Date of birth! \nUse dd/mm/yy format!\n");
//    scanf(" %[^\n]",&r_dob);
//    copy_cha_data(data[g_userCount].dob,r_dob);
//
//
//    char r_addr[30];
//    printf("Enter your address! \n");
//    scanf(" %[^\n]",&r_addr);
//    copy_cha_data(data[g_userCount].addr,r_addr);
//
//    char r_country[10];
//    printf("Enter your Country! \n");
//    scanf(" %[^\n]",&r_country);
//    copy_cha_data[g_userCount].country,r_country);
//
//    int r_postal_code;
//    printf("Enter postal code! \n");
//    scanf(" %[^\n]",&r_postal_code);
//    data[g_userCount].postal_code=r_postal_code;

    char r_phone_no[10];
    while( phone_check_status==0){
        printf("Enter user phone number: ");
        scanf(" %[^\n]",&r_phone_no);
        //phone validation
        if(!ph_valid(r_phone_no)){
            printf("Valid number must have 9 numbers starting with '09'!  Please give valid number!!!\n");
        }else{
            if(phone_exists(r_phone_no)){
                printf("Use different phone number.This number already exists!!!\n");
            }else{
                phone_check_status==1;
                break;
            }
        }
    }
    copy_char_data(data[g_userCount].phone_no,r_phone_no);

    g_userCount++;
    data[g_userCount-1].user_ID = g_userCount;
    data[g_userCount-1].signs = 1000;
    data[g_userCount-1].account_state = 0;
    data[g_userCount-1].period = 0;
    data[g_userCount-1].record_count = 0;

    printf("Registration Complete!!!\n");
    printf("User id : %d , %s is successfully registered. %s got %lf -Signs! \n",data[g_userCount-1].user_ID,
           data[g_userCount-1].username,data[g_userCount-1].username,data[g_userCount-1].signs);

    printf("Check Database after new registration\n");
    for (int j=0;j<g_userCount;j++){
        printf("Db%d--ID:%d-%s-%s-%s-%s-%lf-%d-%d-%d-%s\n",j,data[j].user_ID,data[j].username,data[j].email,data[j].passcode,
               data[j].phone_no,data[j].signs,data[j].account_state,data[j].period,data[j].record_count,data[j].stored_record);
    }
}

void copy_char_data(char destination[30],char source[30]){

    for (int i=0;i<30;i++){
        if (source[i]=='\0'){
            break;
        }else{
            destination[i]=source[i];
        }
    }
}

int comp_cha_arr(char first[30],char second[30]){
    int size_1 = size_of_cha_arr(first);
    int size_2 = size_of_cha_arr(second);

    if (size_1==size_2){
        for (int i=0;i<size_1;i++){
            if (first[i]==second[i]){
                continue;
            }else{
                return 0;
            }
        }return 1;// return for same array
    }else{
        return 0;// return for different array
    }
}
int size_of_cha_arr(char arr[30]){
    int size =0;
    for (int i=0;i<30;i++){
        if (arr[i] == '\0'){
            break;
        }
        size++;
    }
    return size;
}

int email_exists(char email[]){
    for (int i=0;i<=g_userCount;i++){
        if(comp_cha_arr(data[i].email,email)==1){
           return 1;
           break;
        }
    }
    return 0;
}
int email_valid(char check_email[]){
    int first_count =0;
    int email_size = strlen(check_email);
    int valid_status = -1;

    for (int i=0;i<email_size;i++){
       if (check_email[i]=='@'){
            break;
       }else{
            first_count++;
       }
    }
    // check name_valid
    if (first_count!=email_size){
        for (int i=0;i<first_count;i++){
           if  (!((check_email[i]>47&&check_email[i]<58)||(check_email[i]>96&&check_email[i]<123))){
                printf("the first part email is invalid\n");
                return 1;
            }
        }
    }else{
        printf("[+]the first part email is invalid\n");
        return 1;
    }
    //check domain_valid
    int second_count = email_size-first_count;
    int domain_arr_index=0;
    char domain_arr[second_count];

    for (int i =first_count;i<email_size;i++){
        domain_arr[domain_arr_index]=check_email[i];
        domain_arr_index++;
    }
    domain_arr[domain_arr_index]= '\0';
    int len_domain_arr=strlen(domain_arr);

    char domain1[15]={'@','g','m','a','i','l','.','c','o','m'};
    char domain2[15]={'@','y','a','h','o','o','.','c','o','m'};
    char domain3[15]={'@','a','p','p','l','e','.','c','o','m'};
    char domain4[15]={'@','o','u','t','l','o','o','k','.','c','o','m'};
    char domain5[15]={'@','n','1','c','.','c','o','m'};

    int domain1_check = comp_cha_arr(domain_arr,domain1);
    int domain2_check = comp_cha_arr(domain_arr,domain2);
    int domain3_check = comp_cha_arr(domain_arr,domain3);
    int domain4_check = comp_cha_arr(domain_arr,domain4);
    int domain5_check = comp_cha_arr(domain_arr,domain5);

    if ((domain1_check||domain2_check||domain3_check||domain4_check||domain5_check)){
        printf("\nemail is valid\n");
        return 0;
    }else{
        printf("\nemail is invalid\n");
        return 1;
    }
}

int length_check(char ch_string[]){
    int len_str = strlen(ch_string);
    if (len_str <PASS_LEN){
        return 0;
    }else{
        return 1;
    }
}
int pass_valid_check(char password[]){
    int len_str = strlen(password);
    int number_check =0;
    int upCase_check = 0;
    int specialChar = 0;

    for (int i=0;i<len_str;i++){
        if (password[i]>47&&password[i]<57){//number check
            number_check =1;
            break;
        }
    }
    for (int i=0;i<len_str;i++){
        if (password[i]>64&&password[i]<91){//upper&@ check
            upCase_check =1;
            break;
        }
    }
    for (int i=0;i<len_str;i++){
        if(password[i]==33||password[i]==42||password[i]==35||password[i]==36||password[i]==37||password[i]==38||password[i]==64){
            specialChar = 1;
            break;
        }
    }
    if (number_check&&upCase_check&&specialChar){
        return 1;
    }
    return 0;
}
int ph_valid(char ph_no[]){
    int len_str = strlen(ph_no);
    if (len_str == PHONE_LEN){
        if (ph_no[0]==48 && ph_no[1]==57){
            for (int j=2;j<PHONE_LEN;j++){
                if(ph_no[j]>47 && ph_no[j]<58){
                    if (j==PHONE_LEN-1){
                    return 1;}
                }
            }printf("must have numbers only");return 0;
        }else{printf("starting wrong number");return 0;}
    }else{printf("length requirement");return 0;}
}
int phone_exists(char ph_no[]){
    for (int i=0;i<=g_userCount;i++){
        if(comp_cha_arr(data[i].phone_no,ph_no)==1){
           return 1;
           break;
        }
    }
    return 0;
}
void login(){
    int no_check_users =0;
    char l_email[30];
    char l_passcode[30];

    printf("Enter email: ");
    scanf(" %[^\n]",&l_email);
    printf("Enter password: ");
    scanf(" %[^\n]",&l_passcode);

    for (int i=0;i<g_userCount ;i++){
        no_check_users++;
        if (comp_cha_arr(l_email,data[i].email)){
            if (comp_cha_arr(l_passcode,data[i].passcode)){
                if (data[i].user_ID==1 ||data[i].user_ID==2 ||data[i].user_ID==3){
                    printf("Login Succeed!!!\n\n");
                    admin_privileges(data[i].user_ID);
                    break;
                }else{
                    if(data[i].account_state==0){
                        printf("Login Succeed!!!\n\n");
                        user_privileges(data[i].user_ID);
                        break;
                    }else if(data[i].account_state==1){
                        printf("You account is block!!!\n\n");
                        break;
                    }else if(data[i].account_state==2){
                        printf("You account is suspended for %d days!!!\n\n",data[i].period);
                        break;
                    }
                }
            }else{
                printf("Username and password did not match!!!\n\n");
                break;
            }
        }
        if (g_userCount==no_check_users){
            printf("Login Failed!!!\n\n");
        }
    }
}
void admin_privileges(int userid){
    int option=0;
    printf("\nWelcome %s. Your user id is %d!!!\n",data[userid-1].username,userid);
    printf("Choose options:\n1 for currency_transaction\n2 for manage_User\n3 for show history\n4 for check profile\n5 for change settings\n6 show transaction record\n7 for logging out: ");
    scanf("%d",&option);

    if (option==1){
       currency_transaction(userid);
    }else if(option==2){
        manage_user(userid);
    }else if(option==3){
        history(userid);
    }else if(option==4){
        profile(userid);
    }else if (option==5){
        setting(userid);
    }else if(option==6){
        showrecord(userid);
    }else if(option==7){
        record_Data();
        exit(1);
    }
}
void user_privileges(int userid){
    int option=0;
    printf("\nWelcome %s. Your user id is %d!!!\n",data[userid-1].username,userid);
    printf("Choose options:\n1 for currency_transaction\n2 for check profile\n3 for show history\n4 for change settings\n5 show transaction record\n6 for logging out: ");
    scanf("%d",&option);

    if (option==1){
       currency_transaction(userid);
    }else if(option==2){
        profile(userid);
    }else if(option==3){
        history(userid);
    }else if(option==4){
        setting(userid);
    }else if(option==5){
        showrecord(userid);
    }else if(option==6){
        record_Data();
        exit(1);
    }
}
void profile(int userid){
    printf("\nThis is my profile!\n");
    if (userid==1 ||userid==2||userid==3){
        admin_privileges(userid);
    }else{
        user_privileges(userid);
    }
}
void setting(int userid){
    printf("\nChange setting\n");
    if (userid==1 ||userid==2||userid==3){
        admin_privileges(userid);
    }else{
        user_privileges(userid);
    }
}
void history(int userid){
    printf("\nSee History\n");
    if (userid==1 ||userid==2||userid==3){
        admin_privileges(userid);
    }else{
        user_privileges(userid);
    }
}
void showrecord(int userid){
    for (int i=0;i<data[userid-1].record_count;i++){
        printf("%s",data[userid-1].stored_record[i]);
    }

    if (userid==1 ||userid==2||userid==3){
        admin_privileges(userid);
    }else{
        user_privileges(userid);
    }
}

void manage_user(int admin_id){
    printf("This is User Management Session!!!\n");
    int puser_ID=0;
    int manage_option=0;
    printf("Give user_ID to manage:    ");
    scanf("%d",&puser_ID);
    if (puser_ID==1 ||puser_ID==2||puser_ID==3){
        printf("You can't manage this account!!!\n");
        admin_privileges(admin_id);
    }else{
        printf("Press 1 for user_block\nPress 2 for user_suspend\nPress 3 for user_resume\n");
        scanf("%d", &manage_option);
        switch(manage_option){
        case 1:
                printf("Manage to :  %s, %s\n",data[puser_ID-1].username,data[puser_ID-1].email);
                block_user(puser_ID);
                break;

        case 2:
                printf("Manage to :  %s, %s\n",data[puser_ID-1].username,data[puser_ID-1].email);
                suspend_user(puser_ID);
                break;

        case 3:
                printf("Manage to :  %s, %s\n",data[puser_ID-1].username,data[puser_ID-1].email);
                resume_user(puser_ID);
                break;
        }
    }admin_privileges(admin_id);
}

void block_user(int id){
    data[id-1].account_state=1;
   // printf("Change user account_state to %d\n",data[id-1].account_state);
    printf("%s account is blocked!!!",data[id-1].username);
}
void  suspend_user(int id){
    int period;
    printf("Give a period:  ");
    scanf("%d",&period);
    data[id-1].account_state = 2;
    data[id-1].period = period;
    printf("%s account is currently unavailable.It is suspended for %d days!!!\n",data[id-1].username,data[id-1].period);
}

void resume_user(int id){
    data[id-1].account_state = 0;
    data[id-1].period =0;
    printf("%d account is available now!!!\n",data[id-1].username);
}
void currency_transaction(int userid){
    int i=0;
    char t_phoneno[10];
    printf("Enter phone number you want to transfer: \n");
    scanf(" %[^\n]",&t_phoneno);
    printf("%s\n",data[userid-1].phone_no);
    if (!(comp_cha_arr(t_phoneno,data[userid-1].phone_no))){
        while (i<g_userCount){
            if(comp_cha_arr(t_phoneno,data[i].phone_no)){
                int receive_ID = i+1;
                do_transaction(userid,receive_ID,&transaction_bank);
            }i++;
        }printf("No user to transfer!!!\n");
    }else{
        printf("You can not transfer to yourself!!!\n");
    }
    if (userid==1 ||userid==2||userid==3){
        admin_privileges(userid);
    }else{
        user_privileges(userid);
    }
}

 void do_transaction(int sender_id,int receiver_id,float* bankptr){
    int trans_option;
    int make_transaction;
    float amount=0;
    float minimum_balance=50;
    float required_signs=0;
    float transfer_signs=0;
    float transaction_fee =0;
    int trans_count=0;
    char temp_store[150];
    char temp_store1[150];
    int max_record_count=(data[sender_id-1].record_count)%MAXIMUM_RECORD;
    int max_record_count1=(data[receiver_id-1].record_count)%MAXIMUM_RECORD;
//    int count1 = data[sender_id-1].record_count;
//    int count2 = data[receiver_id-1].record_count;
    while(1){
        printf("Press 1 to check transaction fee! Press 2 to make a transaction! Press 3 go back: ");
        scanf("%d",&trans_option);

        if (trans_option==1){
            printf("0.1 percent for Signs<200 \n0.2 percent for 200<=Signs<500\n");

        //currency_transaction(sender_id,receiver_id,&transaction_bank);
        }else if(trans_option==2){
            printf("Confirm transaction, press 1! To cancel press 2:   " );
            scanf("%d",&make_transaction);

        //check signs and transferr
            while(make_transaction){
                printf("Give amount you want to transfer:   ");
                scanf("%f",&amount);
                if (amount>0&&amount<200){
                    transaction_fee = amount*0.001;
                }else if (amount>=200){
                    transaction_fee = amount*0.002;
                }
                required_signs=amount+transaction_fee+minimum_balance;
                transfer_signs = amount+transaction_fee;
                printf("Required signs is %f\n",required_signs);

                if (data[sender_id-1].signs>=required_signs){
                    printf("Before transaction %s has %lf and %s has %lf.\n",
                        data[sender_id-1].username,data[sender_id-1].signs,data[receiver_id-1].username,data[receiver_id-1].signs);
                    data[sender_id-1].signs-=transfer_signs;
                    data[receiver_id-1].signs+=amount;
                    transaction_bank+=transaction_fee;

                    printf("After transaction %s has %lf and %s has %lf.\n",
                        data[sender_id-1].username,data[sender_id-1].signs,data[receiver_id-1].username,data[receiver_id-1].signs);
                    printf("Transaction bank got the charges of %lf and has total signs of %lf.\n",transaction_fee,transaction_bank);
                    printf("Transaction Successful!!!\n\n");

            // record sender transactions
                    time_t t = time(NULL);
                    char time_now[20];
                    strcpy(time_now,ctime(&t));


                    sprintf(temp_store,"%d. %s transfers to %s %lf signs at %s",data[sender_id-1].record_count+1,data[sender_id-1].username,data[receiver_id-1].username,amount,time_now);
                    if (data[sender_id-1].record_count<MAXIMUM_RECORD){
                        if (max_record_count!=0){
                            for (int i=max_record_count;i>=1;i--){
                                strcpy(data[sender_id-1].stored_record[i],data[sender_id-1].stored_record[i-1]);
                            }
                        }
                        strcpy(data[sender_id-1].stored_record[0],temp_store);
                        printf("%s",data[sender_id-1].stored_record[0]);
                        data[sender_id-1].record_count++;
                    }else{
                        for (int i=MAXIMUM_RECORD-1;i>=1;i--){
                            strcpy(data[sender_id-1].stored_record[i],data[sender_id-1].stored_record[i-1]);
                            }
                        strcpy(data[sender_id-1].stored_record[0],temp_store);
                        printf("%s",data[sender_id-1].stored_record[0]);
                        data[sender_id-1].record_count++;
                    }


            // record receiver transactions
                    sprintf(temp_store,"%d. %s receives from %s %lf signs at %s",data[receiver_id-1].record_count+1,data[receiver_id-1].username,data[sender_id-1].username,amount,time_now);
                    if (data[receiver_id-1].record_count<MAXIMUM_RECORD){
                        if (max_record_count1!=0){
                            for (int i=max_record_count1;i>=1;i--){
                                strcpy(data[receiver_id-1].stored_record[i],data[receiver_id-1].stored_record[i-1]);
                            }
                        }
                        strcpy(data[receiver_id-1].stored_record[0],temp_store);
                        printf("%s",data[receiver_id-1].stored_record[0]);
                        data[receiver_id-1].record_count++;
                    }else{
                        for (int i=MAXIMUM_RECORD-1;i>=1;i--){
                            strcpy(data[receiver_id-1].stored_record[i],data[receiver_id-1].stored_record[i-1]);
                        }
                        strcpy(data[receiver_id-1].stored_record[0],temp_store);
                        printf("%s",data[receiver_id-1].stored_record[0]);
                        data[receiver_id-1].record_count++;
                    }

                    make_transaction=0;
                    if (sender_id==1 ||sender_id==2||sender_id==3){
                        admin_privileges(sender_id);
                    }else{
                        user_privileges(sender_id);
                    }

                }else{
                    printf("Not enough sings!Try again:");
                }
            }
        }else if(trans_option==3){
            if (sender_id==1 ||sender_id==2||sender_id==3){
                admin_privileges(sender_id);
            }else{
                user_privileges(sender_id);
            }
        }
    }
}

