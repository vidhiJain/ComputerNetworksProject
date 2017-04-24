#include <stdlib.h>
#include <stdio.h>
#include <oping.h>
int main()
{
    while(1){

           int l= main1();
           if (l==-1) 
            {
               printf("Naya Server\n"); 
            }
            sleep(5);
    }
    return 0;

}
int main1(int argc, char **argv) {
    pingobj_t *ping;
    pingobj_iter_t *iter;

    if ((ping = ping_construct()) == NULL) {
        fprintf(stderr, "ping_construct failed\n");
        return (-1);
    }
    printf("ping_construct() success\n");

    if (ping_host_add(ping, "192.168.0.116") < 0) {
        const char * errmsg = ping_get_error(ping);
        fprintf(stderr, "ping_host_add(192.168.0.116) failed. %s\n", errmsg);
        return (-1);
    }
    printf("ping_host_add() success\n");

    
        if (ping_send(ping) < 0) {
            fprintf(stderr, "ping_send failed\n");
            return (-1);
        }
        printf("ping_send() success\n");

        for (iter = ping_iterator_get(ping); iter != NULL; iter =
                ping_iterator_next(iter)) {
            char hostname[100];
            double latency;
            unsigned int len;

            printf("ping_iterator_get() success\n");
            len = 100;
            ping_iterator_get_info(iter, PING_INFO_HOSTNAME, hostname, &len);
            len = sizeof(double);
            ping_iterator_get_info(iter, PING_INFO_LATENCY, &latency, &len);

            printf("hostname = %s, latency = %f\n", hostname, latency);
            if(latency==-1)return -1;
        }
        sleep(1);
    
    printf("exiting...\n");

    return (0);
}