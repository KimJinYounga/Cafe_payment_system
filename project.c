#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define c_size 50  //고객 최대수
struct menu {
   char product[30];  //상품이름
   int price;  //가격
};
struct customer {
   char id[30]; //휴대폰번호 : 중복없이 아이디생성 가능
   int password; //비밀번호 4자리
   int mypay; //내 잔고 입력.
};
typedef struct menu MENU;
typedef struct customer customer;
void first(MENU *menu, customer *c_data); //첫 화면
int login(customer *data, MENU *menu); //로그인, 내 잔고 확인, 회원가입 하는 함수
void menu_prt(MENU *menu); //메뉴출력 함수
void choice(MENU *menu, int *total);  //주문사항 선택 함수
void mypay_charge(customer *data, MENU *menu, int n, int total);//mypay 충전
void pay(customer *data, int total, int n);//mypay - 주문사항 계산함수
void sign_in(customer *data); //회원등록하는 함수
void sign_in_id(char *id1, char *id2); //회원등록 중 id등록
void sign_in_password(int *pass1, int *pass2); //회원등록 중 password등록
void charge(customer *data, MENU *menu); //잔고 계산

int main(){
   MENU menu[15]={{"아메리카노",1500},{"에스프레소",2000},{"마끼야또",2500},{"바닐라딜라이트",2500},{"카푸치노",1400},
					{"초콜릿",3500},{"모히또",3500},{"딸기쉐이크",3500},{"아이스티복숭아",2500},{"초코쿠키쉐이크",3500},
					{"레몬티",3800},{"자몽티",3500},{"유자티",3800},{"캐모마일",4000},{"밀크티",3300}};
   customer c_data[c_size] = {{"01049494476",4476,0},{"01073226164",6164,0}};//고객 데이터 저장 구조체 50개

   first(menu, c_data); // 첫 화면

   return 0;
}
void first(MENU *menu, customer *c_data){ //첫화면
   int choice1, i, n, total, temp;
   while(1){
      system("cls");
      
      for(temp=0;temp<c_size;temp++){ //data 중 처음으로 password가 0인 temp 찾음.(빈공간)
         if((c_data+temp)->password == 0)
            break;
      }
      printf("(1)메뉴선택 (2)회원등록 (3)종료\n");
      scanf("%d", &choice1);
      if(choice1 == 1){
         n = login(c_data , menu); // n = 로그인 한 배열 번째.
         printf("%d\n",n);

         menu_prt(menu); //메뉴출력

         choice(menu, &total); //메뉴고르기 total=총가격 

         mypay_charge(c_data,menu, n, total); //페이충전

         pay(c_data, total, n);// 주문사항 계산
      }
      else if(choice1 == 2){
         sign_in(c_data+temp);
         first(menu, c_data); //회원등록 후 다시 처음으로
      }
      else
         exit(1);
   }
}
void menu_prt(MENU *menu){ //메뉴출력
   int i;

   printf("                     <<메뉴판>>\n");
   printf("=========================================================================================\n\n");
   printf("	   【caffeine】    |         【non-caffeine】      |           【tea】\n");
   printf("			   |				   |\n");
   for(i=0;i<5;i++){
      printf("%2d.%-15s %6d  |    %2d.%-14s %6d   | %2d.%-15s %6d\n",i+1,menu->product, menu->price,
																	i+6,(menu + 5)->product,(menu + 5)->price,
																	i+11,(menu + 10)->product,(menu + 10)->price);
      menu++;
   }
   printf("\n☆Ice: +500원, size(middle: +500원, large: +1000원)\n\n");
   printf("=========================================================================================\n\n");
   puts("");
}
void choice(MENU *menu, int *total){ //주문사항 선택
   int i, j, price, count, totalprice=0;
   int choice1, choice2, choice3;

   printf("메뉴를 몇개 선택하시겠습니까 : ");
   scanf("%d", &count);//count = 선택할 메뉴의 개수
   printf("===============================================\n");
   for(i=0;i<count;i++){
      printf("메뉴를 선택하십시오 : ");
      scanf("%d",&choice1);
      printf("Hot(1), Ice(2)를 선택하십시오 : ");
      scanf("%d", &choice2);
      printf("사이즈 선택(small(1), middle(2), large(3)) : ");
      scanf("%d", &choice3);

      for(j=0;j<15;j++){
         if(choice1 == (j+1)){
            price = (menu+j)->price; //price=원래 가격
            if(choice2 == 2)
               price += 500;//price + ice가격(500)

            if(choice3 == 2) price += 500; //middle때
            else if(choice3 == 3) price += 1000; //large때

            totalprice+=price;
            printf("-----------------------------------------------\n");
            printf("            %-15s%d\n",(menu+j)->product, price);
            printf("-----------------------------------------------\n");
         } //고객의 주문사항을 문자열로 저장함.
      }
   }

   printf("               총합     %10d\n", totalprice);
   *total = totalprice; //총합을 total에 담아 상위 함수로 넘김.
}
int login(customer *data, MENU *menu){ //로그인, 내 잔고 확인하는 함수 choice로 넘어감..
   int i,temp,count=0, pass;
   int choice1;
   static int result=0;
   char phone_num[30]; //휴대폰번호 입력 변수

   printf("      <login>\n");
   printf("휴대폰 번호를 입력해주세요: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   fflush(stdin);
   gets(phone_num);

   for(temp=0;temp<c_size;temp++){ //data 중 처음으로 password가 0인 temp 찾음.(빈공간)
      if((data+temp)->password == 0)
         break;
   }
   for(i=0;i<c_size;i++){ //i=0부터 고객 최대수까지 검사
      if(!strcmp((data+i)->id,phone_num)){ //id 맞으면
         printf("비밀 번호를 입력해주세요: ");
         printf("____\b\b\b\b");
         scanf("%d",&pass);
         if((data+i)->password == pass){ //아이디따른 패스워드 입력
            printf("내 pay : %d원\n",(data+i)->mypay);
            count=60; //count=0에서 count=60으로 바뀜.
            
			result = i;
            break;
         }
         else{
            system("cls");
            printf("잘못된 번호입니다. 로그인 화면으로 돌아갑니다.\n");
            login(data, menu);
            count=60;

            result = i;
         }
         break;
      }
   }
   if(count==0) //for문 내에서 count에 다른숫자 안들어가면(id나 password틀리면)
   {
      system("cls");
      printf("잘못된 번호입니다.\n");
      printf("(1)회원등록 (2)다시입력\n");
      scanf("%d",&choice1);
      if(choice1 == 1){ //회원등록 들어가면
         sign_in(data+temp);
         (data+temp)->mypay = 0; //로그인 배열번째에 돈 0넣음.
         first(menu,data);
      }
      else if(choice1 == 2){ //다시입력
         login(data,menu);
      }
   }
   return result;
}
void sign_in(customer *data){ //회원등록하는 함수 여기서  data=data 중 저장할 공간.
   char phone_num1[30], phone_num2[30];
   int pass1, pass2, choice, i;
   printf("      <회원등록>\n");
   sign_in_id(phone_num1, phone_num2); //id등록,확인 함수
   strcpy(data->id,phone_num1); //id 저장완료(data(temp)번째에)
   printf("id가 등록되었습니다.\n");
   sign_in_password(&pass1, &pass2); //password 등록, 확인 함수
   data->password = pass1;
   printf("password가 등록되었습니다.\n");

   system("cls");
   printf("      <등록된 정보>\n");
   printf("id = %s\n",data->id);
   printf("password = %d\n",data->password);
   printf("확인하셨으면 (1)을 눌러주세요: ");
      scanf("%d", &choice);
}
void sign_in_id(char *id1, char *id2){ //회원등록 중 id등록
   fflush(stdin);
   printf("휴대폰 번호를 입력해주세요: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   gets(id1);
   printf("다시 한번 입력해주세요: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   gets(id2);
   if(strcmp(id1,id2) != 0){
      system("cls");
      printf("잘못입력하셨습니다.\n");
      printf("초기 화면으로 돌아갑니다.\n");
      printf("      <id 등록>\n");
      sign_in_id(id1, id2);
   }

}
void sign_in_password(int *pass1, int *pass2){ //회원등록 중 password등록
   printf("비밀번호 4자리를 입력해주세요: ____\b\b\b\b");
   scanf("%d", pass1);
   printf("다시 한번 입력해주세요: ____\b\b\b\b");
   scanf("%d", pass2);
   if(*pass1 != *pass2){
      system("cls");
      printf("잘못입력하셨습니다.\n");
      printf("초기 화면으로 돌아갑니다.\n");
      printf("      <password 등록>\n");
      sign_in_password(pass1, pass2);
   }
}
void mypay_charge(customer *data, MENU *menu, int n, int total){ //mypay 충전(n = 입력한 휴대폰번호 배열위치)
   int choice, choice2, price, charge;
   printf("내 pay : %d, 상품금액 : %d\n", (data+n)->mypay, total);
   printf("내 pay를 충전하시겠습니까?\n");
   printf("(1)YES (2)NO \n");
   scanf("%d", &choice);
   system("cls");
   if(choice==1){
      printf("      <pay 충전>\n");
      printf("총 상품 금액 : %d\n",total);
      printf("충전 금액을 입력해주세요 : ");
      scanf("%d", &price);
      charge = ((data+n)->mypay) + price;
      printf("내 pay : %d + %d = %d원\n", (data+n)->mypay, price, charge);
      (data+n)->mypay = ((data+n)->mypay) + price;
      printf("확인하셨으면 (1)을 눌러주세요: ");
      scanf("%d", &choice2);
   }
   else if(choice==2)
      if((data+n)->mypay < total){
         system("cls");
         printf("mypay가 총 금액보다 모자랍니다...\n");
         mypay_charge(data, menu, n, total);
      }
}
void pay(customer *data, int total, int n){ //mypay - 주문사항 계산함수
   int result = (data+n)->mypay - total;
   int last_result = result + (total*0.05);
   int choice;
   system("cls");
   printf("      <계산>\n");
   printf("%d - %d = %d\n",(data+n)->mypay,total,result);
   printf("내 남은 pay = %d + %.0lf(5%%적립) = %d\n",result, total*0.05, last_result);
   (data+n)->mypay= last_result;
   printf("확인하셨으면 (1)을 눌러주세요: ");
   scanf("%d", &choice);
}