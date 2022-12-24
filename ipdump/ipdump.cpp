//协议函数类型定义AF—inet
#include<netinet/in.h>
//地址转换函数 inet_addr()等
#include<arpa/inet.h>
//socket 
#include<sys/socket.h>
#include<sys/types.h>

//标准输入输出
#include<cstdio>
#include<cstdlib>
//提供memset等字符串操作函数
#include<cstring>

//read write execl
#include<unistd.h>

//gethostbyname
#include<netdb.h>

#include<net/ethernet.h>
#include<fstream>

//timeval
#include<sys/time.h>
#include<netinet/if_ether.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<netinet/ip_icmp.h>
#include<netinet/in_systm.h>
#include<netinet/ip.h>
#include<netinet/in.h>

#include<time.h>

#include<iostream>

#include<pthread.h>

// #include<mysql/mysql.h>

#define MAXZSIZE 4096
#define OPTNUM 8
using namespace std;

#define ON 1
#define OFF 0


int recvfd; //接受报文时使用的描述符
struct sockaddr localhost;  //服务器的地址
char ifname[256]="ens33";
char buff[MAXZSIZE];  //接受数据缓冲区

enum{ETHER,ARP,IP,TCP,UDP,ICMP,DUMP,ALL};
char probuff[20];


struct ether_header* eth; //报文报头结构体
struct ether_arp* arp;//arp包的结构体
struct ip* ip;
struct icmp* icmp;
struct tcphdr* tcp;
struct udphdr* udp;
int len; //接收到数据的长度
char* p;
//用于计数的全局变量
//广播帧
int mac_broad;
//网络超短帧<64
int mac_short;
//网络超长帧>1518
int mac_long;
//总的网络帧的字节
int mac_byte;
//网络数据包总数
int mac_packet;
//每秒传输的bit数
int mac_bps;
//每秒传输的字节数
int byteSpeed;
//每秒平均传输的包的数量
int packetSpeed;
//ip关闭数据包
int ip_broadcast;
//ip包总的字节数
int ip_byte;
//数据包的总数
int ip_packet=0;
//udp数据包数量
int udp_packet;
int tcp_packet;
//icmp数据包数量
int icmp_packet;
//icmp重定位
int icmp_redirect;
//icmp目的不可达数据包
int icmp_destination;

void print_ethernet(struct ether_header * eth);
void print_arp(struct ether_arp *arp);
void print_ip(struct ip * ip);
void print_icmp(struct icmp* icmp);
void print_tcp(struct tcphdr *tcp);
void print_udp(struct udphdr* udp);

#if 0
void dis_pro(u_char* pro);
//传入对应的port指定源目地址
void dis_port(u_int port,bool is_destPort,time_t start_time,int total_time);
void dis_ipaddr(const char* ipaddr,bool is_destAddr);
#endif

//将mac地址转换为字符串
char* mac_ntoa(u_char* d);
//在FreeBSD系统中 打开bpf
void open_bpf();
//将tos地址转换为字符串
char* ip_ttoa(int flag);
//将flag的值转换为字符串
char* ip_ftoa(int flag);
//将control flag的值转换为字符串
char* tcp_ftoa();
//显示程序的帮助说明
void help(char* cmd);
//将Ethernet报头的初始地址到FCS之前的值使用十六进制整数和ASCII码来表示
void Dump_packet();
//显示一段时间内的统计结果
void Results_panel(char* start,char* end,double cost_time);

//检查校验和函数
u_short checksum(u_short *data,int len);
//创建TCP头部内容
void make_tcp_header(struct packet_tcp * packet,int src_ip,int src_port,int dst_ip,int dst_port,int seq,int ack,int datalen);
//创建IP头部内容
void make_ip_header(struct ip* ip,int src_ip,int dst_ip,int iplen);
//构造问题数据包
void send_syn(char* dst_ip_char,char* src_ip_char,int dport,int sport,int seq_1);
void create_packet();


bool pro_flag=false;
bool ipaddr_flag=false;
bool port_flag=false;
bool is_dport=false;
bool is_daddr=false;
bool create_flag=false;

int opt[OPTNUM];
int dport;
int sport;
u_char s_addr[15];
u_char d_addr[15];

//构造数据包使用的全局变量
char dst_ip_char[15];
char src_ip_char[15];
int syn_dport=0;
int syn_sport=0;
int syn_seq_1=0;
// void *get_thread(void *arg);
// void *timer_thread(void *arg);

// void *get_thread(void *arg)
// {

// }
// void *timer_thread(void *arg)
// {

// }

