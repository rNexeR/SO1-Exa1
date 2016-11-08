#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define PLAYER_SZ 6

typedef struct msgqbuf
{
  long mtype;
  char mssg[20];
} mbuf_t;

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    double scaled = (double)rand()/RAND_MAX;

    return (max - min +1)*scaled + min;
}

void receive_message(int msgqid, char * message){
	mbuf_t buf;
	if(msgrcv(msgqid, &buf, sizeof buf.mssg, 0, 0) == -1){
		perror("msgrcv");
		exit(1);
	}
	strcpy(message, buf.mssg);
}


void send_message(char* message, int* key, int msgq_id){
	mbuf_t msg_buf;
	msg_buf.mtype = 2;
	bzero(msg_buf.mssg, 20);
	//msg_buf.mssg = message;
	strcpy(msg_buf.mssg, message);
	int len = strlen(msg_buf.mssg);

	if (msgsnd(msgq_id, &msg_buf, len+1, 0) == -1){
		//perror("msgsnd");
		printf("Server closed the connection, exiting\n");
		exit(1);
	}
}

int create_msgq(int* key){
	int msgqid = msgget(*key, 0644 | IPC_CREAT);

	if(msgqid == -1) {
		perror("msgget");
		exit(1);
	}

	return msgqid;
}

int connect_msgq(int key){
	int msgqid;
	if ((msgqid = msgget(key, 0644)) == -1) { /* connect to the queue */
		perror("msgget");
		exit(1);
	}

	return msgqid;
}

void parse_message(char* message, char* player, char* value){
	int i = 0;
	for(i = 0; i < PLAYER_SZ; i++){
		player[i] = message[i];
		value[i] = message[i+PLAYER_SZ];
	}
}

void delete_msgq(int msgqid){
	//return;
	if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
	printf("msgq deleted %d\n", msgqid);
}