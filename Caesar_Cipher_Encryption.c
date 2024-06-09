#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define MAX_STRING 10000

char cipher(char ch, int choice)
{
    if (choice == 1)
    { // Encryption
        if (ch >= 'a' && ch <= 'z')
        {
            ch = 'a' + (ch - 'a' + 3) % 26;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = 'A' + (ch - 'A' + 3) % 26;
        }
    }
    else if (choice == 2)
    { // Decryption
        if (ch >= 'a' && ch <= 'z')
        {
            ch = 'a' + (ch - 'a' + 23) % 26;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = 'A' + (ch - 'A' + 23) % 26;
        }
    }
    return ch;
}

int main(int argc, char *argv[])
{
    int my_rank, p;
    char input_string[MAX_STRING];
    int choice;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        int user_input;
        printf("Enter 1 to input from console or 2 to input from file: ");
        fflush(stdout);
        scanf("%d", &user_input);
        if (user_input == 1)
        {
            printf("Enter the string to encrypt/decrypt: ");
            fflush(stdout);
            scanf("%s", input_string);
        }
        else if (user_input == 2)
        {
            char file_name[300];
            printf("Enter the file name: ");
            fflush(stdout);
            scanf("%s", file_name);
            FILE *file;
            file = fopen(file_name, "r");
            if (file == NULL)
            {
                printf("File not found\n");
                return 1;
            }
            fscanf(file, "%s", input_string);
            fclose(file);
        }
        else
        {
            printf("Invalid input\n");
            return 1;
        }
        printf("Enter 1 to encrypt or 2 to decrypt: ");
        fflush(stdout);
        scanf("%d", &choice);
        int len = strlen(input_string);
        int portion = len / p;
        for (int i = 1; i < p; i++)
        {
            MPI_Send(&choice, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&portion, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(input_string + i * portion, portion, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < portion; i++)
        {
            input_string[i] = cipher(input_string[i], choice);
        }
        for (int i = p * portion; i < len; i++)
        {
            input_string[i] = cipher(input_string[i], choice);
        }
        for (int i = 1; i < p; i++)
        {
            MPI_Recv(input_string + i * portion, portion, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("The encrypted/decrypted string is: %s\n", input_string);
    }
    else
    {
        MPI_Recv(&choice, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int portion;
        MPI_Recv(&portion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        char recv_string[portion];
        MPI_Recv(recv_string, portion, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < portion; i++)
        {
            recv_string[i] = cipher(recv_string[i], choice);
        }
        MPI_Send(recv_string, portion, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}