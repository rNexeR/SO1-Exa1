#include "utils.h"

int rec_msgq_id, send_msgq_id;
int rec_key = 95291, send_key = 95292;

int players_connected;
int max_number = 1000;

int has_number_received;
int last_number_received;

void do_action(char*);
void play();
void receive_numbers();
void validate_number(int, int);
void salir();

int main(int argc, char const *argv[])
{
	players_connected = 0;
	has_number_received = 0;
	last_number_received = 0;
	printf("Server started\n");

	rec_msgq_id = create_msgq(&rec_key);
	send_msgq_id = create_msgq(&send_key);

	printf("Server running, waiting for players to start playing\n");

	char message[20];

	while(players_connected < 2){
		receive_message(rec_msgq_id, message);
		printf("Message received: %s\n", message);
		do_action(message);
	}

	return 0;
}

void do_action(char* message){
	if(strcmp(message, "Player ready") == 0){
		players_connected++;
		printf("Players connected: %d\n", players_connected);
		if(players_connected == 2)
			play();
	}
}

void play(){
	char message[20];
	strcpy(message, "Let's Play 1");
	send_message(message, &send_key, send_msgq_id);
	strcpy(message, "Let's Play 2");
	send_message(message, &send_key, send_msgq_id);
	sleep(2);

	receive_numbers();
}

void receive_numbers(){
	int search_number;
	for (int i = 0; i < 50; ++i)
	{
		search_number = rand_interval(1, max_number);
	}
	
	printf("Number: %d\n", search_number);

	while(1){
		char message[20];
		receive_message(rec_msgq_id, message);
		printf("Number received: %s\n", message);
		validate_number(search_number, atoi(message));
	}
}

void validate_number(int search_number, int sended_number){
	has_number_received = 1;
	if(has_number_received == 1){
		char message[20];
		if(last_number_received < search_number)
			strcpy(message, "ALTO");
		else{
			strcpy(message, "BAJO");
		}
		send_message(message, &send_key, send_msgq_id);
		printf("After receive number I send: %s\n", message);
	}
	if(search_number == sended_number){
		printf("Number was found, sended number: %d\n", sended_number);
		salir();
	}
}

void salir(){
	delete_msgq(rec_msgq_id);
	delete_msgq(send_msgq_id);
	exit(0);
}