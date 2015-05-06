typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo lobbyConnection);

void parseChat(char* inc, int hops, int len);

