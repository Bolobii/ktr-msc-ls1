#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

typedef struct userDetails {
    char name[32];
    char companyName[32];
    char email[64];
    char phone[16];
    char password[32];
} userDetails;

typedef struct businessCard {
    char name[32];
    char companyName[32];
    char email[64];
    char phone[16];
} businessCard;

int info() {
    userDetails user;
    char *buffer;
    char *path;
    FILE *fd;
    int i;
    short size[4];
    char password[32];

    buffer = (char *) malloc(sizeof(char)*64);
    path = (char *) malloc(sizeof(char)*128);

    mkdir("users");

    printf("Profile : \n");
    printf("Name : ");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        user.name[i] = buffer[i];
    }
    size[0] = i;
    user.name[i] = '\0';
    printf("Company name : ");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        user.companyName[i] = buffer[i];
    }
    size[1] = i;
    user.companyName[i] = '\0';
    printf("Email address : ");
    scanf("%s", buffer);
    for(i=0; i<63 && buffer[i] != 0; i++) {
        user.email[i] = buffer[i];
    }
    size[2] = i;
    user.email[i] = '\0';
    printf("Phone number : ");
    scanf("%s", buffer);
    for(i=0; i<15 && buffer[i] != 0; i++) {
        user.phone[i] = buffer[i];
    }
    size[3] = i;
    user.phone[i] = '\0';
    printf("Password : ");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        // Offset to the password when saving it using Vigenere encrypting
        user.password[i] = buffer[i]+3;
    }

    
    path[0] = '\0';
    strcat(path, "users/");
    strcat(path, user.email);
    strcat(path, ".bin");

    fd = fopen(path, "ab");

    fseek(fd, 0, SEEK_END);
    if(ftell(fd) != 0) {
        printf("Account already exists, return to main menu.\n");
        return 0;
    }

    fwrite(&size[2], sizeof(short), 1, fd);
    fputs(user.email, fd);
    fwrite(&i, sizeof(short), 1, fd);
    fputs(user.password, fd);
    fwrite(&size[0], sizeof(short), 1, fd);
    fputs(user.name, fd);
    fwrite(&size[1], sizeof(short), 1, fd);
    fputs(user.companyName, fd);
    fwrite(&size[3], sizeof(short), 1, fd);
    fputs(user.phone, fd);

    fclose(fd);
    free(buffer);
    free(path);
}

char *login(char user[64]) {
    char buffer[64];
    char path[64];
    char email[64];
    char password[32];
    short size, size2;
    int i;
    FILE *fd;

    printf("Enter log in details: \n");
    printf(" Email : \n");
    scanf("%s", buffer);

    for(i=0; i<63 && buffer[i] != 0; i++) {
        email[i] = buffer[i];
    }
    email[i] = '\0';

    strcpy(path, "\0");
    strcat(path, "users/");
    strcat(path, email);
    strcat(path, ".bin");

    if((fd = fopen(path, "rb+")) == NULL) {
        printf("Account doesn't exist, return to main menu\n");
        user = "0";
        return "0";
    }

    fread(&size, sizeof(short), 1, fd);
    fseek(fd, (sizeof(char)*size)+sizeof(short), SEEK_SET);

    fread(&size2, sizeof(short), 1, fd);

    printf(" Password : \n");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        password[i] = buffer[i]+3;
    }
    password[i] = '\0';

    fread(buffer, sizeof(char), size, fd);
    buffer[size] = '\0';

    fclose(fd);
    if(strcmp(buffer, password) != 0) {
        printf("Passwords don't match, return to main menu\n");
        user = "0";
        return "0";
    }

    strcpy(user, email);
    return user;
}

