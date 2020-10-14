#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define man 10000
int bank_money = 100 * man;


void enter_region() { 
	/*asm(
	".data\n"
	"lock:\n"
	".byte 0\n"
	".text\n"
	"_enter_region:\n"
	"movb $1, %al\n" // move 1 to AL
	"xchgb lock,%al\n"
	"cmp $0, %al\n" 
	"jne _enter_region\n"
	);*/
}



void leave_region() { //다빌렸다!
	//    asm("movb $0, lock");
}



void critical_region(char* p,int borrow_money) { //돈빌리는구간
if (bank_money - borrow_money < 0) {
printf("bank_money starve, can't borrow\n");
return 0;
}
else 
{
printf("before borrow bank_money:%d\n",bank_money);
bank_money -= borrow_money;
printf("%s는 %d을 빌린다.\n", p, borrow_money);
printf("대출 후 은행잔액:%d\n", bank_money);
int d = rand() % 1000000;
usleep(d);
}
}



void noncritical_region(char* p,int payback_money,int borrow_money) { //돈쓰는구간,
printf("%s 는 %d을 갚는다.\n", p, payback_money);
printf("%s의 남은 대출금액:%d",p,borrow_money);
bank_money+=payback_money;    
printf("대출금 회수 후 은행잔액:%d\n", bank_money);
int d = rand() % 1000000;
usleep(d);
}



static void* f1(void* p) {
int borrow_money = 0;
int payback_money = 0;
int borrow_num=0; //대출횟수 10번까지만
int temp;
do {
if(borrow_num<10){
puts("f1 wait\n");
enter_region();
if (bank_money >= 10000) {
temp = (rand() % (bank_money / man) + 1) * man; //10000~은행보유금액까지
borrow_money+=temp;
printf("IN CS, 1 start borrow \n");
critical_region(p, temp);
printf("CS END.\n");
leave_region();
borrow_num++;
}
} //돈빌리는 부분, 은행이 최소 10000원 이상은 있어야 빌릴수있다.
if (borrow_money >= man) {
payback_money = ((rand() % (borrow_money / man)) + 1) * man;
borrow_money-=payback_money;
noncritical_region(p,payback_money,borrow_money);
}
}while(borrow_money > 0);
return NULL;
}

static void* f2(void* p) {
int borrow_money = 0;
int payback_money = 0;
int borrow_num=0;
int temp;
do {
if(borrow_num<10){
puts("f2 wait\n");
enter_region();
if (bank_money >= 10000) {
temp = (rand() % (bank_money / man) + 1) * man; //10000~은행보유금액까지
borrow_money+=temp;
printf("IN CS f2 start borrow\n");
critical_region(p, temp);
printf("CS END.\n");
leave_region();
}
} //돈빌리는 부분, 은행이 최소 10000원 이상은 있어야 빌릴수있다.
if (borrow_money >= man) {
payback_money = ((rand() % (borrow_money / man)) + 1) * man;
borrow_money-=payback_money;
noncritical_region(p,payback_money,borrow_money);
}
} while (borrow_money > 0);
return NULL;
}

static void* f3(void* p) {
int borrow_money = 0;
int payback_money = 0;
int borrow_num=0;
int temp;
do {
if(borrow_num<10){
puts("f3 wait\n");
enter_region();
if (bank_money >= 10000) {
temp = (rand() % (bank_money / man) + 1) * man; //10000~은행보유금액까지
borrow_money+=temp;
printf("IN CS f3 start borrow\n");
critical_region(p, temp);
printf("CS END\n");
leave_region();
borrow_num++;
} //돈빌리는 부분, 은행이 최소 10000원 이상은 있어야 빌릴수있다.
}
if (borrow_money >= man) {
payback_money = ((rand() % (borrow_money / man)) + 1) * man;
borrow_money-=payback_money;												
noncritical_region(p,payback_money,borrow_money);
}
} while (borrow_money > 0);
return NULL;
}

static void* f4(void* p) {
int borrow_money = 0;
int payback_money = 0;
int borrow_num=0;
int temp;
do {
if(borrow_num<10){
puts("f4 wait\n");
enter_region();
if (bank_money >= 10000) {
temp = (rand() % (bank_money / man) + 1) * man; //10000~은행보유금액까지
borrow_money+=temp;
printf("IN CS f4 start borrow\n");
critical_region(p, temp);
printf("CS end\n");
leave_region();
borrow_num++;
}
} //돈빌리는 부분, 은행이 최소 10000원 이상은 있어야 빌릴수있다.

if (borrow_money >= man) {
payback_money = ((rand() % (borrow_money / man)) + 1) * man;
borrow_money-=payback_money;
noncritical_region(p,payback_money,borrow_money);
}
} while (borrow_money > 0);
return NULL;
}

static void* f5(void* p) {
int borrow_money = 0;
int payback_money = 0;
int borrow_num=0;
int temp;
do {
if(borrow_num<10){
puts("f5 wait");
enter_region();
if (bank_money >= 10000) {
temp = (rand() % (bank_money / man) + 1) * man; //10000~은행보유금액까지
borrow_money+=temp;
printf("IN CS f5 start borrow\n");
critical_region(p, temp);
printf("CS END\n");
leave_region();
borrow_num++;
}
} //돈빌리는 부분, 은행이 최소 10000원 이상은 있어야 빌릴수있다.

if (borrow_money >= man) {
payback_money = ((rand() % (borrow_money / man)) + 1) * man;
borrow_money-=payback_money;
noncritical_region(p,payback_money,borrow_money);
} 
} while (borrow_money > 0);
return NULL;

}

int main() {
int rc;
pthread_t t1, t2,t3,t4,t5;
rc = pthread_create(&t1, NULL, f1, "f1");
if (rc != 0) {
fprintf(stderr, "pthread f1 failed\n");
return EXIT_FAILURE;
}
rc = pthread_create(&t2, NULL, f2, "f2");
if (rc != 0) {
fprintf(stderr, "pthread f2 failed\n");
return EXIT_FAILURE;
}
rc = pthread_create(&t3, NULL, f3, "f3");
if (rc != 0) {
fprintf(stderr, "pthread f3 failed\n");
return EXIT_FAILURE;
}
rc = pthread_create(&t4, NULL, f4, "f4");
if (rc != 0) {
fprintf(stderr, "pthread f4 failed\n");
return EXIT_FAILURE;
}
rc = pthread_create(&t5, NULL, f5, "f5");
if (rc != 0) {
fprintf(stderr, "pthread f5 failed\n");
return EXIT_FAILURE;
}
pthread_join(t1, NULL);
pthread_join(t2, NULL);
pthread_join(t3, NULL);
pthread_join(t4, NULL);
pthread_join(t5, NULL);
puts("All threads finished.");
return 0;
}
