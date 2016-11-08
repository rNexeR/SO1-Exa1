#include "utils.h"

int send_msgqid, rec_msgqid;
int send_key = 95291, rec_key = 95292;

int max_number = 1000, min_number = 1;

void do_action(char*);
void send_numbers();
void receive_answer(int);

int main(int argc, char const *argv[])
{
	printf("Player started\n");
	send_msgqid = connect_msgq(send_key);
	rec_msgqid = connect_msgq(rec_key);

	printf("Player is communicating with server\n");
	char message[20];
	strcpy(message, "Player ready");
	send_message(message, &send_key, send_msgqid);
	printf("Player is waiting for instructions\n");
	while(1){
		bzero(message, 20);
		receive_message(rec_msgqid, message);
		printf("Message received: %s\n", message);
		do_action(message);
	}
	return 0;
}

void do_action(char* message){
	if(strcmp(message, "Let's Play 1") == 0){
		sleep(1);
		send_numbers();
	}else if(strcmp(message, "Let's Play 2") == 0){
		send_numbers();
	}
}

void send_numbers(){
	printf("I'm able to send numbers\n");
	int number;
	while(1){
		number = rand_interval(min_number, max_number);
		char n[4];
		sprintf(n, "%d", number);
		send_message(n, &send_key, send_msgqid);
		printf("Sended number: %d\n", number);
		receive_answer(number);
		sleep(1);
	}
	return;
}

void receive_answer(int number){
	char message[20];
	bzero(message, 20);
	receive_message(rec_msgqid, message);
	printf("Answer received: %s\n", message);
	if(strcmp(message, "ALTO") == 0){
		min_number = number;
		printf("Min number changed\n");
	}else{
		max_number = number;
		printf("Max number changed\n");
	}
}