void addCard(char user[64]) {
    businessCard card;
    char *buffer;
    char *path;
    FILE *fd;
    int i;
    short size[4];
    short nbCards = 0;

    buffer = (char *) malloc(sizeof(char)*64);
    path = (char *) malloc(sizeof(char)*128);

    mkdir("cards");

    printf("Profile : \n");
    printf("Name : ");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        card.name[i] = buffer[i];
    }
    size[0] = i;
    card.name[i] = '\0';
    printf("Company name : ");
    scanf("%s", buffer);
    for(i=0; i<31 && buffer[i] != 0; i++) {
        card.companyName[i] = buffer[i];
    }
    size[1] = i;
    card.companyName[i] = '\0';
    printf("Email address : ");
    scanf("%s", buffer);
    for(i=0; i<63 && buffer[i] != 0; i++) {
        card.email[i] = buffer[i];
    }
    size[2] = i;
    card.email[i] = '\0';
    printf("Phone number : ");
    scanf("%s", buffer);
    for(i=0; i<15 && buffer[i] != 0; i++) {
        card.phone[i] = buffer[i];
    }
    size[3] = i;
    card.phone[i] = '\0';

    
    path[0] = '\0';
    strcat(path, "cards/");
    strcat(path, user);
    strcat(path, ".bin");

    fd = fopen(path, "ab+");

    fseek(fd, 0, SEEK_END);
    
    fwrite(&size[0], sizeof(short), 1, fd);
    fputs(card.name, fd);
    fwrite(&size[1], sizeof(short), 1, fd);
    fputs(card.companyName, fd);
    fwrite(&size[2], sizeof(short), 1, fd);
    fputs(card.email, fd);
    fwrite(&size[3], sizeof(short), 1, fd);
    fputs(card.phone, fd);

    fclose(fd);
}

void viewCards(char user[64]) {
    short nbCards;
    char buffer[64];
    char *path;
    FILE *fd;
    int i;
    short size;

    path = (char *) malloc(sizeof(char)*128);

    printf("Your cards : \n");

    mkdir("cards");

    path[0] = '\0';
    strcat(path, "cards/");
    strcat(path, user);
    strcat(path, ".bin");

    fd = fopen(path, "ab+");

    fseek(fd, 0, SEEK_END);
    if (ftell(fd) > 0) {
        fseek(fd, 0, SEEK_SET);
        while(!feof(fd)) {
            fread(&size, sizeof(short), 1, fd);
            fread(buffer, sizeof(char), size, fd);
            buffer[size] = '\0';
            printf("Name : %s", buffer);
            fread(&size, sizeof(short), 1, fd);
            fread(buffer, sizeof(char), size, fd);
            buffer[size] = '\0';
            printf(" Company name : %s", buffer);
            fread(&size, sizeof(short), 1, fd);
            fread(buffer, sizeof(char), size, fd);
            buffer[size] = '\0';
            printf(" Email : %s", buffer);
            fread(&size, sizeof(short), 1, fd);
            fread(buffer, sizeof(char), size, fd);
            buffer[size] = '\0';
            printf(" Phone : %s\n", buffer);
        }

    }

    free(path);

    fclose(fd);
}

int main() {
    int choice = 0;
    char user[64];

    user[0] = '0';

    while(choice < 5) {
        if(strcmp(user, "0") != 0)
            printf("Logged in as %s\n\n", user);

        printf("Select an option : \n");
        printf(" 1 : Create an account\n");
        printf(" 2 : Log In\n");
        printf(" 3 : Insert new card\n");
        printf(" 4 : View your cards\n");
        printf(" 5 : Log out\n");
        printf(" 6 : Close\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                info();
                break;
            case 2:
                if(strcmp(user, "0") != 0)
                    printf("You are already logged in\n");
                else
                    login(user);
                break;
            case 3:
                if(strcmp(user, "0") == 0)
                    printf("You aren't logged in\n");
                else
                    addCard(user);
                break;
            case 4:
                if(strcmp(user, "0") == 0)
                    printf("You aren't logged in\n");
                else
                    viewCards(user);
                break;
            case 5:
                user[0] = '0';
                user[1] = '\0';
                break;
            default:
                break;
        }
    }
    return 0;
}