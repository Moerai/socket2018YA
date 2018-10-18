#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 9000
 
char buffer[BUFSIZ] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZ], *ptr, *ptr2;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
		
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			
			if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")) == 0){
				strcpy(buffer, "안녕하세요 만나서. 반가워요.");
			} else if(strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야"))==0){
				strcpy(buffer, "준원봇 : 제 이름은 전준원입니다. 주인님.");
			} else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야"))==0){
				strcpy(buffer, "저는 21살이고 1998년 10월 27일에 태어났어요.");
			} else{
				strcpy(buffer, "제가 답변할 수 없는 질문이예요. 저는 멍청하답니다.");
			}

			if(strncmp(rcvBuffer, "strlen ", 7) == 0){
				n = strlen(rcvBuffer)-7;
				sprintf(buffer, "제가 받은 문자는 %d 글자입니다.", n);
			} else if(strncmp(rcvBuffer, "strcmp ", 7) == 0){
				ptr = strtok(rcvBuffer, " ");

				ptr = strtok(NULL, " ");
				ptr2 = strtok(NULL, " ");

				if(strcmp(ptr, ptr2) == 0){
				    	
					strcpy(buffer, "서로 일치합니다.");
				} else {
				
					strcpy(buffer, "서로 일치하지 않습니다.");
				}
			}

			write(c_socket, buffer, strlen(buffer));
		}

		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
