#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#include <pthread.h>
#include <sys/time.h>

int time_up;
FILE *log_file;
int total=0;
float th_put=0;
float avg_rtt=0;
// user info struct
struct user_info {
  // user id
  int id;

  // socket info
  int portno;
  char *hostname;
  float think_time;

  // user metrics
  int total_count;
  float total_rtt;
};

// error handling function
void error(char *msg) {
  perror(msg);
  exit(0);
}

// time diff in seconds
float time_diff(struct timeval *t2, struct timeval *t1) {
  return (t2->tv_sec - t1->tv_sec) + (t2->tv_usec - t1->tv_usec) / 1e6;
}

// user thread function
void *user_function(void *arg) {
  /* get user info */
  struct user_info *info = (struct user_info *)arg;

  int sockfd, n;
  char buffer[256];
  struct timeval start, end;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  while (1) {
    /* start timer */
    gettimeofday(&start, NULL);

    /* TODO: create socket */

    /* TODO: set server attrs */

    /* TODO: connect to server */

    /* TODO: send message to server */

    /* TODO: read reply from server */

    /* TODO: close socket */

    /* end timer */
  //int portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  server =  gethostbyname(info->hostname);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(info->portno);

  /* connect to server */

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");
  else
  printf("Connected to server\n");
  
char buffer[1024]="GET index.html HTTP/1.1";



  /* send user message to server */

  n = write(sockfd, buffer, strlen(buffer));
  if (n < 0)
    error("ERROR writing to socket");
  bzero(buffer, 1024);
//printf("server response");
  /* read reply from server */
 n = read(sockfd, buffer, 255);
    //printf("buffer");
  
  if (n < 0)
    error("ERROR reading from socket");
  //printf("Server response: %s\n", buffer);
   total++;
  if(n==0)
  {
  break;
    printf("connection closed");

  }
  close(sockfd);

 


    gettimeofday(&end, NULL);

    /* if time up, break */
    if (time_up)
      break;

    /* TODO: update user metrics */
info->total_rtt=info->total_rtt+time_diff(&end,&start);
info->total_count++;
    /* TODO: sleep for think time */
  sleep(info->think_time);  
  }

  /* exit thread */
  fprintf(log_file, "User #%d finished\n", info->id);
  fflush(log_file);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int user_count, portno, test_duration;
  float think_time;
  char *hostname;

  if (argc != 6) {
    fprintf(stderr,
            "Usage: %s <hostname> <server port> <number of concurrent users> "
            "<think time (in s)> <test duration (in s)>\n",
            argv[0]);
    exit(0);
  }

  hostname = argv[1];
  portno = atoi(argv[2]);
  user_count = atoi(argv[3]);
  think_time = atof(argv[4]);
  test_duration = atoi(argv[5]);

  printf("Hostname: %s\n", hostname);
  printf("Port: %d\n", portno);
  printf("User Count: %d\n", user_count);
  printf("Think Time: %f s\n", think_time);
  printf("Test Duration: %d s\n", test_duration);

  /* open log file */
  log_file = fopen("load_gen.log", "w");

  pthread_t threads[user_count];
  struct user_info info[user_count];
  struct timeval start, end;

  /* start timer */
  gettimeofday(&start, NULL);
  time_up = 0;
  for (int i = 0; i < user_count; ++i) {
    /* TODO: initialize user info */
      info[i].id=i+1;
      info[i].hostname = hostname;
      info[i].portno= portno;
      info[i].think_time= think_time;
      info[i].total_count =0;
      info[i].total_rtt=0;


    /* TODO: create user thread */
      pthread_create(&threads[i], NULL,user_function, &info[i]);
    
    fprintf(log_file, "Created thread %d\n", i);
  }

  /* TODO: wait for test duration */
   time_t endwait;
    

    endwait = time (NULL) + test_duration ;
    while (time (NULL) < endwait)
    {
        
    }

  fprintf(log_file, "Woke up\n");

  /* end timer */
  time_up = 1; 
  gettimeofday(&end, NULL);

  /* TODO: wait for all threads to finish */
  for(int i=0;i<user_count;i++)
  {
         pthread_join(threads[i], NULL);
  }

  /* TODO: print results */
    for(int i=0;i<user_count;i++)
  {  avg_rtt+=(info[i].total_rtt/info[i].total_count);
     printf("total_count = %d , RTT = %f\n",info[i].total_count,(info[i].total_rtt)/info[i].total_count);

  }
    /* close log file */

    fclose(log_file);
  FILE *data_file;
  data_file = fopen("load_data.csv", "a");
  fprintf(log_file,"%d,%f,%f\n",atoi(argv[3]),(total/(float)atoi(argv[5])),(avg_rtt/(float)atoi(argv[3])));

  fclose(data_file);


  return 0;
}