//time为需要统计的时间段
void* create_Socket(int pro_type,time_t time1,int total_time)
{

    printf("start\n");
    printf("源端口：%d\n",sport);
    printf("目的端口：%d\n",dport);
    printf("源地址：%s\n",s_addr);
    printf("目的地址：%s\n",d_addr);
    printf("统计时间：%d\n",total_time);
     //使用socket系统调用打开套接字
    // if(recvfd=socket(PF_INET,SOCK_STREAM,htons(ETH_P_ALL))<0)
    if((recvfd=socket(PF_INET,SOCK_PACKET,htons(ETH_P_ALL)))<0)
    {
        perror("create socket error");
        exit(EXIT_FAILURE);
    }
  
    memset((char*)&localhost,0,sizeof(localhost));
    //设置本机ip地址 INADDR_ANY表示任意的ip
    localhost.sa_family=PF_INET;
    strcpy(localhost.sa_data,ifname);

    if(bind(recvfd,&localhost,sizeof(localhost))<0)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    //循环接受数据包进行筛选
    while(1)
    {
        
        if(difftime(time(NULL),time1)>=total_time){return (void*) 1;}
        
        memset(buff,'!',4096);
        printf("%S",buff);


        if((len=read(recvfd,buff,MAXZSIZE))<0)
        {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        //设置eth报文的初始指针
        p=buff;

        int mac_length=0;
        for(int i=0;i<4095;i++)
        {
            if(p[i]!='!')
            {
                mac_length++;
            }
            else break;
        }
        
        mac_byte+=mac_length;
        // printf("包的长度为：%d",mac_length);
        
        eth=(struct ether_header*)p;
        p=p+sizeof(struct ether_header);

        //判断是否为超长数据帧
        if(mac_length>1518)
        {
            mac_long++;
        }else if(mac_length<60)
        {
            mac_short++;
        }

        mac_packet++;

        if(ntohs(eth->ether_type)==ETHERTYPE_ARP)
        {
            if(opt[ARP])
            {                
                    printf("arp包\n");
                    //显示arp内容
                    arp=(struct ether_arp*)p;
                    print_arp(arp);             
            }    

        }
        //ip数据包的处理程序
        else if(ntohs(eth->ether_type)==ETHERTYPE_IP)
        {
            if(opt[IP])
            {    
                ip=(struct ip*)p;
                //ip数据包总字节数增加
                ip_byte+=ip->ip_len;
                // printf("ip数据包的总长度为%d字节",ip->ip_len);
                switch (ip->ip_p)
                {     
                case IPPROTO_TCP:
                    /* code */
                    if(opt[TCP])
                    {
                        if(!ipaddr_flag || ((is_daddr && (!strcmp((const char*)d_addr,inet_ntoa(ip->ip_dst)))) || 
                         ((!is_daddr) && (!strcmp((const char*)s_addr,inet_ntoa(ip->ip_src))))))
                        {
                            p=p+((int)(ip->ip_hl)<<2);
                            tcp=(struct tcphdr*)p;
                            int port=-1;
                            if(is_dport) port=dport;
                            else port=sport;
                            if(!port_flag || ((is_dport && (ntohs(tcp->dest)==port)) || ((!is_dport) && (ntohs(tcp->source)==port))))
                            {
                                print_ethernet(eth);
                                //显示ip内容
                                print_ip(ip);
                                printf("ip 包\n");    
                                printf("传输层协议%d",ip->ip_p);
                                p=p+((int)(tcp->doff)<<2);
                                printf("tcp 包\n");
                                tcp_packet++;
                                print_tcp(tcp);
                            }
                        }
                    }
                    break;
                case IPPROTO_UDP:
                    if(opt[UDP])
                    {
                        if(!ipaddr_flag || ((is_daddr && (!strcmp((const char*)d_addr,inet_ntoa(ip->ip_dst)))) || 
                         ((!is_daddr) && (!strcmp((const char*)s_addr,inet_ntoa(ip->ip_src))))))
                        {
                            
                            p=p+((int)(ip->ip_hl)<<2);
                            udp=(struct udphdr*)p;
                            int port=-1;
                            if(is_dport) port=dport;
                            else port=sport;
                            if(!port_flag || ((is_dport && (ntohs(udp->dest)==port)) || ((!is_dport) && (ntohs(udp->source)==port))))
                            {
                                print_ethernet(eth);
                                //显示ip内容
                                print_ip(ip);
                                printf("ip 包\n");    
                                printf("传输层协议%d\n",ip->ip_p);
                                p=p+sizeof(struct udphdr);
                                printf("udp 包\n");
                                //显示UDP的内容
                                print_udp(udp);
                                udp_packet++;
                            }
                        }
                    }
                    break;
                case IPPROTO_ICMP:
                    if(opt[ICMP])
                    {
                        if(!ipaddr_flag || ((is_daddr && (!strcmp((const char*)d_addr,inet_ntoa(ip->ip_dst)))) || 
                         ((!is_daddr) && (!strcmp((const char*)s_addr,inet_ntoa(ip->ip_src))))))
                        {
                            print_ethernet(eth);
                            p=p+((int)(ip->ip_hl)<<2);
                            //显示ip内容
                            print_ip(ip);
                            printf("ip 包\n");    
                            printf("传输层协议%d\n",ip->ip_p);
                            icmp=(struct icmp*)p;
                            p=p+sizeof(struct udphdr);
                            //显示ICMP的内容
                            print_icmp(icmp);
                            printf("icmp 包\n");
                            icmp_packet++;
                        }
                    }
                    break;
                default:
                        print_ethernet(eth);
                        p=p+((int)(ip->ip_hl)<<2);
                        //显示ip内容
                        print_ip(ip);
                        printf("ip 包\n");    
                        printf("传输层协议%d\n",ip->ip_p);
                        //显示eth内容
                        print_ethernet(eth);
                        printf("unkown");
                        break;
                }
            }
            ip_packet++;
        }        
    }
}

int main(int argc,char** argv)
{
    opt[ETHER]=OFF;
    opt[ARP]=ON;
    opt[IP]=ON;
    opt[TCP]=ON;
    opt[UDP]=ON;
    opt[ICMP]=ON;
    opt[DUMP]=OFF;
    opt[ALL]=OFF;

    int c;
    int total_time;
    while((c=getopt(argc,argv,"aei:p:t::d:s:hl"))!=EOF)
    {
        switch(c)
        {
            case 'a':
                opt[ALL]=ON;
                break;
            case 'i':
                strcpy(ifname,optarg);
                break;
            case 'e':
                opt[ETHER]=ON;
                break;
            case 'd':
                opt[DUMP]=ON;
                optind--;
                while(argv[optind]!=NULL&&argv[optind][0]!='-')
                {
                    if(memcmp(argv[optind],"port",4)==0)
                    {
                        port_flag=true;
                        is_dport=true;
                        printf("目的端口过滤");
                        sscanf(&argv[optind][4],"%d",&dport);
                        printf("%d",dport);
                    }
                    else if(memcmp(argv[optind],"addr",4)==0)
                    {
                        ipaddr_flag=true;
                        is_daddr=true;
                        printf("目的地址过滤");
                        printf("%s",&argv[optind][4]);
                        sscanf(&argv[optind][4],"%s",&d_addr);
                        
                    }
                    else 
                    {
                        help(argv[0]);
                        exit(EXIT_FAILURE);
                    }  
                    optind++;
                }
                break;
            case 's':
                optind--;
                while(argv[optind]!=NULL&&argv[optind][0]!='-')
                {
                    if(memcmp(argv[optind],"port",4)==0)
                    {
                        port_flag=true;
                        is_dport=false;
                        printf("源端口过滤");
                        sscanf((char*)&argv[optind][4],"%d",&sport);
                        printf("%d",sport);
                    }
                    else if(memcmp(argv[optind],"addr",4)==0)
                    {
                        ipaddr_flag=true;
                        is_daddr=false;
                        printf("目的地址过滤");
                        // printf("%s",&argv[optind][4]);
                        sscanf((char*)&argv[optind][4],"%s",&s_addr);
                        printf("%s",s_addr);
                    }
                    else 
                    {
                        help(argv[0]);
                        exit(EXIT_FAILURE);
                    }  
                    optind++;
                }
                break;
            case 'p':
                opt[ARP]=OFF;
                // opt[IP]=OFF;
                opt[TCP]=OFF;
                opt[UDP]=OFF;
                opt[ICMP]=OFF;
                optind--;
                while(argv[optind]!=NULL&&argv[optind][0]!='-')
                {
                    if(strcmp(argv[optind],"arp")==0)
                    {
                        opt[ARP]=ON;
                        printf("arp过滤");
                    }
                    else if(strcmp(argv[optind],"ip")==0)
                    {
                        opt[IP]=ON;
                        printf("ip过滤");
                    }
                    else if(strcmp(argv[optind],"tcp")==0)
                    {
                        opt[TCP]=ON;
                        printf("tcp过滤");
                    } 
                    else if(strcmp(argv[optind],"udp")==0)
                    {
                        opt[UDP]=ON;
                        printf("udp过滤");
                    }
                    else if(strcmp(argv[optind],"icmp")==0)
                    {
                        opt[ICMP]=ON;
                        printf("icmp过滤");
                    } 
                    else if(strcmp(argv[optind],"other")==0);
                    else 
                    {
                        help(argv[0]);
                        exit(EXIT_FAILURE);
                    }  
                    optind++;
                }
                break;
            case 't':
                sscanf(optarg,"%d",&total_time);
                printf("%d",total_time);
                break;
            case 'l':
                create_packet();
                create_flag=true;
                break;
            case 'h':
            case '?':
            default:
                help(argv[0]);
                exit(EXIT_FAILURE);
                break;

        }
    }


    if(optind<argc)
    {
        while(optind<argc)
            printf("%s",argv[optind+1]);
        printf("\n");
        help(argv[0]);
        exit(EXIT_FAILURE);
    }

    if(!create_flag)
    {
        double cost_time;
    char str[30]; 
    char str1[30]; 
    time_t now_time;
    //获取开始时的时间
    struct tm *start_local_time;
    time_t  start_t;
    start_t=time(NULL); 
    start_local_time=localtime(&start_t);

    //捕获数据包
    create_Socket(0,start_t,total_time);

    //获取结束时的时间
    struct tm *end_local_time;
    time_t  end_t;
    end_t=time(NULL);
    end_local_time=localtime(&end_t); 

    //进行结果的统计
    strftime(str,100,"%T %D",start_local_time);  
    printf(str);
    strftime(str1,100,"%T %D",end_local_time);  
    printf(str1);  
    cost_time=difftime(end_t,start_t);
    printf("%f\n",cost_time);
    Results_panel(str,str1,cost_time);
    }
    return NULL;
}

char* mac_ntoa(u_char* d)
{
    static char str[50];
    sprintf(str,"%02x:%02x:%02x:%02x:%02x:%02x",d[0],d[1],d[2],d[3],d[4],d[5]);
    return str;
}
//将tos地址转换字符串
char* ip_ttoa(int flag)
{
    static int f[]={'l','l','l','D','T','R','C','X'};

    static char str[17];

    u_int mask=0x80;
    
    int i;

    for(i=0;i<8;i++)
    {
        if(((flag<<i)&mask)!=0)
            str[i]=f[i];
        else 
            str[i]='0';
    }

    str[i]='\0';

    return str;
}
//将flag的值转换为字符串 
char* ip_ftoa(int flag)
{
    static int f[]={'R','D','M'};
    static char str[17];

    u_int mask=0x8000;

    int i;

    for(i=0;i<3;i++)
    {
        if(((flag<<i)&mask)!=0)
            str[i]=f[i];
        else 
            str[i]='0';
    }

    str[i]='\0';

    return str;
}
//将control flag的值转换为字符串
char* tcp_ftoa()
{
    static int f[]={'U','A','P','R','S','F'};

    static char str[17];
    
}
//显示程序的帮助说明
void help(char* cmd)
{
    fprintf(stderr,"usage:%s[-aedh][-i ifname][-p protocols][-ttimeval][-l][-d port|addr][-s port|addr]\n",cmd);
    fprintf(stderr," protocols:arp ip icmp tcp udp other\n");
#ifdef linux
    fprintf(stderr,"default:%s -p arp ip icmp tcp udp\n",cmd);
#endif   

}
//将Ethernet报头的初始地址到FCS之前的值使用十六进制整数和ASCII码来表示
void Dump_packet(unsigned char * buff,int len)
{
    int i,j;
    printf("Trame Dump:\n");
    for(i=0;i<len;i+=16)
    {
        for(j=i;j<i+16&&j<len;j++)
        {
            printf("%02x",buff[j]);
        }
        if(j%2==1)
            printf("    ");
    

    if(j==len&&len%16!=0)
        for(j=0;j<40-(len%16)*2;j++)
        {
            printf("    ");
        }
    printf(":");

    for(j=1;j<i+16&&j<len;j++)
    {
        if((buff[j]>=0x20)&&(buff[j]<=0x7e))
            putchar(buff[j]);
        else
            printf(" . ");
    }

    printf("\n");
    }
    fflush(stdout);
}
/*显示ethernet的数据包的内容*/
void print_ethernet(struct ether_header * eth)
{
    eth->ether_dhost;
    eth->ether_shost;
    //获取以太网类型
    int type=ntohs(eth->ether_type);
    //1500？？？？十六进制为0x05DC 
    if(type<=1500)
    {
        printf("IEEE 802.3 Ethernet Frame:\n");
    }
    else
    {
        printf("Ethernet Frame:\n");
    }


    printf("----------------------------------------------------------------------------\n");
    printf("Destination MAC Address:%17s\n",mac_ntoa(eth->ether_dhost));
    printf("----------------------------------------------------------------------------\n");

    printf("----------------------------------------------------------------------------\n");
    printf("Soruce MAC Address:%17s\n",mac_ntoa(eth->ether_shost));
    printf("----------------------------------------------------------------------------\n");

    //网元发现功能
    std::ofstream outfile;
    outfile.open("macs.txt",ios::out|ios::app);
    if(!outfile.is_open()){ printf("open file failed!");}
    else{  
        outfile<<mac_ntoa(eth->ether_dhost);
        outfile<<std::endl;
        outfile<<mac_ntoa(eth->ether_shost);
        outfile<<std::endl;
    }

    if(memcmp(mac_ntoa(eth->ether_dhost),"FF:FF:FF:FF:FF:FF",17)==0)
    {
        mac_broad++;
    }
    if(type<1500)
    {
        printf("Length:%5u\n",type);
    }
    else
    {
        printf("Ethernet Type 0x%04x\n",type);
    }

    printf("----------------------------------------------------------------------------\n");
}
/*显示ARP包的内容*/
void print_arp(struct ether_arp *arp)
{
    static char* arp_operation[]={"Undefine","(ARP Request)","(ARP Reply)","(RARP Request)","(RARP Reply)"};
    int opt=ntohs(arp->ea_hdr.ar_op);
    //可选域种存储的是arp请求的处理
    if(opt<1|| opt>5)
    {
        opt=0;
    }

    printf("protocol:ARP \n");
    printf("----------------------------------------------------------------------------\n");
    printf("Format of hardware address:%2u%-11s Format of protocol address:0x%04x%-9s|\n",
        ntohs(arp->ea_hdr.ar_hrd),(ntohs(arp->ea_hdr.ar_hrd)==ARPHRD_ETHER)?"(Ethernet)":"(notether)",
        ntohs(arp->ea_hdr.ar_pro),(ntohs(arp->ea_hdr.ar_pro)==ARPHRD_ETHER)?"(IP)":"(NOT IP)"
    );
    printf("----------------------------------------------------------------------------\n");
    
    printf("hard_len:%3u  addr len %2u  op:%4d %16s \n",
        arp->ea_hdr.ar_hln,arp->ea_hdr.ar_pln,ntohs(arp->ea_hdr.ar_op),arp_operation[opt]);
    
    printf("----------------------------------------------------------------------------\n");

    printf("Source MAC Address:%17s\n",mac_ntoa(arp->arp_sha));

    printf("----------------------------------------------------------------------------\n");

    printf("Source IP Address:%15s\n",inet_ntoa(*(struct in_addr *) & arp->arp_spa));

    printf("----------------------------------------------------------------------------\n");

    printf("Source MAC Address:%17s\n",mac_ntoa(arp->arp_tha));

    printf("----------------------------------------------------------------------------\n");

    printf("Source IP Address:%15s\n",inet_ntoa(*(struct in_addr *) & arp->arp_tpa));

    printf("----------------------------------------------------------------------------\n"); 
}
/* 显示IP报头的详细信息 */
void print_ip(struct ip * ip)
{
    printf("PROTOCOL: IP\n");
    printf("----------------------------------------------------------------------------\n");

    printf("IV:%1u HL:%2u T:%8s Total Length:%10u\n",ip->ip_v,ip->ip_hl,ip_ttoa(ip->ip_tos),ntohs(ip->ip_len));

    printf("----------------------------------------------------------------------------\n");
    printf("Identifier:%5u FO:%5u\n",ntohs(ip->ip_id),ntohs(ip->ip_off)&IP_OFFMASK);
    printf("----------------------------------------------------------------------------\n");
    printf("Source IP Address:%15s\n",inet_ntoa(*(struct in_addr*) & (ip->ip_src)));

    printf("----------------------------------------------------------------------------\n");
    printf("Destination IP Address:%15s\n",inet_ntoa(*(struct in_addr*) & (ip->ip_dst)));
    printf("----------------------------------------------------------------------------\n");

    if(memcmp(inet_ntoa(*(struct in_addr*) & (ip->ip_dst)),"255.255.255.255",15)==0)
    {
        ip_broadcast++;
    }
    
}
/* 显示ICMP报头的详细信息 */
void print_icmp(struct icmp* icmp)
{
    static char* type_name[]={
        "Echo Reply",
        "Undefine",
        "Undefine",
        "Destination Unreachable",
        "SourceQuench",
        "Redirect(change route)",
        "Undefine",
        "Undefine",
        "Echo Request",
        "Undefine",
        "Undefine",
        "Time Exceeded",
        "Parameter Problem",
        "Timestamp Request",
        "Timestamp Reply",
        "Information Request",
        "Information Reply",
        "Address Mask Request",
        "Address Mask Reply",
        "Unknown"
    };

    
    int type= icmp->icmp_type;
    if(type<0 || type>18)
    {
        type=19;
    }

    // printf("Protocol:ICMP(%S)\n",type_name[type]);

    printf("Protocol:ICMP\n");
    printf("----------------------------------------------------------------------------\n");

    printf("Type:%3u Code:%3u CheckSum:%5u\n",icmp->icmp_type,icmp->icmp_code,ntohs(icmp->icmp_cksum));

    printf("----------------------------------------------------------------------------\n");

    if(icmp->icmp_type==0 || icmp->icmp_type==8)
    {
        printf("identification:%5u Sequence Number:%5u\n",ntohs(icmp->icmp_id),ntohs(icmp->icmp_seq));
    }
    else if(icmp->icmp_type==3)
    {
        if(icmp->icmp_code==4)
        {
            printf("void:%5u Next MTU:%5u \n",ntohs(icmp->icmp_pmvoid),ntohs(icmp->icmp_nextmtu));
            printf("----------------------------------------------------------------------------\n");
        }
        else
        {   
            //type=3 code=1 主机不可达
            icmp_destination++; 
            printf("Unused:%10lu \n",(u_long)ntohl(icmp->icmp_void));
            printf("----------------------------------------------------------------------------\n");
        }
    }
    else if(icmp->icmp_type==5)
    {
        printf("Router IP Address:%15s \n",inet_ntoa(*(struct in_addr*)&(icmp->icmp_gwaddr)));
        printf("----------------------------------------------------------------------------\n");
        icmp_redirect++;
        //对主机的地址重定向 type=5 code=1
    }
    else if(icmp->icmp_type==11)
    {
        printf("Unused:%10lu \n",(u_long)ntohl(icmp->icmp_void));
        printf("----------------------------------------------------------------------------\n");
    }

    if(icmp->icmp_type==3||icmp->icmp_type==5||icmp->icmp_type==11)
        print_ip((struct ip*)(((char*)icmp)+8));

    printf("----------------------------------------------------------------------------\n");
}
/* 显示TCP报头的信息 */
void print_tcp(struct tcphdr *tcp)
{
            printf("PRTTOCOL:TCP\n");
            printf("----------------------------------------------------------------------------\n");
            printf("Source Port:%5u  Destination Port:%5u\n",ntohs(tcp->source),ntohs(tcp->dest));
            printf("----------------------------------------------------------------------------\n");
            printf("Sequence Number:%10lu\n",(u_long)ntohl(tcp->seq));
            printf("----------------------------------------------------------------------------\n");
            printf("Acknowledgement Number:%10lu\n",(u_long)ntohl(tcp->ack_seq));
            printf("----------------------------------------------------------------------------\n");
            printf("Do:%5u Reserved:%5u Window Size:%5u\n",ntohs(tcp->doff),ntohs(tcp->ack),ntohs(tcp->window));
            printf("----------------------------------------------------------------------------\n");
            printf("CheckSum:%5u Urgent Pointer:%5u\n",ntohs(tcp->check),ntohs(tcp->urg_ptr));
            printf("----------------------------------------------------------------------------\n");
    
}
/* 显示UDP报头的数据 */
void print_udp(struct udphdr* udp)
{
    printf("PROTOCOL:UDP\n");
    printf("----------------------------------------------------------------------------\n");
    printf("Source Port:%5u Dest Port:%5u\n",ntohs(udp->source),ntohs(udp->dest));
    printf("----------------------------------------------------------------------------\n");
    printf("Length:%5u CheckSum:%5u\n",ntohs(udp->len),ntohs(udp->check));
    printf("----------------------------------------------------------------------------\n");
}
// //设置协议过滤条件
// void dis_pro(u_char* pro)
// {
//     int pro_type;
//     if(!memcmp(pro,"arp",3))
//     {
//         pro_type=ETHERTYPE_ARP;
//     }
//     else if(!memcmp(pro,"ip",2))
//     {
//         pro_type=ETHERTYPE_IP;
//     }
//     else if(!memcmp(pro,"icmp",4))
//     {
//         pro_type=IPPROTO_ICMP;
//     }
//     else if(!memcmp(pro,"tcp",3))
//     {
//         pro_type=IPPROTO_TCP;
//     }
//     else if(!memcmp(pro,"udp",3))
//     {
//         pro_type=IPPROTO_UDP;
//     }

//     std::cout<<pro_type;
//     std::cout<<pro_flag;
//     std::cout<<(pro_type==ETHERTYPE_ARP);

//     // create_Socket(pro_type,0,0,"","");
// }
// ////设置端口过滤
// void dis_port(u_int port,bool is_destPort,time_t start_time,int total_time)
// {
//      port_flag=true;
//      if(is_destPort)
//      {
//         create_Socket(0,port,0,"","",start_time,total_time);
//      }
//      else
//      {
//         create_Socket(0,0,port,"","",start_time,total_time);
//      }  
// }
// //设置ip地址过滤
// void dis_ipaddr(const char* ipaddr,bool is_destAddr)
// {
//     ipaddr_flag=true;
//     if(is_destAddr)
//     {
//         // create_Socket(0,0,0,ipaddr,"");
//     }
//     else
//     {
//         // create_Socket(0,0,0,"",ipaddr);
//     }
// }

//创建数据包发送  
void create_packet()
{
    printf("指定构造的数据包的具体的字段：\n");
    printf("源地址（使用点分十进制进行表示）:\n");
    scanf("%s",&src_ip_char);
    printf("目的地址（使用点分十进制进行表示）:\n");
    scanf("%s",&dst_ip_char);
    printf("源端口:\n");
    scanf("%d",&syn_sport);
    printf("目的端口:\n");
    scanf("%d",&syn_dport);
    printf("seq");
    scanf("%d",&syn_seq_1);
    send_syn(dst_ip_char,src_ip_char,syn_dport,syn_sport,syn_seq_1);
}
//显示统计结果
void Results_panel(char* start,char* end,double cost_time)
{
    // //广播帧
    // int mac_broad=1126;
    // //网络超短帧<64
    // int mac_short=2141;
    // //网络超长帧>1518
    // int mac_long=0;
    // //总的网络帧的字节
    // int mac_byte=791190;
    // //网络数据包总数
    // int mac_packet=4946;
    // //每秒传输的bit数
    // int mac_bps=(mac_byte*8)/cost_time;
    // //每秒传输的字节数
    // int byteSpeed=mac_bps/8;
    // //每秒平均传输的包的数量
    // int packetSpeed=mac_packet/cost_time;
    // //ip关闭数据包
    // int ip_broadcast=406;
    // //ip包总的字节数
    // int ip_byte=501040;
    // //数据包的总数
    // int ip_packet=3402;
    // //udp数据包数量
    // int udp_packet=514;
    // int tcp_packet=0;
    // //icmp数据包数量
    // int icmp_packet=924;
    // //icmp重定位
    // int icmp_redirect=0;
    // //icmp目的不可达数据包
    // int icmp_destination=0;
    mac_bps=(mac_byte*8)/cost_time;
    //每秒传输的字节数
    byteSpeed=mac_bps/8;
    //每秒平均传输的包的数量
    packetSpeed=mac_packet/cost_time;
    printf("+-------------------+------------------------------+\n");
    printf("|Variable           |Values                        |\n");
    printf("+-------------------+------------------------------+\n");
    printf("|StartTime          |%30s|\n",start);
    printf("+-------------------+------------------------------+\n");
    printf("|End  Time          |%30s|\n",end);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC Broad          |%30d|\n",mac_broad);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC Short          |%30d|\n",mac_short);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC Long           |%30d|\n",mac_long);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC Byte           |%30d|\n",mac_byte);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC Packet         |%30d|\n",mac_packet);
    printf("+-------------------+------------------------------+\n");
    printf("|Bit/s              |%30d|\n",mac_bps);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC ByteSpeed      |%30d|\n",byteSpeed);
    printf("+-------------------+------------------------------+\n");
    printf("|MAC PacketSpeed    |%30d|\n",packetSpeed);
    printf("+-------------------+------------------------------+\n");
    printf("|IP  BroadCast      |%30d|\n",ip_broadcast);
    printf("+-------------------+------------------------------+\n");
    printf("|IP  Byte           |%30d|\n",ip_byte);
    printf("+-------------------+------------------------------+\n");
    printf("|IP  Packet         |%30d|\n",ip_packet);
    printf("+-------------------+------------------------------+\n");
    printf("|UDP Packet         |%30d|\n",udp_packet);
    printf("+-------------------+------------------------------+\n");
    printf("|TCP Packet         |%30d|\n",tcp_packet);
    printf("+-------------------+------------------------------+\n");
    printf("|ICMPPacket         |%30d|\n",icmp_packet);
    printf("+-------------------+------------------------------+\n");
    printf("|ICMPRedirect       |%30d|\n",icmp_redirect);
    printf("+-------------------+------------------------------+\n");
    printf("|ICMPDestination    |%30d|\n",icmp_destination);
    printf("+-------------------+------------------------------+\n");
}

struct packet_tcp
{
    struct ip ip;
    struct tcphdr tcp;
    u_char data[MAXZSIZE];
};

u_short checksum(u_short *data,int len)
{
    u_long sum=0;
    //short为两个字节:: 16bit
    //每次两个字节
    for(;len>1;len-=2)
    {
        sum+=*data++;
        //右移，将超出的位置
        if(sum&0x80000000)
            sum=(sum&0xffff)+(sum>>16);
    }

    //奇数个字节的情况
    if(len==1)
    {
        u_short i=0;
        *(u_char*)(&i)=*(u_char*)data;
        sum+=i;
    }

    //对于溢出的位进行处理,sum占16位
    //在前十六位为1得情况下反复进行处理
    while(sum>>16)
    {
        sum=(sum&0xffff)+(sum>>16);
    }

    //在补码为0的情况下，并不使用一个补码，而是原封不动的将该值作为返回值
    return (sum==0xffff)?sum:~sum;
}
//创建tcp报头
void make_tcp_header(struct packet_tcp * packet,int src_ip,int src_port,int dst_ip,int dst_port,int seq,int ack,int datalen)
{
    packet->tcp.seq=htonl(seq);
    packet->tcp.ack_seq=htonl(ack);
    packet->tcp.source=htons(src_port);
    packet->tcp.dest=htons(dst_port);
    packet->tcp.doff=5;
    packet->tcp.syn=1;
    packet->tcp.window=htons(8192);
    packet->tcp.urg_ptr=0;

    //生成伪报头
    packet->ip.ip_ttl=0;
    packet->ip.ip_p=IPPROTO_TCP;
    packet->ip.ip_src.s_addr=src_ip;
    packet->ip.ip_dst.s_addr=dst_ip;
    packet->ip.ip_sum=htons(sizeof(packet->tcp)+datalen);

#define PSEUDO_HEADER_LEN 12
    packet->tcp.check=0;
    packet->tcp.check=checksum((u_short*)(&(packet->ip.ip_ttl)),PSEUDO_HEADER_LEN+sizeof(packet->tcp)+datalen);

}
//创建ip报头
void make_ip_header(struct ip* ip,int src_ip,int dst_ip,int iplen)
{
    ip->ip_v=IPVERSION;
    ip->ip_hl=sizeof(struct ip)>>2;
    ip->ip_id=0;

    ip->ip_len=htons(iplen);
    ip->ip_off=htons(0);

    ip->ip_ttl=2;
    ip->ip_p=IPPROTO_TCP;
    ip->ip_src.s_addr=src_ip;
    ip->ip_dst.s_addr=dst_ip;

    ip->ip_sum=0;
    ip->ip_sum=checksum((u_short*)ip,sizeof(struct ip));

}
//发送syn数据包
void send_syn(char* dst_ip_char,char* src_ip_char,int dport,int sport,int seq_1)
{
    struct packet_tcp send;//发送的tcp包
    struct sockaddr_in dest;
    u_int src_ip;
    u_int dst_ip;
    u_short src_port;
    u_short dst_port;

    int sockfd;
    u_int32_t seq;
    u_int32_t ack;
    int datalen;
    int iplen;
    int on=1;

    // char dst_ip_char[15]="192.168.108.62";
    // char src_ip_char[15]="192.168.92.148";
    // int dport=10000;
    // int sport=4000;
    // int seq_1=110;

    //打开一个RAW套接字
    if((sockfd=socket(AF_INET,SOCK_RAW,IPPROTO_RAW))<0)
    {
        perror("socket(SOCK_RAW)");
        exit(EXIT_FAILURE);
    }

    //生成自己的一个IP报头
    if(setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0)
    {
        perror("setsockopt(IPPROTO_IP,IP_HDRINCL)");
        exit(EXIT_FAILURE);
    }

    //设置报头的值
    memset((char*)&dest,0,sizeof(dest));
    dest.sin_family=AF_INET;
    dst_ip=dest.sin_addr.s_addr=inet_addr(dst_ip_char);
    dest.sin_port=htons(dport);
    src_ip=inet_addr(src_ip_char);

    dst_port=dport;
    src_port=sport;
    seq=seq_1;
    ack=0;
    datalen=0;

    iplen=datalen+sizeof(send.ip)+sizeof(send.tcp);

    memset((char*)&send,0,sizeof(send));

    make_tcp_header(&send,src_ip,src_port,dst_ip,dst_port,seq,ack,datalen);

    make_ip_header(&(send.ip),src_ip,dst_ip,iplen);

    printf("send syn packet to %s\n",dst_ip_char);
    if(sendto(sockfd,(char*)&send,iplen,0,(struct sockaddr*)&dest,sizeof(dest))<0)
    {
        perror("sendto\n");
        exit(EXIT_FAILURE);
    }

    printf("send success\n");
    
    //查看数据包的内容
    // char buff[4096];
    // memcpy(buff,(char*)&send,sizeof(ip)+sizeof(tcphdr));
    
    // for(int i=0;i<4000;i++)
    // {
    //     printf("%c",buff[i]);
    // }

    close(sockfd);
}
